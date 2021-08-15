# Copyright 2018, The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

i0 = Input("i0", ("TENSOR_FLOAT32", [2, 2]))  # input 0

o1 = Output("o1", ("TENSOR_FLOAT32", [1, 2])) # output for model1
o2 = Output("o2", ("TENSOR_FLOAT32", [2, 1])) # output for model2
o3 = Output("o3", ("TENSOR_FLOAT32", [1]))    # output for model3

model1 = Model().Operation("MEAN", i0, [0], 1).To(o1)       # along axis 0, keep_dim=True
model2 = Model().Operation("MEAN", i0, [1], 1).To(o2)       # along axis 1, keep_dim=True
model3 = Model().Operation("MEAN", i0, [0, 1], 0).To(o3)    # along both axis, keep_dim=False

inputs1 = {i0: [1, 2, 3, 4]}
outputs11 = {o1: [4, 6]}
outputs12 = {o2: [3, 7]}
outputs13 = {o3: [10]}

inputs2 = {i0: [-1, -2, -3, -4]}
outputs21 = {o1: [-4, -6]}
outputs22 = {o2: [-3, -7]}
outputs23 = {o3: [-10]}

Example((inputs1, outputs11), model=model1)
Example((inputs1, outputs12), model=model2)
Example((inputs1, outputs13), model=model3)

Example((inputs2, outputs21), model=model1)
Example((inputs2, outputs22), model=model2)
Example((inputs2, outputs23), model=model3)
