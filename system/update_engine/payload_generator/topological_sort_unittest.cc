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

#include "update_engine/payload_generator/topological_sort.h"

#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "update_engine/payload_generator/graph_types.h"

using std::make_pair;
using std::vector;

namespace chromeos_update_engine {

class TopologicalSortTest : public ::testing::Test {};

namespace {
// Returns true if the value is found in vect. If found, the index is stored
// in out_index if out_index is not null.
template <typename T>
bool IndexOf(const vector<T>& vect,
             const T& value,
             typename vector<T>::size_type* out_index) {
  for (typename vector<T>::size_type i = 0; i < vect.size(); i++) {
    if (vect[i] == value) {
      if (out_index) {
        *out_index = i;
      }
      return true;
    }
  }
  return false;
}
}  // namespace

TEST(TopologicalSortTest, SimpleTest) {
  int counter = 0;
  const Vertex::Index n_a = counter++;
  const Vertex::Index n_b = counter++;
  const Vertex::Index n_c = counter++;
  const Vertex::Index n_d = counter++;
  const Vertex::Index n_e = counter++;
  const Vertex::Index n_f = counter++;
  const Vertex::Index n_g = counter++;
  const Vertex::Index n_h = counter++;
  const Vertex::Index n_i = counter++;
  const Vertex::Index n_j = counter++;
  const Graph::size_type kNodeCount = counter++;

  Graph graph(kNodeCount);

  graph[n_i].out_edges.insert(make_pair(n_j, EdgeProperties()));
  graph[n_i].out_edges.insert(make_pair(n_c, EdgeProperties()));
  graph[n_i].out_edges.insert(make_pair(n_e, EdgeProperties()));
  graph[n_i].out_edges.insert(make_pair(n_h, EdgeProperties()));
  graph[n_c].out_edges.insert(make_pair(n_b, EdgeProperties()));
  graph[n_b].out_edges.insert(make_pair(n_a, EdgeProperties()));
  graph[n_e].out_edges.insert(make_pair(n_d, EdgeProperties()));
  graph[n_e].out_edges.insert(make_pair(n_g, EdgeProperties()));
  graph[n_g].out_edges.insert(make_pair(n_d, EdgeProperties()));
  graph[n_g].out_edges.insert(make_pair(n_f, EdgeProperties()));
  graph[n_d].out_edges.insert(make_pair(n_a, EdgeProperties()));

  vector<Vertex::Index> sorted;
  TopologicalSort(graph, &sorted);

  for (Vertex::Index i = 0; i < graph.size(); i++) {
    vector<Vertex::Index>::size_type src_index = 0;
    EXPECT_TRUE(IndexOf(sorted, i, &src_index));
    for (Vertex::EdgeMap::const_iterator it = graph[i].out_edges.begin();
         it != graph[i].out_edges.end();
         ++it) {
      vector<Vertex::Index>::size_type dst_index = 0;
      EXPECT_TRUE(IndexOf(sorted, it->first, &dst_index));
      EXPECT_LT(dst_index, src_index);
    }
  }
}

}  // namespace chromeos_update_engine
