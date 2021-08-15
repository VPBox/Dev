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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_TARJAN_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_TARJAN_H_

// This is an implementation of Tarjan's algorithm which finds all
// Strongly Connected Components in a graph.

// Note: a true Tarjan algorithm would find all strongly connected components
// in the graph. This implementation will only find the strongly connected
// component containing the vertex passed in.

#include <vector>

#include "update_engine/payload_generator/graph_types.h"

namespace chromeos_update_engine {

class TarjanAlgorithm {
 public:
  TarjanAlgorithm() : index_(0), required_vertex_(0) {}

  // 'out' is set to the result if there is one, otherwise it's untouched.
  void Execute(Vertex::Index vertex,
               Graph* graph,
               std::vector<Vertex::Index>* out);

 private:
  void Tarjan(Vertex::Index vertex, Graph* graph);

  Vertex::Index index_;
  Vertex::Index required_vertex_;
  std::vector<Vertex::Index> stack_;
  std::vector<std::vector<Vertex::Index>> components_;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_TARJAN_H_
