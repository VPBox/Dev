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

#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <base/format_macros.h>
#include <base/logging.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_generator/cycle_breaker.h"
#include "update_engine/payload_generator/delta_diff_generator.h"
#include "update_engine/payload_generator/delta_diff_utils.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/graph_types.h"
#include "update_engine/payload_generator/graph_utils.h"

using std::map;
using std::set;
using std::string;
using std::stringstream;
using std::vector;

namespace chromeos_update_engine {

using Block = InplaceGenerator::Block;

namespace {

void GenVertex(Vertex* out,
               const vector<Extent>& src_extents,
               const vector<Extent>& dst_extents,
               const string& path,
               InstallOperation::Type type) {
  out->aop.op.set_type(type);
  out->aop.name = path;
  StoreExtents(src_extents, out->aop.op.mutable_src_extents());
  StoreExtents(dst_extents, out->aop.op.mutable_dst_extents());
}

vector<Extent> VectOfExt(uint64_t start_block, uint64_t num_blocks) {
  return vector<Extent>(1, ExtentForRange(start_block, num_blocks));
}

EdgeProperties EdgeWithReadDep(const vector<Extent>& extents) {
  EdgeProperties ret;
  ret.extents = extents;
  return ret;
}

EdgeProperties EdgeWithWriteDep(const vector<Extent>& extents) {
  EdgeProperties ret;
  ret.write_extents = extents;
  return ret;
}

template <typename T>
void DumpVect(const vector<T>& vect) {
  stringstream ss(stringstream::out);
  for (typename vector<T>::const_iterator it = vect.begin(), e = vect.end();
       it != e;
       ++it) {
    ss << *it << ", ";
  }
  LOG(INFO) << "{" << ss.str() << "}";
}

void AppendExtent(vector<Extent>* vect, uint64_t start, uint64_t length) {
  vect->resize(vect->size() + 1);
  vect->back().set_start_block(start);
  vect->back().set_num_blocks(length);
}

void OpAppendExtent(InstallOperation* op, uint64_t start, uint64_t length) {
  Extent* extent = op->add_src_extents();
  extent->set_start_block(start);
  extent->set_num_blocks(length);
}

}  // namespace

class InplaceGeneratorTest : public ::testing::Test {
 protected:
  // Initialize |blob_path_|, |blob_file_size_| and |blob_file_fd_| variables
  // with a new blob file. The file is closed and removed automatically when
  // the test finishes.
  void CreateBlobFile() {
    // blob_fd_closer_ takes a pointer to blob_fd_. Make sure we destroy a
    // previous instance before overriding blob_fd_.
    blob_fd_closer_.reset();
    EXPECT_TRUE(utils::MakeTempFile(
        "InplaceGenerator_blob_file.XXXXXX", &blob_path_, &blob_fd_));
    blob_path_unlinker_.reset(new ScopedPathUnlinker(blob_path_));
    blob_fd_closer_.reset(new ScopedFdCloser(&blob_fd_));
    blob_file_size_ = 0;
    EXPECT_GE(blob_fd_, 0);
    blob_file_.reset(new BlobFileWriter(blob_fd_, &blob_file_size_));
  }

  // Dump the list of operations |aops| in case of test failure.
  void DumpAopsOnFailure(const vector<AnnotatedOperation>& aops) {
    if (HasNonfatalFailure()) {
      LOG(INFO) << "Result operation list:";
      for (const auto& aop : aops) {
        LOG(INFO) << aop;
      }
    }
  }

  // Blob file name, file descriptor and file size used to store operation
  // blobs.
  string blob_path_;
  int blob_fd_{-1};
  off_t blob_file_size_{0};
  std::unique_ptr<BlobFileWriter> blob_file_;
  std::unique_ptr<ScopedPathUnlinker> blob_path_unlinker_;
  std::unique_ptr<ScopedFdCloser> blob_fd_closer_;
};

TEST_F(InplaceGeneratorTest, BlockDefaultValues) {
  // Tests that a Block is initialized with the default values as a
  // Vertex::kInvalidIndex. This is required by the delta generators.
  Block block;
  EXPECT_EQ(Vertex::kInvalidIndex, block.reader);
  EXPECT_EQ(Vertex::kInvalidIndex, block.writer);
}

TEST_F(InplaceGeneratorTest, SubstituteBlocksTest) {
  vector<Extent> remove_blocks;
  AppendExtent(&remove_blocks, 3, 3);
  AppendExtent(&remove_blocks, 7, 1);
  vector<Extent> replace_blocks;
  AppendExtent(&replace_blocks, 10, 2);
  AppendExtent(&replace_blocks, 13, 2);
  Vertex vertex;
  InstallOperation& op = vertex.aop.op;
  OpAppendExtent(&op, 4, 3);
  OpAppendExtent(&op, kSparseHole, 4);  // Sparse hole in file
  OpAppendExtent(&op, 3, 1);
  OpAppendExtent(&op, 7, 3);

  InplaceGenerator::SubstituteBlocks(&vertex, remove_blocks, replace_blocks);

  EXPECT_EQ(7, op.src_extents_size());
  EXPECT_EQ(11U, op.src_extents(0).start_block());
  EXPECT_EQ(1U, op.src_extents(0).num_blocks());
  EXPECT_EQ(13U, op.src_extents(1).start_block());
  EXPECT_EQ(1U, op.src_extents(1).num_blocks());
  EXPECT_EQ(6U, op.src_extents(2).start_block());
  EXPECT_EQ(1U, op.src_extents(2).num_blocks());
  EXPECT_EQ(kSparseHole, op.src_extents(3).start_block());
  EXPECT_EQ(4U, op.src_extents(3).num_blocks());
  EXPECT_EQ(10U, op.src_extents(4).start_block());
  EXPECT_EQ(1U, op.src_extents(4).num_blocks());
  EXPECT_EQ(14U, op.src_extents(5).start_block());
  EXPECT_EQ(1U, op.src_extents(5).num_blocks());
  EXPECT_EQ(8U, op.src_extents(6).start_block());
  EXPECT_EQ(2U, op.src_extents(6).num_blocks());
}

TEST_F(InplaceGeneratorTest, CutEdgesTest) {
  Graph graph;
  vector<Block> blocks(9);

  // Create nodes in graph
  {
    graph.resize(graph.size() + 1);
    graph.back().aop.op.set_type(InstallOperation::MOVE);
    // Reads from blocks 3, 5, 7
    vector<Extent> extents;
    AppendBlockToExtents(&extents, 3);
    AppendBlockToExtents(&extents, 5);
    AppendBlockToExtents(&extents, 7);
    StoreExtents(extents, graph.back().aop.op.mutable_src_extents());
    blocks[3].reader = graph.size() - 1;
    blocks[5].reader = graph.size() - 1;
    blocks[7].reader = graph.size() - 1;

    // Writes to blocks 1, 2, 4
    extents.clear();
    AppendBlockToExtents(&extents, 1);
    AppendBlockToExtents(&extents, 2);
    AppendBlockToExtents(&extents, 4);
    StoreExtents(extents, graph.back().aop.op.mutable_dst_extents());
    blocks[1].writer = graph.size() - 1;
    blocks[2].writer = graph.size() - 1;
    blocks[4].writer = graph.size() - 1;
  }
  {
    graph.resize(graph.size() + 1);
    graph.back().aop.op.set_type(InstallOperation::MOVE);
    // Reads from blocks 1, 2, 4
    vector<Extent> extents;
    AppendBlockToExtents(&extents, 1);
    AppendBlockToExtents(&extents, 2);
    AppendBlockToExtents(&extents, 4);
    StoreExtents(extents, graph.back().aop.op.mutable_src_extents());
    blocks[1].reader = graph.size() - 1;
    blocks[2].reader = graph.size() - 1;
    blocks[4].reader = graph.size() - 1;

    // Writes to blocks 3, 5, 6
    extents.clear();
    AppendBlockToExtents(&extents, 3);
    AppendBlockToExtents(&extents, 5);
    AppendBlockToExtents(&extents, 6);
    StoreExtents(extents, graph.back().aop.op.mutable_dst_extents());
    blocks[3].writer = graph.size() - 1;
    blocks[5].writer = graph.size() - 1;
    blocks[6].writer = graph.size() - 1;
  }

  // Create edges
  InplaceGenerator::CreateEdges(&graph, blocks);

  // Find cycles
  CycleBreaker cycle_breaker;
  set<Edge> cut_edges;
  cycle_breaker.BreakCycles(graph, &cut_edges);

  EXPECT_EQ(1U, cut_edges.size());
  EXPECT_TRUE(cut_edges.end() !=
              cut_edges.find(std::pair<Vertex::Index, Vertex::Index>(1, 0)));

  vector<CutEdgeVertexes> cuts;
  EXPECT_TRUE(InplaceGenerator::CutEdges(&graph, cut_edges, &cuts));

  EXPECT_EQ(3U, graph.size());

  // Check new node in graph:
  EXPECT_EQ(InstallOperation::MOVE, graph.back().aop.op.type());
  EXPECT_EQ(2, graph.back().aop.op.src_extents_size());
  EXPECT_EQ(1, graph.back().aop.op.dst_extents_size());
  EXPECT_EQ(kTempBlockStart, graph.back().aop.op.dst_extents(0).start_block());
  EXPECT_EQ(2U, graph.back().aop.op.dst_extents(0).num_blocks());
  EXPECT_TRUE(graph.back().out_edges.empty());

  // Check that old node reads from new blocks
  EXPECT_EQ(2, graph[0].aop.op.src_extents_size());
  EXPECT_EQ(kTempBlockStart, graph[0].aop.op.src_extents(0).start_block());
  EXPECT_EQ(2U, graph[0].aop.op.src_extents(0).num_blocks());
  EXPECT_EQ(7U, graph[0].aop.op.src_extents(1).start_block());
  EXPECT_EQ(1U, graph[0].aop.op.src_extents(1).num_blocks());

  // And that the old dst extents haven't changed
  EXPECT_EQ(2, graph[0].aop.op.dst_extents_size());
  EXPECT_EQ(1U, graph[0].aop.op.dst_extents(0).start_block());
  EXPECT_EQ(2U, graph[0].aop.op.dst_extents(0).num_blocks());
  EXPECT_EQ(4U, graph[0].aop.op.dst_extents(1).start_block());
  EXPECT_EQ(1U, graph[0].aop.op.dst_extents(1).num_blocks());

  // Ensure it only depends on the next node and the new temp node
  EXPECT_EQ(2U, graph[0].out_edges.size());
  EXPECT_TRUE(graph[0].out_edges.end() != graph[0].out_edges.find(1));
  EXPECT_TRUE(graph[0].out_edges.end() !=
              graph[0].out_edges.find(graph.size() - 1));

  // Check second node has unchanged extents
  EXPECT_EQ(2, graph[1].aop.op.src_extents_size());
  EXPECT_EQ(1U, graph[1].aop.op.src_extents(0).start_block());
  EXPECT_EQ(2U, graph[1].aop.op.src_extents(0).num_blocks());
  EXPECT_EQ(4U, graph[1].aop.op.src_extents(1).start_block());
  EXPECT_EQ(1U, graph[1].aop.op.src_extents(1).num_blocks());

  EXPECT_EQ(2, graph[1].aop.op.dst_extents_size());
  EXPECT_EQ(3U, graph[1].aop.op.dst_extents(0).start_block());
  EXPECT_EQ(1U, graph[1].aop.op.dst_extents(0).num_blocks());
  EXPECT_EQ(5U, graph[1].aop.op.dst_extents(1).start_block());
  EXPECT_EQ(2U, graph[1].aop.op.dst_extents(1).num_blocks());

  // Ensure it only depends on the next node
  EXPECT_EQ(1U, graph[1].out_edges.size());
  EXPECT_TRUE(graph[1].out_edges.end() != graph[1].out_edges.find(2));
}

TEST_F(InplaceGeneratorTest, AssignTempBlocksReuseTest) {
  Graph graph(9);

  const vector<Extent> empt;
  uint64_t tmp = kTempBlockStart;
  const string kFilename = "/foo";

  vector<CutEdgeVertexes> cuts;
  cuts.resize(3);

  // Simple broken loop:
  GenVertex(
      &graph[0], VectOfExt(0, 1), VectOfExt(1, 1), "", InstallOperation::MOVE);
  GenVertex(&graph[1],
            VectOfExt(tmp, 1),
            VectOfExt(0, 1),
            "",
            InstallOperation::MOVE);
  GenVertex(&graph[2],
            VectOfExt(1, 1),
            VectOfExt(tmp, 1),
            "",
            InstallOperation::MOVE);
  // Corresponding edges:
  graph[0].out_edges[2] = EdgeWithReadDep(VectOfExt(1, 1));
  graph[1].out_edges[2] = EdgeWithWriteDep(VectOfExt(tmp, 1));
  graph[1].out_edges[0] = EdgeWithReadDep(VectOfExt(0, 1));
  // Store the cut:
  cuts[0].old_dst = 1;
  cuts[0].old_src = 0;
  cuts[0].new_vertex = 2;
  cuts[0].tmp_extents = VectOfExt(tmp, 1);
  tmp++;

  // Slightly more complex pair of loops:
  GenVertex(
      &graph[3], VectOfExt(4, 2), VectOfExt(2, 2), "", InstallOperation::MOVE);
  GenVertex(
      &graph[4], VectOfExt(6, 1), VectOfExt(7, 1), "", InstallOperation::MOVE);
  GenVertex(&graph[5],
            VectOfExt(tmp, 3),
            VectOfExt(4, 3),
            kFilename,
            InstallOperation::MOVE);
  GenVertex(&graph[6],
            VectOfExt(2, 2),
            VectOfExt(tmp, 2),
            "",
            InstallOperation::MOVE);
  GenVertex(&graph[7],
            VectOfExt(7, 1),
            VectOfExt(tmp + 2, 1),
            "",
            InstallOperation::MOVE);
  // Corresponding edges:
  graph[3].out_edges[6] = EdgeWithReadDep(VectOfExt(2, 2));
  graph[4].out_edges[7] = EdgeWithReadDep(VectOfExt(7, 1));
  graph[5].out_edges[6] = EdgeWithWriteDep(VectOfExt(tmp, 2));
  graph[5].out_edges[7] = EdgeWithWriteDep(VectOfExt(tmp + 2, 1));
  graph[5].out_edges[3] = EdgeWithReadDep(VectOfExt(4, 2));
  graph[5].out_edges[4] = EdgeWithReadDep(VectOfExt(6, 1));
  // Store the cuts:
  cuts[1].old_dst = 5;
  cuts[1].old_src = 3;
  cuts[1].new_vertex = 6;
  cuts[1].tmp_extents = VectOfExt(tmp, 2);
  cuts[2].old_dst = 5;
  cuts[2].old_src = 4;
  cuts[2].new_vertex = 7;
  cuts[2].tmp_extents = VectOfExt(tmp + 2, 1);

  // Supplier of temp block:
  GenVertex(&graph[8], empt, VectOfExt(8, 1), "", InstallOperation::REPLACE);

  // Specify the final order:
  vector<Vertex::Index> op_indexes;
  op_indexes.push_back(2);
  op_indexes.push_back(0);
  op_indexes.push_back(1);
  op_indexes.push_back(6);
  op_indexes.push_back(3);
  op_indexes.push_back(7);
  op_indexes.push_back(4);
  op_indexes.push_back(5);
  op_indexes.push_back(8);

  vector<vector<Vertex::Index>::size_type> reverse_op_indexes;
  InplaceGenerator::GenerateReverseTopoOrderMap(op_indexes,
                                                &reverse_op_indexes);

  CreateBlobFile();
  EXPECT_TRUE(InplaceGenerator::AssignTempBlocks(&graph,
                                                 "/dev/zero",
                                                 blob_file_.get(),
                                                 &op_indexes,
                                                 &reverse_op_indexes,
                                                 cuts));
  EXPECT_FALSE(graph[6].valid);
  EXPECT_FALSE(graph[7].valid);
  EXPECT_EQ(1, graph[1].aop.op.src_extents_size());
  EXPECT_EQ(2U, graph[1].aop.op.src_extents(0).start_block());
  EXPECT_EQ(1U, graph[1].aop.op.src_extents(0).num_blocks());
  EXPECT_EQ(InstallOperation::REPLACE_BZ, graph[5].aop.op.type());
}

TEST_F(InplaceGeneratorTest, MoveAndSortFullOpsToBackTest) {
  Graph graph(4);
  graph[0].aop.name = "A";
  graph[0].aop.op.set_type(InstallOperation::REPLACE);
  graph[1].aop.name = "B";
  graph[1].aop.op.set_type(InstallOperation::BSDIFF);
  graph[2].aop.name = "C";
  graph[2].aop.op.set_type(InstallOperation::REPLACE_BZ);
  graph[3].aop.name = "D";
  graph[3].aop.op.set_type(InstallOperation::MOVE);

  vector<Vertex::Index> vect(graph.size());

  for (vector<Vertex::Index>::size_type i = 0; i < vect.size(); ++i) {
    vect[i] = i;
  }
  InplaceGenerator::MoveAndSortFullOpsToBack(&graph, &vect);
  EXPECT_EQ(vect.size(), graph.size());
  EXPECT_EQ(graph[vect[0]].aop.name, "B");
  EXPECT_EQ(graph[vect[1]].aop.name, "D");
  EXPECT_EQ(graph[vect[2]].aop.name, "A");
  EXPECT_EQ(graph[vect[3]].aop.name, "C");
}

TEST_F(InplaceGeneratorTest, AssignTempBlocksTest) {
  Graph graph(9);
  const vector<Extent> empt;  // empty
  const string kFilename = "/foo";

  // Some scratch space:
  GenVertex(&graph[0], empt, VectOfExt(200, 1), "", InstallOperation::REPLACE);
  GenVertex(&graph[1], empt, VectOfExt(210, 10), "", InstallOperation::REPLACE);
  GenVertex(&graph[2], empt, VectOfExt(220, 1), "", InstallOperation::REPLACE);

  // A cycle that requires 10 blocks to break:
  GenVertex(&graph[3],
            VectOfExt(10, 11),
            VectOfExt(0, 9),
            "",
            InstallOperation::BSDIFF);
  graph[3].out_edges[4] = EdgeWithReadDep(VectOfExt(0, 9));
  GenVertex(&graph[4],
            VectOfExt(0, 9),
            VectOfExt(10, 11),
            "",
            InstallOperation::BSDIFF);
  graph[4].out_edges[3] = EdgeWithReadDep(VectOfExt(10, 11));

  // A cycle that requires 9 blocks to break:
  GenVertex(&graph[5],
            VectOfExt(40, 11),
            VectOfExt(30, 10),
            "",
            InstallOperation::BSDIFF);
  graph[5].out_edges[6] = EdgeWithReadDep(VectOfExt(30, 10));
  GenVertex(&graph[6],
            VectOfExt(30, 10),
            VectOfExt(40, 11),
            "",
            InstallOperation::BSDIFF);
  graph[6].out_edges[5] = EdgeWithReadDep(VectOfExt(40, 11));

  // A cycle that requires 40 blocks to break (which is too many):
  GenVertex(&graph[7],
            VectOfExt(120, 50),
            VectOfExt(60, 40),
            "",
            InstallOperation::BSDIFF);
  graph[7].out_edges[8] = EdgeWithReadDep(VectOfExt(60, 40));
  GenVertex(&graph[8],
            VectOfExt(60, 40),
            VectOfExt(120, 50),
            kFilename,
            InstallOperation::BSDIFF);
  graph[8].out_edges[7] = EdgeWithReadDep(VectOfExt(120, 50));

  graph_utils::DumpGraph(graph);

  vector<Vertex::Index> final_order;

  CreateBlobFile();
  EXPECT_TRUE(InplaceGenerator::ConvertGraphToDag(&graph,
                                                  "/dev/zero",
                                                  blob_file_.get(),
                                                  &final_order,
                                                  Vertex::kInvalidIndex));

  Graph expected_graph(12);
  GenVertex(&expected_graph[0],
            empt,
            VectOfExt(200, 1),
            "",
            InstallOperation::REPLACE);
  GenVertex(&expected_graph[1],
            empt,
            VectOfExt(210, 10),
            "",
            InstallOperation::REPLACE);
  GenVertex(&expected_graph[2],
            empt,
            VectOfExt(220, 1),
            "",
            InstallOperation::REPLACE);
  GenVertex(&expected_graph[3],
            VectOfExt(10, 11),
            VectOfExt(0, 9),
            "",
            InstallOperation::BSDIFF);
  expected_graph[3].out_edges[9] = EdgeWithReadDep(VectOfExt(0, 9));
  GenVertex(&expected_graph[4],
            VectOfExt(60, 9),
            VectOfExt(10, 11),
            "",
            InstallOperation::BSDIFF);
  expected_graph[4].out_edges[3] = EdgeWithReadDep(VectOfExt(10, 11));
  expected_graph[4].out_edges[9] = EdgeWithWriteDep(VectOfExt(60, 9));
  GenVertex(&expected_graph[5],
            VectOfExt(40, 11),
            VectOfExt(30, 10),
            "",
            InstallOperation::BSDIFF);
  expected_graph[5].out_edges[10] = EdgeWithReadDep(VectOfExt(30, 10));

  GenVertex(&expected_graph[6],
            VectOfExt(60, 10),
            VectOfExt(40, 11),
            "",
            InstallOperation::BSDIFF);
  expected_graph[6].out_edges[5] = EdgeWithReadDep(VectOfExt(40, 11));
  expected_graph[6].out_edges[10] = EdgeWithWriteDep(VectOfExt(60, 10));

  GenVertex(&expected_graph[7],
            VectOfExt(120, 50),
            VectOfExt(60, 40),
            "",
            InstallOperation::BSDIFF);
  expected_graph[7].out_edges[6] = EdgeWithReadDep(VectOfExt(60, 10));

  GenVertex(&expected_graph[8],
            empt,
            VectOfExt(0, 50),
            "/foo",
            InstallOperation::REPLACE_BZ);
  expected_graph[8].out_edges[7] = EdgeWithReadDep(VectOfExt(120, 50));

  GenVertex(&expected_graph[9],
            VectOfExt(0, 9),
            VectOfExt(60, 9),
            "",
            InstallOperation::MOVE);

  GenVertex(&expected_graph[10],
            VectOfExt(30, 10),
            VectOfExt(60, 10),
            "",
            InstallOperation::MOVE);
  expected_graph[10].out_edges[4] = EdgeWithReadDep(VectOfExt(60, 9));

  EXPECT_EQ(12U, graph.size());
  EXPECT_FALSE(graph.back().valid);
  for (Graph::size_type i = 0; i < graph.size() - 1; i++) {
    EXPECT_TRUE(graph[i].out_edges == expected_graph[i].out_edges);
    if (i == 8) {
      // special case
    } else {
      // EXPECT_TRUE(graph[i] == expected_graph[i]) << "i = " << i;
    }
  }
}

TEST_F(InplaceGeneratorTest, CreateScratchNodeTest) {
  Vertex vertex;
  InplaceGenerator::CreateScratchNode(12, 34, &vertex);
  EXPECT_EQ(InstallOperation::REPLACE_BZ, vertex.aop.op.type());
  EXPECT_EQ(0U, vertex.aop.op.data_offset());
  EXPECT_EQ(0U, vertex.aop.op.data_length());
  EXPECT_EQ(1, vertex.aop.op.dst_extents_size());
  EXPECT_EQ(12U, vertex.aop.op.dst_extents(0).start_block());
  EXPECT_EQ(34U, vertex.aop.op.dst_extents(0).num_blocks());
}

TEST_F(InplaceGeneratorTest, ApplyMapTest) {
  vector<uint64_t> collection = {1, 2, 3, 4, 6};
  vector<uint64_t> expected_values = {1, 2, 5, 4, 8};
  map<uint64_t, uint64_t> value_map;
  value_map[3] = 5;
  value_map[6] = 8;
  value_map[5] = 10;

  InplaceGenerator::ApplyMap(&collection, value_map);
  EXPECT_EQ(expected_values, collection);
}

// We can't produce MOVE operations with a source or destination in the block 0.
// This test checks that the cycle breaker procedure doesn't produce such
// operations.
TEST_F(InplaceGeneratorTest, ResolveReadAfterWriteDependenciesAvoidMoveToZero) {
  size_t block_size = 4096;
  size_t num_blocks = 4;
  vector<AnnotatedOperation> aops;

  // Create a REPLACE_BZ for block 0, and a circular dependency among all other
  // blocks. This situation would prefer to issue a MOVE to scratch space and
  // the only available block is 0.
  aops.emplace_back();
  aops.back().name = base::StringPrintf("<bz-block-0>");
  aops.back().op.set_type(InstallOperation::REPLACE_BZ);
  StoreExtents({ExtentForRange(0, 1)}, aops.back().op.mutable_dst_extents());

  for (size_t i = 1; i < num_blocks; i++) {
    AnnotatedOperation aop;
    aop.name = base::StringPrintf("<op-%" PRIuS ">", i);
    aop.op.set_type(InstallOperation::BSDIFF);
    StoreExtents({ExtentForRange(1 + i % (num_blocks - 1), 1)},
                 aop.op.mutable_src_extents());
    StoreExtents({ExtentForRange(i, 1)}, aop.op.mutable_dst_extents());
    aops.push_back(aop);
  }

  PartitionConfig part("part");
  part.path = "/dev/zero";
  part.size = num_blocks * block_size;

  CreateBlobFile();

  // We ran two tests here. The first one without enough blocks for the scratch
  // space, forcing it to create a new full operation and the second case with
  // one extra block in the partition that can be used for the move operation.
  for (const auto part_blocks : vector<uint64_t>{num_blocks, num_blocks + 1}) {
    SCOPED_TRACE(
        base::StringPrintf("Using partition_blocks=%" PRIu64, part_blocks));
    vector<AnnotatedOperation> result_aops = aops;
    EXPECT_TRUE(InplaceGenerator::ResolveReadAfterWriteDependencies(
        part,
        part,
        part_blocks * block_size,
        block_size,
        blob_file_.get(),
        &result_aops));

    size_t full_ops = 0;
    for (const auto& aop : result_aops) {
      if (diff_utils::IsAReplaceOperation(aop.op.type()))
        full_ops++;

      if (aop.op.type() != InstallOperation::MOVE)
        continue;
      for (const Extent& extent : aop.op.src_extents()) {
        EXPECT_NE(0U, extent.start_block())
            << "On src extents for aop: " << aop;
      }
      for (const Extent& extent : aop.op.dst_extents()) {
        EXPECT_NE(0U, extent.start_block())
            << "On dst extents for aop: " << aop;
      }
    }

    // If there's extra space in the partition, it should not use a new full
    // operation for it.
    EXPECT_EQ(part_blocks == num_blocks ? 2U : 1U, full_ops);

    DumpAopsOnFailure(result_aops);
  }
}

// Test that we can shrink a filesystem and break cycles.
TEST_F(InplaceGeneratorTest, ResolveReadAfterWriteDependenciesShrinkData) {
  size_t block_size = 4096;
  size_t old_blocks = 10;
  size_t new_blocks = 8;
  vector<AnnotatedOperation> aops;

  // Create a loop using the blocks 1-6 and one other operation writing to the
  // block 7 from outside the new partition. The loop in the blocks 1-6 uses
  // two-block operations, so it needs two blocks of scratch space. It can't use
  // the block 0 as scratch space (see previous test) and it can't use the
  // blocks 7 or 8 due the last move operation.

  aops.emplace_back();
  aops.back().name = base::StringPrintf("<bz-block-0>");
  aops.back().op.set_type(InstallOperation::REPLACE_BZ);
  StoreExtents({ExtentForRange(0, 1)}, aops.back().op.mutable_dst_extents());

  const size_t num_ops = 3;
  for (size_t i = 0; i < num_ops; i++) {
    AnnotatedOperation aop;
    aop.name = base::StringPrintf("<op-%" PRIuS ">", i);
    aop.op.set_type(InstallOperation::BSDIFF);
    StoreExtents({ExtentForRange(1 + 2 * i, 2)}, aop.op.mutable_src_extents());
    StoreExtents({ExtentForRange(1 + 2 * ((i + 1) % num_ops), 2)},
                 aop.op.mutable_dst_extents());
    aops.push_back(aop);
  }

  {
    AnnotatedOperation aop;
    aop.name = "<op-shrink>";
    aop.op.set_type(InstallOperation::BSDIFF);
    StoreExtents({ExtentForRange(8, 1)}, aop.op.mutable_src_extents());
    StoreExtents({ExtentForRange(7, 1)}, aop.op.mutable_dst_extents());
    aops.push_back(aop);
  }

  PartitionConfig old_part("part");
  old_part.path = "/dev/zero";
  old_part.size = old_blocks * block_size;

  PartitionConfig new_part("part");
  new_part.path = "/dev/zero";
  new_part.size = new_blocks * block_size;

  CreateBlobFile();

  EXPECT_TRUE(InplaceGenerator::ResolveReadAfterWriteDependencies(
      old_part,
      new_part,
      (old_blocks + 2) * block_size,  // enough scratch space.
      block_size,
      blob_file_.get(),
      &aops));

  size_t full_ops = 0;
  for (const auto& aop : aops) {
    if (diff_utils::IsAReplaceOperation(aop.op.type()))
      full_ops++;
  }
  // There should be only one REPLACE* operation, the one we added for block 0.
  EXPECT_EQ(1U, full_ops);

  // There should be only one MOVE operation, the one used to break the loop
  // which should write to scratch space past the block 7 (the last block of the
  // new partition) which is being written later.
  size_t move_ops = 0;
  for (const auto& aop : aops) {
    if (aop.op.type() == InstallOperation::MOVE) {
      move_ops++;
      for (const Extent& extent : aop.op.dst_extents()) {
        EXPECT_LE(7U, extent.start_block())
            << "On dst extents for aop: " << aop;
      }
    }
  }
  EXPECT_EQ(1U, move_ops);

  DumpAopsOnFailure(aops);
}

}  // namespace chromeos_update_engine
