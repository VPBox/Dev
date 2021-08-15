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

#include <set>
#include <vector>

#include <base/logging.h>

using std::set;
using std::vector;

namespace chromeos_update_engine {

namespace {
void TopologicalSortVisit(const Graph& graph,
                          set<Vertex::Index>* visited_nodes,
                          vector<Vertex::Index>* nodes,
                          Vertex::Index node) {
  if (visited_nodes->find(node) != visited_nodes->end())
    return;

  visited_nodes->insert(node);
  // Visit all children.
  for (Vertex::EdgeMap::const_iterator it = graph[node].out_edges.begin();
       it != graph[node].out_edges.end();
       ++it) {
    TopologicalSortVisit(graph, visited_nodes, nodes, it->first);
  }
  // Visit this node.
  nodes->push_back(node);
}
}  // namespace

void TopologicalSort(const Graph& graph, vector<Vertex::Index>* out) {
  set<Vertex::Index> visited_nodes;

  for (Vertex::Index i = 0; i < graph.size(); i++) {
    TopologicalSortVisit(graph, &visited_nodes, out, i);
  }
}

}  // namespace chromeos_update_engine
