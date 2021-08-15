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

#include <string>
#include <utility>
#include <vector>

#include <base/logging.h>
#include <base/macros.h>

#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/annotated_operation.h"
#include "update_engine/payload_generator/extent_utils.h"

using std::make_pair;
using std::pair;
using std::string;
using std::vector;

namespace chromeos_update_engine {
namespace graph_utils {

uint64_t EdgeWeight(const Graph& graph, const Edge& edge) {
  uint64_t weight = 0;
  const vector<Extent>& extents =
      graph[edge.first].out_edges.find(edge.second)->second.extents;
  for (vector<Extent>::const_iterator it = extents.begin(); it != extents.end();
       ++it) {
    if (it->start_block() != kSparseHole)
      weight += it->num_blocks();
  }
  return weight;
}

void AddReadBeforeDep(Vertex* src, Vertex::Index dst, uint64_t block) {
  Vertex::EdgeMap::iterator edge_it = src->out_edges.find(dst);
  if (edge_it == src->out_edges.end()) {
    // Must create new edge
    pair<Vertex::EdgeMap::iterator, bool> result =
        src->out_edges.insert(make_pair(dst, EdgeProperties()));
    CHECK(result.second);
    edge_it = result.first;
  }
  AppendBlockToExtents(&edge_it->second.extents, block);
}

void AddReadBeforeDepExtents(Vertex* src,
                             Vertex::Index dst,
                             const vector<Extent>& extents) {
  // TODO(adlr): Be more efficient than adding each block individually.
  for (vector<Extent>::const_iterator it = extents.begin(), e = extents.end();
       it != e;
       ++it) {
    const Extent& extent = *it;
    for (uint64_t block = extent.start_block(),
                  block_end = extent.start_block() + extent.num_blocks();
         block != block_end;
         ++block) {
      AddReadBeforeDep(src, dst, block);
    }
  }
}

void DropWriteBeforeDeps(Vertex::EdgeMap* edge_map) {
  // Specially crafted for-loop for the map-iterate-delete dance.
  for (Vertex::EdgeMap::iterator it = edge_map->begin();
       it != edge_map->end();) {
    if (!it->second.write_extents.empty())
      it->second.write_extents.clear();
    if (it->second.extents.empty()) {
      // Erase *it, as it contains no blocks
      edge_map->erase(it++);
    } else {
      ++it;
    }
  }
}

// For each node N in graph, drop all edges N->|index|.
void DropIncomingEdgesTo(Graph* graph, Vertex::Index index) {
  // This would be much more efficient if we had doubly-linked
  // edges in the graph.
  for (Graph::iterator it = graph->begin(), e = graph->end(); it != e; ++it) {
    it->out_edges.erase(index);
  }
}

namespace {
template <typename T>
void DumpExtents(const T& field, int prepend_space_count) {
  string header(prepend_space_count, ' ');
  for (const auto& extent : field) {
    LOG(INFO) << header << "(" << extent.start_block() << ", "
              << extent.num_blocks() << ")";
  }
}

void DumpOutEdges(const Vertex::EdgeMap& out_edges) {
  for (Vertex::EdgeMap::const_iterator it = out_edges.begin(),
                                       e = out_edges.end();
       it != e;
       ++it) {
    LOG(INFO) << "    " << it->first << " read-before:";
    DumpExtents(it->second.extents, 6);
    LOG(INFO) << "      write-before:";
    DumpExtents(it->second.write_extents, 6);
  }
}
}  // namespace

void DumpGraph(const Graph& graph) {
  LOG(INFO) << "Graph length: " << graph.size();
  for (Graph::size_type i = 0, e = graph.size(); i != e; ++i) {
    LOG(INFO) << i << (graph[i].valid ? "" : "-INV") << ": "
              << graph[i].aop.name << ": "
              << InstallOperationTypeName(graph[i].aop.op.type());
    LOG(INFO) << "  src_extents:";
    DumpExtents(graph[i].aop.op.src_extents(), 4);
    LOG(INFO) << "  dst_extents:";
    DumpExtents(graph[i].aop.op.dst_extents(), 4);
    LOG(INFO) << "  out edges:";
    DumpOutEdges(graph[i].out_edges);
  }
}

}  // namespace graph_utils
}  // namespace chromeos_update_engine
