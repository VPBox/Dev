//
// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_INPLACE_GENERATOR_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_INPLACE_GENERATOR_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "update_engine/payload_generator/blob_file_writer.h"
#include "update_engine/payload_generator/delta_diff_generator.h"
#include "update_engine/payload_generator/graph_types.h"
#include "update_engine/payload_generator/operations_generator.h"

// InplaceGenerator contains all functionality related to the inplace algorithm
// for generating update payloads. These are the functions used when delta minor
// version is 1.

namespace chromeos_update_engine {

// This struct stores all relevant info for an edge that is cut between
// nodes old_src -> old_dst by creating new vertex new_vertex. The new
// relationship is:
// old_src -(read before)-> new_vertex <-(write before)- old_dst
// new_vertex is a MOVE operation that moves some existing blocks into
// temp space. The temp extents are, by necessity, stored in new_vertex
// (as dst extents) and old_dst (as src extents), but they are also broken
// out into tmp_extents, as the nodes themselves may contain many more
// extents.
struct CutEdgeVertexes {
  Vertex::Index new_vertex;
  Vertex::Index old_src;
  Vertex::Index old_dst;
  std::vector<Extent> tmp_extents;
};

class InplaceGenerator : public OperationsGenerator {
 public:
  // Represents a disk block on the install partition.
  struct Block {
    // During install, each block on the install partition will be written
    // and some may be read (in all likelihood, many will be read).
    // The reading and writing will be performed by InstallOperations,
    // each of which has a corresponding vertex in a graph.
    // A Block object tells which vertex will read or write this block
    // at install time.
    // Generally, there will be a vector of Block objects whose length
    // is the number of blocks on the install partition.
    Block() : reader(Vertex::kInvalidIndex), writer(Vertex::kInvalidIndex) {}
    Vertex::Index reader;
    Vertex::Index writer;
  };

  InplaceGenerator() = default;

  // Checks all the operations in the graph have a type assigned.
  static void CheckGraph(const Graph& graph);

  // Modifies blocks read by 'op' so that any blocks referred to by
  // 'remove_extents' are replaced with blocks from 'replace_extents'.
  // 'remove_extents' and 'replace_extents' must be the same number of blocks.
  // Blocks will be substituted in the order listed in the vectors.
  // E.g. if 'op' reads blocks 1, 2, 3, 4, 5, 6, 7, 8, remove_extents
  // contains blocks 6, 2, 3, 5, and replace blocks contains
  // 12, 13, 14, 15, then op will be changed to read from:
  // 1, 13, 14, 4, 15, 12, 7, 8
  static void SubstituteBlocks(Vertex* vertex,
                               const std::vector<Extent>& remove_extents,
                               const std::vector<Extent>& replace_extents);

  // Cuts 'edges' from 'graph' according to the AU algorithm. This means
  // for each edge A->B, remove the dependency that B occur before A.
  // Do this by creating a new operation X that copies from the blocks
  // specified by the edge's properties to temp space T. Modify B to read
  // from T rather than the blocks in the edge. Modify A to depend on X,
  // but not on B. Free space is found by looking in 'blocks'.
  // Returns true on success.
  static bool CutEdges(Graph* graph,
                       const std::set<Edge>& edges,
                       std::vector<CutEdgeVertexes>* out_cuts);

  // Creates all the edges for the graph. Writers of a block point to
  // readers of the same block. This is because for an edge A->B, B
  // must complete before A executes.
  static void CreateEdges(Graph* graph, const std::vector<Block>& blocks);

  // Takes |op_indexes|, which is effectively a mapping from order in
  // which the op is performed -> graph vertex index, and produces the
  // reverse: a mapping from graph vertex index -> op_indexes index.
  static void GenerateReverseTopoOrderMap(
      const std::vector<Vertex::Index>& op_indexes,
      std::vector<std::vector<Vertex::Index>::size_type>* reverse_op_indexes);

  // Sorts the vector |cuts| by its |cuts[].old_dest| member. Order is
  // determined by the order of elements in op_indexes.
  static void SortCutsByTopoOrder(const std::vector<Vertex::Index>& op_indexes,
                                  std::vector<CutEdgeVertexes>* cuts);

  // Given a topologically sorted graph |op_indexes| and |graph|, alters
  // |op_indexes| to move all the full operations to the end of the vector.
  // Full operations should not be depended on, so this is safe.
  static void MoveAndSortFullOpsToBack(Graph* graph,
                                       std::vector<Vertex::Index>* op_indexes);

  // Returns true iff there are no extents in the graph that refer to temp
  // blocks. Temp blocks are in the range [kTempBlockStart, kSparseHole).
  static bool NoTempBlocksRemain(const Graph& graph);

  // Takes a |graph|, which has edges that must be cut, as listed in
  // |cuts|.  Cuts the edges. Maintains a list in which the operations
  // will be performed (in |op_indexes|) and the reverse (in
  // |reverse_op_indexes|).  Cutting edges requires scratch space, and
  // if insufficient scratch is found, the file is reread and will be
  // send down (either as REPLACE or REPLACE_BZ).  Returns true on
  // success.
  static bool AssignTempBlocks(
      Graph* graph,
      const std::string& new_part,
      BlobFileWriter* blob_file,
      std::vector<Vertex::Index>* op_indexes,
      std::vector<std::vector<Vertex::Index>::size_type>* reverse_op_indexes,
      const std::vector<CutEdgeVertexes>& cuts);

  // Handles allocation of temp blocks to a cut edge by converting the
  // dest node to a full op. This removes the need for temp blocks, but
  // comes at the cost of a worse compression ratio.
  // For example, say we have A->B->A. It would first be cut to form:
  // A->B->N<-A, where N copies blocks to temp space. If there are no
  // temp blocks, this function can be called to convert it to the form:
  // A->B. Now, A is a full operation.
  static bool ConvertCutToFullOp(Graph* graph,
                                 const CutEdgeVertexes& cut,
                                 const std::string& new_part,
                                 BlobFileWriter* blob_file);

  // Takes a graph, which is not a DAG, which represents the files just
  // read from disk, and converts it into a DAG by breaking all cycles
  // and finding temp space to resolve broken edges.
  // The final order of the nodes is given in |final_order|
  // Some files may need to be reread from disk, thus |fd| and
  // |data_file_size| are be passed.
  // If |scratch_vertex| is not kInvalidIndex, removes it from
  // |final_order| before returning.
  // Returns true on success.
  static bool ConvertGraphToDag(Graph* graph,
                                const std::string& new_part,
                                BlobFileWriter* blob_file,
                                std::vector<Vertex::Index>* final_order,
                                Vertex::Index scratch_vertex);

  // Creates a dummy REPLACE_BZ node in the given |vertex|. This can be used
  // to provide scratch space. The node writes |num_blocks| blocks starting at
  // |start_block|The node should be marked invalid before writing all nodes to
  // the output file.
  static void CreateScratchNode(uint64_t start_block,
                                uint64_t num_blocks,
                                Vertex* vertex);

  // The |blocks| vector contains a reader and writer for each block on the
  // filesystem that's being in-place updated. We populate the reader/writer
  // fields of |blocks| by calling this function.
  // For each block in |operation| that is read or written, find that block
  // in |blocks| and set the reader/writer field to the vertex passed.
  // |graph| is not strictly necessary, but useful for printing out
  // error messages.
  static bool AddInstallOpToBlocksVector(const InstallOperation& operation,
                                         const Graph& graph,
                                         Vertex::Index vertex,
                                         std::vector<Block>* blocks);

  // Add a vertex (if |existing_vertex| is kInvalidVertex) or update an
  // |existing_vertex| with the passed |operation|.
  // This method will also register the vertex as the reader or writer of the
  // blocks involved in the operation updating the |blocks| vector. The
  // |op_name| associated with the Vertex is used for logging purposes.
  static bool AddInstallOpToGraph(Graph* graph,
                                  Vertex::Index existing_vertex,
                                  std::vector<Block>* blocks,
                                  const InstallOperation& operation,
                                  const std::string& op_name);

  // Apply the transformation stored in |the_map| to the |collection| vector
  // replacing the map keys found in |collection| with its associated value in
  // |the_map|.
  static void ApplyMap(std::vector<uint64_t>* collection,
                       const std::map<uint64_t, uint64_t>& the_map);

  // Resolve all read-after-write dependencies in the operation list |aops|. The
  // operations in |aops| are such that they generate the desired |new_part| if
  // applied reading always from the original image. This function reorders the
  // operations and generates new operations when needed to make these
  // operations produce the same |new_part| result when applied in-place.
  // The new operations will create blobs in |data_file_fd| and update
  // the file size pointed by |data_file_size| if needed.
  // On success, stores the new operations in |aops| in the right order and
  // returns true.
  static bool ResolveReadAfterWriteDependencies(
      const PartitionConfig& old_part,
      const PartitionConfig& new_part,
      uint64_t partition_size,
      size_t block_size,
      BlobFileWriter* blob_file,
      std::vector<AnnotatedOperation>* aops);

  // Generate the update payload operations for the given partition using
  // only operations that read from the target and/or write to the target,
  // hence, applying the payload "in-place" in the target partition. This method
  // assumes that the contents of the source image are pre-copied to the target
  // partition, up to the size of the source image. Use this method to generate
  // a delta update with the minor version kInPlaceMinorPayloadVersion.
  // The operations are stored in |aops|. All the offsets in the operations
  // reference the data written to |blob_file|.
  bool GenerateOperations(const PayloadGenerationConfig& config,
                          const PartitionConfig& old_part,
                          const PartitionConfig& new_part,
                          BlobFileWriter* blob_file,
                          std::vector<AnnotatedOperation>* aops) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(InplaceGenerator);
};

};  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_INPLACE_GENERATOR_H_
