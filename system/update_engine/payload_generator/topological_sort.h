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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_TOPOLOGICAL_SORT_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_TOPOLOGICAL_SORT_H_

#include <vector>

#include "update_engine/payload_generator/graph_types.h"

namespace chromeos_update_engine {

// Performs a topological sort on the directed graph 'graph' and stores
// the nodes, in order visited, in 'out'.
// For example, this graph:
// A ---> C ----.
//  \           v
//   `--> B --> D
// Might result in this in 'out':
// out[0] = D
// out[1] = B
// out[2] = C
// out[3] = A
// Note: results are undefined if there is a cycle in the graph.
void TopologicalSort(const Graph& graph, std::vector<Vertex::Index>* out);

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_TOPOLOGICAL_SORT_H_
