//
// Copyright (C) 2009 The Android Open Source Project
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

#include "update_engine/payload_generator/graph_utils.h"

#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/extent_utils.h"

using std::make_pair;
using std::vector;

namespace chromeos_update_engine {

class GraphUtilsTest : public ::testing::Test {};

TEST(GraphUtilsTest, SimpleTest) {
  Graph graph(2);

  graph[0].out_edges.insert(make_pair(1, EdgeProperties()));

  vector<Extent>& extents = graph[0].out_edges[1].extents;

  EXPECT_EQ(0U, extents.size());
  AppendBlockToExtents(&extents, 0);
  EXPECT_EQ(1U, extents.size());
  AppendBlockToExtents(&extents, 1);
  AppendBlockToExtents(&extents, 2);
  EXPECT_EQ(1U, extents.size());
  AppendBlockToExtents(&extents, 4);

  EXPECT_EQ(2U, extents.size());
  EXPECT_EQ(0U, extents[0].start_block());
  EXPECT_EQ(3U, extents[0].num_blocks());
  EXPECT_EQ(4U, extents[1].start_block());
  EXPECT_EQ(1U, extents[1].num_blocks());

  EXPECT_EQ(4U, graph_utils::EdgeWeight(graph, make_pair(0, 1)));
}

TEST(GraphUtilsTest, DepsTest) {
  Graph graph(3);

  graph_utils::AddReadBeforeDep(&graph[0], 1, 3);
  EXPECT_EQ(1U, graph[0].out_edges.size());
  {
    Extent& extent = graph[0].out_edges[1].extents[0];
    EXPECT_EQ(3U, extent.start_block());
    EXPECT_EQ(1U, extent.num_blocks());
  }
  graph_utils::AddReadBeforeDep(&graph[0], 1, 4);
  EXPECT_EQ(1U, graph[0].out_edges.size());
  {
    Extent& extent = graph[0].out_edges[1].extents[0];
    EXPECT_EQ(3U, extent.start_block());
    EXPECT_EQ(2U, extent.num_blocks());
  }
  graph_utils::AddReadBeforeDepExtents(
      &graph[2], 1, vector<Extent>(1, ExtentForRange(5, 2)));
  EXPECT_EQ(1U, graph[2].out_edges.size());
  {
    Extent& extent = graph[2].out_edges[1].extents[0];
    EXPECT_EQ(5U, extent.start_block());
    EXPECT_EQ(2U, extent.num_blocks());
  }
  // Change most recent edge from read-before to write-before
  graph[2].out_edges[1].write_extents.swap(graph[2].out_edges[1].extents);
  graph_utils::DropWriteBeforeDeps(&graph[2].out_edges);
  EXPECT_EQ(0U, graph[2].out_edges.size());

  EXPECT_EQ(1U, graph[0].out_edges.size());
  graph_utils::DropIncomingEdgesTo(&graph, 1);
  EXPECT_EQ(0U, graph[0].out_edges.size());
}

}  // namespace chromeos_update_engine
