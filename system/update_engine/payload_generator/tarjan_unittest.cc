//
// Copyright (C) 2010 The Android Open Source Project
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

#include "update_engine/payload_generator/tarjan.h"

#include <string>
#include <utility>

#include <base/logging.h>
#include <base/stl_util.h>
#include <gtest/gtest.h>

#include "update_engine/payload_generator/graph_types.h"

using std::make_pair;
using std::string;
using std::vector;

namespace chromeos_update_engine {

class TarjanAlgorithmTest : public ::testing::Test {};

TEST(TarjanAlgorithmTest, SimpleTest) {
  const Vertex::Index n_a = 0;
  const Vertex::Index n_b = 1;
  const Vertex::Index n_c = 2;
  const Vertex::Index n_d = 3;
  const Vertex::Index n_e = 4;
  const Vertex::Index n_f = 5;
  const Vertex::Index n_g = 6;
  const Vertex::Index n_h = 7;
  const Graph::size_type kNodeCount = 8;

  Graph graph(kNodeCount);

  graph[n_a].out_edges.insert(make_pair(n_e, EdgeProperties()));
  graph[n_a].out_edges.insert(make_pair(n_f, EdgeProperties()));
  graph[n_b].out_edges.insert(make_pair(n_a, EdgeProperties()));
  graph[n_c].out_edges.insert(make_pair(n_d, EdgeProperties()));
  graph[n_d].out_edges.insert(make_pair(n_e, EdgeProperties()));
  graph[n_d].out_edges.insert(make_pair(n_f, EdgeProperties()));
  graph[n_e].out_edges.insert(make_pair(n_b, EdgeProperties()));
  graph[n_e].out_edges.insert(make_pair(n_c, EdgeProperties()));
  graph[n_e].out_edges.insert(make_pair(n_f, EdgeProperties()));
  graph[n_f].out_edges.insert(make_pair(n_g, EdgeProperties()));
  graph[n_g].out_edges.insert(make_pair(n_h, EdgeProperties()));
  graph[n_h].out_edges.insert(make_pair(n_g, EdgeProperties()));

  TarjanAlgorithm tarjan;

  for (Vertex::Index i = n_a; i <= n_e; i++) {
    vector<Vertex::Index> vertex_indexes;
    tarjan.Execute(i, &graph, &vertex_indexes);

    EXPECT_EQ(5U, vertex_indexes.size());
    EXPECT_TRUE(base::ContainsValue(vertex_indexes, n_a));
    EXPECT_TRUE(base::ContainsValue(vertex_indexes, n_b));
    EXPECT_TRUE(base::ContainsValue(vertex_indexes, n_c));
    EXPECT_TRUE(base::ContainsValue(vertex_indexes, n_d));
    EXPECT_TRUE(base::ContainsValue(vertex_indexes, n_e));
  }

  {
    vector<Vertex::Index> vertex_indexes;
    tarjan.Execute(n_f, &graph, &vertex_indexes);

    EXPECT_EQ(1U, vertex_indexes.size());
    EXPECT_TRUE(base::ContainsValue(vertex_indexes, n_f));
  }

  for (Vertex::Index i = n_g; i <= n_h; i++) {
    vector<Vertex::Index> vertex_indexes;
    tarjan.Execute(i, &graph, &vertex_indexes);

    EXPECT_EQ(2U, vertex_indexes.size());
    EXPECT_TRUE(base::ContainsValue(vertex_indexes, n_g));
    EXPECT_TRUE(base::ContainsValue(vertex_indexes, n_h));
  }
}

}  // namespace chromeos_update_engine
