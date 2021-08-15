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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_GRAPH_UTILS_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_GRAPH_UTILS_H_

#include <vector>

#include <base/macros.h>

#include "update_engine/payload_generator/graph_types.h"
#include "update_engine/update_metadata.pb.h"

// A few utility functions for graphs

namespace chromeos_update_engine {

namespace graph_utils {

// Returns the number of blocks represented by all extents in the edge.
uint64_t EdgeWeight(const Graph& graph, const Edge& edge);

// These add a read-before dependency from graph[src] -> graph[dst]. If the dep
// already exists, the block/s is/are added to the existing edge.
void AddReadBeforeDep(Vertex* src, Vertex::Index dst, uint64_t block);
void AddReadBeforeDepExtents(Vertex* src,
                             Vertex::Index dst,
                             const std::vector<Extent>& extents);

void DropWriteBeforeDeps(Vertex::EdgeMap* edge_map);

// For each node N in graph, drop all edges N->|index|.
void DropIncomingEdgesTo(Graph* graph, Vertex::Index index);

void DumpGraph(const Graph& graph);

}  // namespace graph_utils

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_GRAPH_UTILS_H_
