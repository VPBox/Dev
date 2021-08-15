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

#include <algorithm>
#include <vector>

#include <base/logging.h>
#include <base/stl_util.h>

using std::min;
using std::vector;

namespace chromeos_update_engine {

namespace {
const vector<Vertex>::size_type kInvalidIndex = -1;
}

void TarjanAlgorithm::Execute(Vertex::Index vertex,
                              Graph* graph,
                              vector<Vertex::Index>* out) {
  stack_.clear();
  components_.clear();
  index_ = 0;
  for (Graph::iterator it = graph->begin(); it != graph->end(); ++it)
    it->index = it->lowlink = kInvalidIndex;
  required_vertex_ = vertex;

  Tarjan(vertex, graph);
  if (!components_.empty())
    out->swap(components_[0]);
}

void TarjanAlgorithm::Tarjan(Vertex::Index vertex, Graph* graph) {
  CHECK_EQ((*graph)[vertex].index, kInvalidIndex);
  (*graph)[vertex].index = index_;
  (*graph)[vertex].lowlink = index_;
  index_++;
  stack_.push_back(vertex);
  for (Vertex::EdgeMap::iterator it = (*graph)[vertex].out_edges.begin();
       it != (*graph)[vertex].out_edges.end();
       ++it) {
    Vertex::Index vertex_next = it->first;
    if ((*graph)[vertex_next].index == kInvalidIndex) {
      Tarjan(vertex_next, graph);
      (*graph)[vertex].lowlink =
          min((*graph)[vertex].lowlink, (*graph)[vertex_next].lowlink);
    } else if (base::ContainsValue(stack_, vertex_next)) {
      (*graph)[vertex].lowlink =
          min((*graph)[vertex].lowlink, (*graph)[vertex_next].index);
    }
  }
  if ((*graph)[vertex].lowlink == (*graph)[vertex].index) {
    vector<Vertex::Index> component;
    Vertex::Index other_vertex;
    do {
      other_vertex = stack_.back();
      stack_.pop_back();
      component.push_back(other_vertex);
    } while (other_vertex != vertex && !stack_.empty());

    if (base::ContainsValue(component, required_vertex_)) {
      components_.resize(components_.size() + 1);
      component.swap(components_.back());
    }
  }
}

}  // namespace chromeos_update_engine
