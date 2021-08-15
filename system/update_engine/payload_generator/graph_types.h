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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_GRAPH_TYPES_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_GRAPH_TYPES_H_

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <base/macros.h>

#include "update_engine/payload_generator/annotated_operation.h"
#include "update_engine/payload_generator/extent_utils.h"
#include "update_engine/update_metadata.pb.h"

// A few classes that help in generating delta images use these types
// for the graph work.

namespace chromeos_update_engine {

struct EdgeProperties {
  // Read-before extents. I.e., blocks in |extents| must be read by the
  // node pointed to before the pointing node runs (presumably b/c it
  // overwrites these blocks).
  std::vector<Extent> extents;

  // Write before extents. I.e., blocks in |write_extents| must be written
  // by the node pointed to before the pointing node runs (presumably
  // b/c it reads the data written by the other node).
  std::vector<Extent> write_extents;

  bool operator==(const EdgeProperties& that) const {
    return extents == that.extents && write_extents == that.write_extents;
  }
};

struct Vertex {
  Vertex() : valid(true), index(-1), lowlink(-1) {}
  bool valid;

  typedef std::map<std::vector<Vertex>::size_type, EdgeProperties> EdgeMap;
  EdgeMap out_edges;

  // We sometimes wish to consider a subgraph of a graph. A subgraph would have
  // a subset of the vertices from the graph and a subset of the edges.
  // When considering this vertex within a subgraph, subgraph_edges stores
  // the out-edges.
  typedef std::set<std::vector<Vertex>::size_type> SubgraphEdgeMap;
  SubgraphEdgeMap subgraph_edges;

  // For Tarjan's algorithm:
  std::vector<Vertex>::size_type index;
  std::vector<Vertex>::size_type lowlink;

  // Other Vertex properties:
  AnnotatedOperation aop;

  typedef std::vector<Vertex>::size_type Index;
  static const Vertex::Index kInvalidIndex;
};

typedef std::vector<Vertex> Graph;

typedef std::pair<Vertex::Index, Vertex::Index> Edge;

const uint64_t kTempBlockStart = 1ULL << 60;
static_assert(kTempBlockStart != 0, "kTempBlockStart invalid");

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_GRAPH_TYPES_H_
