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

#include "update_engine/payload_generator/inplace_generator.h"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <base/stl_util.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/cycle_breaker.h"
#include "update_engine/payload_generator/delta_diff_generator.h"
#include "update_engine/payload_generator/delta_diff_utils.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/graph_types.h"
#include "update_engine/payload_generator/graph_utils.h"
#include "update_engine/payload_generator/topological_sort.h"
#include "update_engine/update_metadata.pb.h"

using std::make_pair;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

namespace chromeos_update_engine {

using Block = InplaceGenerator::Block;

namespace {

// The only PayloadVersion supported by this implementation.
const PayloadVersion kInPlacePayloadVersion{kChromeOSMajorPayloadVersion,
                                            kInPlaceMinorPayloadVersion};

// This class allocates non-existent temp blocks, starting from
// kTempBlockStart. Other code is responsible for converting these
// temp blocks into real blocks, as the client can't read or write to
// these blocks.
class DummyExtentAllocator {
 public:
  vector<Extent> Allocate(const uint64_t block_count) {
    vector<Extent> ret(1);
    ret[0].set_start_block(next_block_);
    ret[0].set_num_blocks(block_count);
    next_block_ += block_count;
    return ret;
  }

 private:
  uint64_t next_block_{kTempBlockStart};
};

// Takes a vector of blocks and returns an equivalent vector of Extent
// objects.
vector<Extent> CompressExtents(const vector<uint64_t>& blocks) {
  vector<Extent> new_extents;
  for (uint64_t block : blocks) {
    AppendBlockToExtents(&new_extents, block);
  }
  return new_extents;
}

// Helper class to compare two operations by start block of the first Extent in
// their destination extents given the index of the operations in the graph.
class IndexedInstallOperationsDstComparator {
 public:
  explicit IndexedInstallOperationsDstComparator(Graph* graph)
      : graph_(graph) {}

  // Compares the operations in the vertex a and b of graph_.
  bool operator()(size_t a, size_t b) const {
    return diff_utils::CompareAopsByDestination((*graph_)[a].aop,
                                                (*graph_)[b].aop);
  }

 private:
  const Graph* const graph_;
};

}  // namespace

void InplaceGenerator::CheckGraph(const Graph& graph) {
  for (const Vertex& v : graph) {
    CHECK(v.aop.op.has_type());
  }
}

void InplaceGenerator::SubstituteBlocks(Vertex* vertex,
                                        const vector<Extent>& remove_extents,
                                        const vector<Extent>& replace_extents) {
  // First, expand out the blocks that op reads from
  vector<uint64_t> read_blocks = ExpandExtents(vertex->aop.op.src_extents());
  {
    // Expand remove_extents and replace_extents
    vector<uint64_t> remove_extents_expanded = ExpandExtents(remove_extents);
    vector<uint64_t> replace_extents_expanded = ExpandExtents(replace_extents);
    CHECK_EQ(remove_extents_expanded.size(), replace_extents_expanded.size());
    map<uint64_t, uint64_t> conversion;
    for (vector<uint64_t>::size_type i = 0; i < replace_extents_expanded.size();
         i++) {
      conversion[remove_extents_expanded[i]] = replace_extents_expanded[i];
    }
    ApplyMap(&read_blocks, conversion);
    for (auto& edge_prop_pair : vertex->out_edges) {
      vector<uint64_t> write_before_deps_expanded =
          ExpandExtents(edge_prop_pair.second.write_extents);
      ApplyMap(&write_before_deps_expanded, conversion);
      edge_prop_pair.second.write_extents =
          CompressExtents(write_before_deps_expanded);
    }
  }
  // Convert read_blocks back to extents
  vertex->aop.op.clear_src_extents();
  vector<Extent> new_extents = CompressExtents(read_blocks);
  StoreExtents(new_extents, vertex->aop.op.mutable_src_extents());
}

bool InplaceGenerator::CutEdges(Graph* graph,
                                const set<Edge>& edges,
                                vector<CutEdgeVertexes>* out_cuts) {
  DummyExtentAllocator scratch_allocator;
  vector<CutEdgeVertexes> cuts;
  cuts.reserve(edges.size());

  uint64_t scratch_blocks_used = 0;
  for (const Edge& edge : edges) {
    cuts.resize(cuts.size() + 1);
    vector<Extent> old_extents =
        (*graph)[edge.first].out_edges[edge.second].extents;
    // Choose some scratch space
    scratch_blocks_used += graph_utils::EdgeWeight(*graph, edge);
    cuts.back().tmp_extents =
        scratch_allocator.Allocate(graph_utils::EdgeWeight(*graph, edge));
    // create vertex to copy original->scratch
    cuts.back().new_vertex = graph->size();
    graph->emplace_back();
    cuts.back().old_src = edge.first;
    cuts.back().old_dst = edge.second;

    EdgeProperties& cut_edge_properties =
        (*graph)[edge.first].out_edges.find(edge.second)->second;

    // This should never happen, as we should only be cutting edges between
    // real file nodes, and write-before relationships are created from
    // a real file node to a temp copy node:
    CHECK(cut_edge_properties.write_extents.empty())
        << "Can't cut edge that has write-before relationship.";

    // make node depend on the copy operation
    (*graph)[edge.first].out_edges.insert(
        make_pair(graph->size() - 1, cut_edge_properties));

    // Set src/dst extents and other proto variables for copy operation
    graph->back().aop.op.set_type(InstallOperation::MOVE);
    StoreExtents(cut_edge_properties.extents,
                 graph->back().aop.op.mutable_src_extents());
    StoreExtents(cuts.back().tmp_extents,
                 graph->back().aop.op.mutable_dst_extents());
    graph->back().aop.op.set_src_length(graph_utils::EdgeWeight(*graph, edge) *
                                        kBlockSize);
    graph->back().aop.op.set_dst_length(graph->back().aop.op.src_length());

    // make the dest node read from the scratch space
    SubstituteBlocks(&((*graph)[edge.second]),
                     (*graph)[edge.first].out_edges[edge.second].extents,
                     cuts.back().tmp_extents);

    // delete the old edge
    CHECK_EQ(static_cast<Graph::size_type>(1),
             (*graph)[edge.first].out_edges.erase(edge.second));

    // Add an edge from dst to copy operation
    EdgeProperties write_before_edge_properties;
    write_before_edge_properties.write_extents = cuts.back().tmp_extents;
    (*graph)[edge.second].out_edges.insert(
        make_pair(graph->size() - 1, write_before_edge_properties));
  }
  out_cuts->swap(cuts);
  return true;
}

// Creates all the edges for the graph. Writers of a block point to
// readers of the same block. This is because for an edge A->B, B
// must complete before A executes.
void InplaceGenerator::CreateEdges(Graph* graph, const vector<Block>& blocks) {
  for (vector<Block>::size_type i = 0; i < blocks.size(); i++) {
    // Blocks with both a reader and writer get an edge
    if (blocks[i].reader == Vertex::kInvalidIndex ||
        blocks[i].writer == Vertex::kInvalidIndex)
      continue;
    // Don't have a node depend on itself
    if (blocks[i].reader == blocks[i].writer)
      continue;
    // See if there's already an edge we can add onto
    Vertex::EdgeMap::iterator edge_it =
        (*graph)[blocks[i].writer].out_edges.find(blocks[i].reader);
    if (edge_it == (*graph)[blocks[i].writer].out_edges.end()) {
      // No existing edge. Create one
      (*graph)[blocks[i].writer].out_edges.insert(
          make_pair(blocks[i].reader, EdgeProperties()));
      edge_it = (*graph)[blocks[i].writer].out_edges.find(blocks[i].reader);
      CHECK(edge_it != (*graph)[blocks[i].writer].out_edges.end());
    }
    AppendBlockToExtents(&edge_it->second.extents, i);
  }
}

namespace {

class SortCutsByTopoOrderLess {
 public:
  explicit SortCutsByTopoOrderLess(
      const vector<vector<Vertex::Index>::size_type>& table)
      : table_(table) {}
  bool operator()(const CutEdgeVertexes& a, const CutEdgeVertexes& b) {
    return table_[a.old_dst] < table_[b.old_dst];
  }

 private:
  const vector<vector<Vertex::Index>::size_type>& table_;
};

}  // namespace

void InplaceGenerator::GenerateReverseTopoOrderMap(
    const vector<Vertex::Index>& op_indexes,
    vector<vector<Vertex::Index>::size_type>* reverse_op_indexes) {
  vector<vector<Vertex::Index>::size_type> table(op_indexes.size());
  for (vector<Vertex::Index>::size_type i = 0, e = op_indexes.size(); i != e;
       ++i) {
    Vertex::Index node = op_indexes[i];
    if (table.size() < (node + 1)) {
      table.resize(node + 1);
    }
    table[node] = i;
  }
  reverse_op_indexes->swap(table);
}

void InplaceGenerator::SortCutsByTopoOrder(
    const vector<Vertex::Index>& op_indexes, vector<CutEdgeVertexes>* cuts) {
  // first, make a reverse lookup table.
  vector<vector<Vertex::Index>::size_type> table;
  GenerateReverseTopoOrderMap(op_indexes, &table);
  SortCutsByTopoOrderLess less(table);
  sort(cuts->begin(), cuts->end(), less);
}

void InplaceGenerator::MoveAndSortFullOpsToBack(
    Graph* graph, vector<Vertex::Index>* op_indexes) {
  vector<Vertex::Index> ret;
  vector<Vertex::Index> full_ops;
  ret.reserve(op_indexes->size());
  for (auto op_index : *op_indexes) {
    InstallOperation::Type type = (*graph)[op_index].aop.op.type();
    if (type == InstallOperation::REPLACE ||
        type == InstallOperation::REPLACE_BZ) {
      full_ops.push_back(op_index);
    } else {
      ret.push_back(op_index);
    }
  }
  LOG(INFO) << "Stats: " << full_ops.size() << " full ops out of "
            << (full_ops.size() + ret.size()) << " total ops.";
  // Sort full ops according to their dst_extents.
  sort(full_ops.begin(),
       full_ops.end(),
       IndexedInstallOperationsDstComparator(graph));
  ret.insert(ret.end(), full_ops.begin(), full_ops.end());
  op_indexes->swap(ret);
}

namespace {

template <typename T>
bool TempBlocksExistInExtents(const T& extents) {
  for (const auto& extent : extents) {
    uint64_t start = extent.start_block();
    uint64_t num = extent.num_blocks();
    if (start >= kTempBlockStart || (start + num) >= kTempBlockStart) {
      LOG(ERROR) << "temp block!";
      LOG(ERROR) << "start: " << start << ", num: " << num;
      LOG(ERROR) << "kTempBlockStart: " << kTempBlockStart;
      LOG(ERROR) << "returning true";
      return true;
    }
    // check for wrap-around, which would be a bug:
    CHECK(start <= (start + num));
  }
  return false;
}

// Converts the cuts, which must all have the same |old_dst| member,
// to full. It does this by converting the |old_dst| to REPLACE or
// REPLACE_BZ, dropping all incoming edges to |old_dst|, and marking
// all temp nodes invalid.
bool ConvertCutsToFull(
    Graph* graph,
    const string& new_part,
    BlobFileWriter* blob_file,
    vector<Vertex::Index>* op_indexes,
    vector<vector<Vertex::Index>::size_type>* reverse_op_indexes,
    const vector<CutEdgeVertexes>& cuts) {
  CHECK(!cuts.empty());
  set<Vertex::Index> deleted_nodes;
  for (const CutEdgeVertexes& cut : cuts) {
    TEST_AND_RETURN_FALSE(
        InplaceGenerator::ConvertCutToFullOp(graph, cut, new_part, blob_file));
    deleted_nodes.insert(cut.new_vertex);
  }
  deleted_nodes.insert(cuts[0].old_dst);

  vector<Vertex::Index> new_op_indexes;
  new_op_indexes.reserve(op_indexes->size());
  for (Vertex::Index vertex_index : *op_indexes) {
    if (base::ContainsKey(deleted_nodes, vertex_index))
      continue;
    new_op_indexes.push_back(vertex_index);
  }
  new_op_indexes.push_back(cuts[0].old_dst);
  op_indexes->swap(new_op_indexes);
  InplaceGenerator::GenerateReverseTopoOrderMap(*op_indexes,
                                                reverse_op_indexes);
  return true;
}

// Tries to assign temp blocks for a collection of cuts, all of which share
// the same old_dst member. If temp blocks can't be found, old_dst will be
// converted to a REPLACE or REPLACE_BZ operation. Returns true on success,
// which can happen even if blocks are converted to full. Returns false
// on exceptional error cases.
bool AssignBlockForAdjoiningCuts(
    Graph* graph,
    const string& new_part,
    BlobFileWriter* blob_file,
    vector<Vertex::Index>* op_indexes,
    vector<vector<Vertex::Index>::size_type>* reverse_op_indexes,
    const vector<CutEdgeVertexes>& cuts) {
  CHECK(!cuts.empty());
  const Vertex::Index old_dst = cuts[0].old_dst;
  // Calculate # of blocks needed
  uint64_t blocks_needed = 0;
  vector<uint64_t> cuts_blocks_needed(cuts.size());
  for (vector<CutEdgeVertexes>::size_type i = 0; i < cuts.size(); ++i) {
    uint64_t cut_blocks_needed = 0;
    for (const Extent& extent : cuts[i].tmp_extents) {
      cut_blocks_needed += extent.num_blocks();
    }
    blocks_needed += cut_blocks_needed;
    cuts_blocks_needed[i] = cut_blocks_needed;
  }

  // Find enough blocks
  ExtentRanges scratch_ranges;
  // Each block that's supplying temp blocks and the corresponding blocks:
  typedef vector<pair<Vertex::Index, ExtentRanges>> SupplierVector;
  SupplierVector block_suppliers;
  uint64_t scratch_blocks_found = 0;
  for (vector<Vertex::Index>::size_type i = (*reverse_op_indexes)[old_dst] + 1,
                                        e = op_indexes->size();
       i < e;
       ++i) {
    Vertex::Index test_node = (*op_indexes)[i];
    if (!(*graph)[test_node].valid)
      continue;
    // See if this node has sufficient blocks
    ExtentRanges ranges;
    ranges.AddRepeatedExtents((*graph)[test_node].aop.op.dst_extents());
    ranges.SubtractExtent(
        ExtentForRange(kTempBlockStart, kSparseHole - kTempBlockStart));
    ranges.SubtractRepeatedExtents((*graph)[test_node].aop.op.src_extents());
    // For now, for simplicity, subtract out all blocks in read-before
    // dependencies.
    for (Vertex::EdgeMap::const_iterator
             edge_i = (*graph)[test_node].out_edges.begin(),
             edge_e = (*graph)[test_node].out_edges.end();
         edge_i != edge_e;
         ++edge_i) {
      ranges.SubtractExtents(edge_i->second.extents);
    }

    // Prevent using the block 0 as scratch space due to crbug.com/480751.
    if (ranges.ContainsBlock(0)) {
      LOG(INFO) << "Removing block 0 from the selected scratch range in vertex "
                << i;
      ranges.SubtractBlock(0);
    }

    if (ranges.blocks() == 0)
      continue;

    if (ranges.blocks() + scratch_blocks_found > blocks_needed) {
      // trim down ranges
      vector<Extent> new_ranges =
          ranges.GetExtentsForBlockCount(blocks_needed - scratch_blocks_found);
      ranges = ExtentRanges();
      ranges.AddExtents(new_ranges);
    }
    scratch_ranges.AddRanges(ranges);
    block_suppliers.push_back(make_pair(test_node, ranges));
    scratch_blocks_found += ranges.blocks();
    if (scratch_ranges.blocks() >= blocks_needed)
      break;
  }
  if (scratch_ranges.blocks() < blocks_needed) {
    LOG(INFO) << "Unable to find sufficient scratch";
    TEST_AND_RETURN_FALSE(ConvertCutsToFull(
        graph, new_part, blob_file, op_indexes, reverse_op_indexes, cuts));
    return true;
  }
  // Use the scratch we found
  TEST_AND_RETURN_FALSE(scratch_ranges.blocks() == scratch_blocks_found);

  // Make all the suppliers depend on this node
  for (const auto& index_range_pair : block_suppliers) {
    graph_utils::AddReadBeforeDepExtents(
        &(*graph)[index_range_pair.first],
        old_dst,
        index_range_pair.second.GetExtentsForBlockCount(
            index_range_pair.second.blocks()));
  }

  // Replace temp blocks in each cut
  for (vector<CutEdgeVertexes>::size_type i = 0; i < cuts.size(); ++i) {
    const CutEdgeVertexes& cut = cuts[i];
    vector<Extent> real_extents =
        scratch_ranges.GetExtentsForBlockCount(cuts_blocks_needed[i]);
    scratch_ranges.SubtractExtents(real_extents);

    // Fix the old dest node w/ the real blocks
    InplaceGenerator::SubstituteBlocks(
        &(*graph)[old_dst], cut.tmp_extents, real_extents);

    // Fix the new node w/ the real blocks. Since the new node is just a
    // copy operation, we can replace all the dest extents w/ the real
    // blocks.
    InstallOperation* op = &(*graph)[cut.new_vertex].aop.op;
    op->clear_dst_extents();
    StoreExtents(real_extents, op->mutable_dst_extents());
  }
  return true;
}

}  // namespace

bool InplaceGenerator::AssignTempBlocks(
    Graph* graph,
    const string& new_part,
    BlobFileWriter* blob_file,
    vector<Vertex::Index>* op_indexes,
    vector<vector<Vertex::Index>::size_type>* reverse_op_indexes,
    const vector<CutEdgeVertexes>& cuts) {
  CHECK(!cuts.empty());

  // group of cuts w/ the same old_dst:
  vector<CutEdgeVertexes> cuts_group;

  for (vector<CutEdgeVertexes>::size_type i = cuts.size() - 1, e = 0; true;
       --i) {
    LOG(INFO) << "Fixing temp blocks in cut " << i
              << ": old dst: " << cuts[i].old_dst
              << " new vertex: " << cuts[i].new_vertex
              << " path: " << (*graph)[cuts[i].old_dst].aop.name;

    if (cuts_group.empty() || (cuts_group[0].old_dst == cuts[i].old_dst)) {
      cuts_group.push_back(cuts[i]);
    } else {
      CHECK(!cuts_group.empty());
      TEST_AND_RETURN_FALSE(AssignBlockForAdjoiningCuts(graph,
                                                        new_part,
                                                        blob_file,
                                                        op_indexes,
                                                        reverse_op_indexes,
                                                        cuts_group));
      cuts_group.clear();
      cuts_group.push_back(cuts[i]);
    }

    if (i == e) {
      // break out of for() loop
      break;
    }
  }
  CHECK(!cuts_group.empty());
  TEST_AND_RETURN_FALSE(AssignBlockForAdjoiningCuts(
      graph, new_part, blob_file, op_indexes, reverse_op_indexes, cuts_group));
  return true;
}

bool InplaceGenerator::NoTempBlocksRemain(const Graph& graph) {
  size_t idx = 0;
  for (Graph::const_iterator it = graph.begin(), e = graph.end(); it != e;
       ++it, ++idx) {
    if (!it->valid)
      continue;
    const InstallOperation& op = it->aop.op;
    if (TempBlocksExistInExtents(op.dst_extents()) ||
        TempBlocksExistInExtents(op.src_extents())) {
      LOG(INFO) << "bad extents in node " << idx;
      LOG(INFO) << "so yeah";
      return false;
    }

    // Check out-edges:
    for (const auto& edge_prop_pair : it->out_edges) {
      if (TempBlocksExistInExtents(edge_prop_pair.second.extents) ||
          TempBlocksExistInExtents(edge_prop_pair.second.write_extents)) {
        LOG(INFO) << "bad out edge in node " << idx;
        LOG(INFO) << "so yeah";
        return false;
      }
    }
  }
  return true;
}

bool InplaceGenerator::ConvertCutToFullOp(Graph* graph,
                                          const CutEdgeVertexes& cut,
                                          const string& new_part,
                                          BlobFileWriter* blob_file) {
  // Drop all incoming edges, keep all outgoing edges

  // Keep all outgoing edges
  if ((*graph)[cut.old_dst].aop.op.type() != InstallOperation::REPLACE_BZ &&
      (*graph)[cut.old_dst].aop.op.type() != InstallOperation::REPLACE) {
    Vertex::EdgeMap out_edges = (*graph)[cut.old_dst].out_edges;
    graph_utils::DropWriteBeforeDeps(&out_edges);

    // Replace the operation with a REPLACE or REPLACE_BZ to generate the same
    // |new_extents| list of blocks and update the graph.
    vector<AnnotatedOperation> new_aop;
    vector<Extent> new_extents;
    ExtentsToVector((*graph)[cut.old_dst].aop.op.dst_extents(), &new_extents);
    TEST_AND_RETURN_FALSE(diff_utils::DeltaReadFile(
        &new_aop,
        "",  // old_part
        new_part,
        vector<Extent>(),  // old_extents
        new_extents,
        {},  // old_deflates
        {},  // new_deflates
        (*graph)[cut.old_dst].aop.name,
        -1,  // chunk_blocks, forces to have a single operation.
        kInPlacePayloadVersion,
        blob_file));
    TEST_AND_RETURN_FALSE(new_aop.size() == 1);
    TEST_AND_RETURN_FALSE(AddInstallOpToGraph(
        graph, cut.old_dst, nullptr, new_aop.front().op, new_aop.front().name));

    (*graph)[cut.old_dst].out_edges = out_edges;

    // Right now we don't have doubly-linked edges, so we have to scan
    // the whole graph.
    graph_utils::DropIncomingEdgesTo(graph, cut.old_dst);
  }

  // Delete temp node
  (*graph)[cut.old_src].out_edges.erase(cut.new_vertex);
  CHECK((*graph)[cut.old_dst].out_edges.find(cut.new_vertex) ==
        (*graph)[cut.old_dst].out_edges.end());
  (*graph)[cut.new_vertex].valid = false;
  LOG(INFO) << "marked node invalid: " << cut.new_vertex;
  return true;
}

bool InplaceGenerator::ConvertGraphToDag(Graph* graph,
                                         const string& new_part,
                                         BlobFileWriter* blob_file,
                                         vector<Vertex::Index>* final_order,
                                         Vertex::Index scratch_vertex) {
  CycleBreaker cycle_breaker;
  LOG(INFO) << "Finding cycles...";
  set<Edge> cut_edges;
  cycle_breaker.BreakCycles(*graph, &cut_edges);
  LOG(INFO) << "done finding cycles";
  CheckGraph(*graph);

  // Calculate number of scratch blocks needed

  LOG(INFO) << "Cutting cycles...";
  vector<CutEdgeVertexes> cuts;
  TEST_AND_RETURN_FALSE(CutEdges(graph, cut_edges, &cuts));
  LOG(INFO) << "done cutting cycles";
  LOG(INFO) << "There are " << cuts.size() << " cuts.";
  CheckGraph(*graph);

  LOG(INFO) << "Creating initial topological order...";
  TopologicalSort(*graph, final_order);
  LOG(INFO) << "done with initial topo order";
  CheckGraph(*graph);

  LOG(INFO) << "Moving full ops to the back";
  MoveAndSortFullOpsToBack(graph, final_order);
  LOG(INFO) << "done moving full ops to back";

  vector<vector<Vertex::Index>::size_type> inverse_final_order;
  GenerateReverseTopoOrderMap(*final_order, &inverse_final_order);

  SortCutsByTopoOrder(*final_order, &cuts);

  if (!cuts.empty())
    TEST_AND_RETURN_FALSE(AssignTempBlocks(
        graph, new_part, blob_file, final_order, &inverse_final_order, cuts));
  LOG(INFO) << "Making sure all temp blocks have been allocated";

  // Remove the scratch node, if any
  if (scratch_vertex != Vertex::kInvalidIndex) {
    final_order->erase(final_order->begin() +
                       inverse_final_order[scratch_vertex]);
    (*graph)[scratch_vertex].valid = false;
    GenerateReverseTopoOrderMap(*final_order, &inverse_final_order);
  }

  graph_utils::DumpGraph(*graph);
  CHECK(NoTempBlocksRemain(*graph));
  LOG(INFO) << "done making sure all temp blocks are allocated";
  return true;
}

void InplaceGenerator::CreateScratchNode(uint64_t start_block,
                                         uint64_t num_blocks,
                                         Vertex* vertex) {
  vertex->aop.name = "<scratch>";
  vertex->aop.op.set_type(InstallOperation::REPLACE_BZ);
  vertex->aop.op.set_data_offset(0);
  vertex->aop.op.set_data_length(0);
  Extent* extent = vertex->aop.op.add_dst_extents();
  extent->set_start_block(start_block);
  extent->set_num_blocks(num_blocks);
}

bool InplaceGenerator::AddInstallOpToBlocksVector(
    const InstallOperation& operation,
    const Graph& graph,
    Vertex::Index vertex,
    vector<Block>* blocks) {
  // See if this is already present.
  TEST_AND_RETURN_FALSE(operation.dst_extents_size() > 0);

  enum BlockField { READER = 0, WRITER, BLOCK_FIELD_COUNT };
  for (int field = READER; field < BLOCK_FIELD_COUNT; field++) {
    const char* past_participle = (field == READER) ? "read" : "written";
    const google::protobuf::RepeatedPtrField<Extent>& extents =
        (field == READER) ? operation.src_extents() : operation.dst_extents();
    Vertex::Index Block::*access_type =
        (field == READER) ? &Block::reader : &Block::writer;

    for (const Extent& extent : extents) {
      for (uint64_t block = extent.start_block();
           block < (extent.start_block() + extent.num_blocks());
           block++) {
        if ((*blocks)[block].*access_type != Vertex::kInvalidIndex) {
          LOG(FATAL) << "Block " << block << " is already " << past_participle
                     << " by " << (*blocks)[block].*access_type << "("
                     << graph[(*blocks)[block].*access_type].aop.name
                     << ") and also " << vertex << "(" << graph[vertex].aop.name
                     << ")";
        }
        (*blocks)[block].*access_type = vertex;
      }
    }
  }
  return true;
}

bool InplaceGenerator::AddInstallOpToGraph(Graph* graph,
                                           Vertex::Index existing_vertex,
                                           vector<Block>* blocks,
                                           const InstallOperation& operation,
                                           const string& op_name) {
  Vertex::Index vertex = existing_vertex;
  if (vertex == Vertex::kInvalidIndex) {
    graph->emplace_back();
    vertex = graph->size() - 1;
  }
  (*graph)[vertex].aop.op = operation;
  CHECK((*graph)[vertex].aop.op.has_type());
  (*graph)[vertex].aop.name = op_name;

  if (blocks)
    TEST_AND_RETURN_FALSE(InplaceGenerator::AddInstallOpToBlocksVector(
        (*graph)[vertex].aop.op, *graph, vertex, blocks));
  return true;
}

void InplaceGenerator::ApplyMap(vector<uint64_t>* collection,
                                const map<uint64_t, uint64_t>& the_map) {
  for (uint64_t& elem : *collection) {
    const auto& map_it = the_map.find(elem);
    if (map_it != the_map.end())
      elem = map_it->second;
  }
}

bool InplaceGenerator::ResolveReadAfterWriteDependencies(
    const PartitionConfig& old_part,
    const PartitionConfig& new_part,
    uint64_t partition_size,
    size_t block_size,
    BlobFileWriter* blob_file,
    vector<AnnotatedOperation>* aops) {
  // Convert the operations to the graph.
  Graph graph;
  CheckGraph(graph);
  vector<Block> blocks(std::max(old_part.size, new_part.size) / block_size);
  for (const auto& aop : *aops) {
    AddInstallOpToGraph(
        &graph, Vertex::kInvalidIndex, &blocks, aop.op, aop.name);
  }
  CheckGraph(graph);

  // Final scratch block (if there's space)
  Vertex::Index scratch_vertex = Vertex::kInvalidIndex;
  if (blocks.size() < (partition_size / block_size)) {
    scratch_vertex = graph.size();
    graph.emplace_back();
    size_t scratch_blocks = (partition_size / block_size) - blocks.size();
    LOG(INFO) << "Added " << scratch_blocks << " scratch space blocks.";
    CreateScratchNode(blocks.size(), scratch_blocks, &graph.back());
  }
  CheckGraph(graph);

  LOG(INFO) << "Creating edges...";
  CreateEdges(&graph, blocks);
  LOG(INFO) << "Done creating edges";
  CheckGraph(graph);

  vector<Vertex::Index> final_order;
  TEST_AND_RETURN_FALSE(ConvertGraphToDag(
      &graph, new_part.path, blob_file, &final_order, scratch_vertex));

  // Copy operations over to the |aops| vector in the final_order generated by
  // the topological sort.
  aops->clear();
  aops->reserve(final_order.size());
  for (const Vertex::Index vertex_index : final_order) {
    const Vertex& vertex = graph[vertex_index];
    aops->push_back(vertex.aop);
  }
  return true;
}

bool InplaceGenerator::GenerateOperations(const PayloadGenerationConfig& config,
                                          const PartitionConfig& old_part,
                                          const PartitionConfig& new_part,
                                          BlobFileWriter* blob_file,
                                          vector<AnnotatedOperation>* aops) {
  TEST_AND_RETURN_FALSE(old_part.name == new_part.name);
  TEST_AND_RETURN_FALSE(config.version.major == kInPlacePayloadVersion.major);
  TEST_AND_RETURN_FALSE(config.version.minor == kInPlacePayloadVersion.minor);

  ssize_t hard_chunk_blocks =
      (config.hard_chunk_size == -1
           ? -1
           : config.hard_chunk_size / config.block_size);
  size_t soft_chunk_blocks = config.soft_chunk_size / config.block_size;
  uint64_t partition_size = new_part.size;
  if (new_part.name == kPartitionNameRoot)
    partition_size = config.rootfs_partition_size;

  LOG(INFO) << "Delta compressing " << new_part.name << " partition...";
  TEST_AND_RETURN_FALSE(diff_utils::DeltaReadPartition(aops,
                                                       old_part,
                                                       new_part,
                                                       hard_chunk_blocks,
                                                       soft_chunk_blocks,
                                                       config.version,
                                                       blob_file));
  LOG(INFO) << "Done reading " << new_part.name;

  TEST_AND_RETURN_FALSE(ResolveReadAfterWriteDependencies(
      old_part, new_part, partition_size, config.block_size, blob_file, aops));
  LOG(INFO) << "Done reordering " << new_part.name;
  return true;
}

};  // namespace chromeos_update_engine
