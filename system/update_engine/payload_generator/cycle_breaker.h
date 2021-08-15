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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_CYCLE_BREAKER_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_CYCLE_BREAKER_H_

// This is a modified implementation of Donald B. Johnson's algorithm for
// finding all elementary cycles (a.k.a. circuits) in a directed graph.
// See the paper "Finding All the Elementary Circuits of a Directed Graph"
// at http://dutta.csc.ncsu.edu/csc791_spring07/wrap/circuits_johnson.pdf
// for reference.

// Note: this version of the algorithm not only finds cycles, but breaks them.
// It uses a simple greedy algorithm for cutting: when a cycle is discovered,
// the edge with the least weight is cut. Longer term we may wish to do
// something more intelligent, since the goal is (ideally) to minimize the
// sum of the weights of all cut cycles. In practice, it's intractable
// to consider all cycles before cutting any; there are simply too many.
// In a sample graph representative of a typical workload, I found over
// 5 * 10^15 cycles.

#include <set>
#include <vector>

#include "update_engine/payload_generator/graph_types.h"

namespace chromeos_update_engine {

class CycleBreaker {
 public:
  CycleBreaker() : skipped_ops_(0) {}
  // out_cut_edges is replaced with the cut edges.
  void BreakCycles(const Graph& graph, std::set<Edge>* out_cut_edges);

  size_t skipped_ops() const { return skipped_ops_; }

 private:
  void HandleCircuit();
  void Unblock(Vertex::Index u);
  bool Circuit(Vertex::Index vertex, Vertex::Index depth);
  bool StackContainsCutEdge() const;

  std::vector<bool> blocked_;         // "blocked" in the paper
  Vertex::Index current_vertex_;      // "s" in the paper
  std::vector<Vertex::Index> stack_;  // the stack variable in the paper
  Graph subgraph_;                    // "A_K" in the paper
  Graph blocked_graph_;               // "B" in the paper

  std::set<Edge> cut_edges_;

  // Number of operations skipped b/c we know they don't have any
  // incoming edges.
  size_t skipped_ops_;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_CYCLE_BREAKER_H_
