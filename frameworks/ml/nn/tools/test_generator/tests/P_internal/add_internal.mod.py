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

model = Model()

i0 = Input("i0", ("TENSOR_FLOAT32", [2]))  # input 0
i1 = Input("i1", ("TENSOR_FLOAT32", [2]))  # input 0
i2 = Input("i2", ("TENSOR_FLOAT32", [2]))  # input 0
i3 = Input("i3", ("TENSOR_FLOAT32", [2]))  # input 0
i4 = Input("i4", ("TENSOR_FLOAT32", [2]))  # input 0
i5 = Input("i5", ("TENSOR_FLOAT32", [2]))  # input 0
i6 = Input("i6", ("TENSOR_FLOAT32", [2]))  # input 0
i7 = Input("i7", ("TENSOR_FLOAT32", [2]))  # input 0
i8 = Input("i8", ("TENSOR_FLOAT32", [2]))  # input 0

t0 = Internal("t0", ("TENSOR_FLOAT32", [2]))
t1 = Internal("t1", ("TENSOR_FLOAT32", [2]))
t2 = Internal("t2", ("TENSOR_FLOAT32", [2]))
t3 = Internal("t3", ("TENSOR_FLOAT32", [2]))
t4 = Internal("t4", ("TENSOR_FLOAT32", [2]))
t5 = Internal("t5", ("TENSOR_FLOAT32", [2]))
t6 = Internal("t6", ("TENSOR_FLOAT32", [2]))

o0 = Output("o0", ("TENSOR_FLOAT32", [2]))
o1 = Output("o1", ("TENSOR_FLOAT32", [2]))
o2 = Output("o2", ("TENSOR_FLOAT32", [2]))

p0 = Parameter("p0", ("TENSOR_FLOAT32", [2]), [0.0, 1.0])
act = Int32Scalar("act", 0)

model.Operation("ADD", o0, o1, act).To(o2)
model.Operation("ADD", p0, t5, act).To(t6)
model.Operation("ADD", i2, t0, act).To(t1)
model.Operation("ADD", i6, p0, act).To(t5)
model.Operation("ADD", i0, i1, act).To(t0)
model.Operation("ADD", t1, t3, act).To(t4)
model.Operation("ADD", t2, i5, act).To(t3)
model.Operation("ADD", t4, t6, act).To(o0)
model.Operation("ADD", i3, i4, act).To(t2)
model.Operation("ADD", i7, i8, act).To(o1)

inputs = {
    i0: [0, 0],
    i1: [0, 0],
    i2: [0, 0],
    i3: [0, 0],
    i4: [0, 0],
    i5: [0, 0],
    i6: [0, 0],
    i7: [0, 0],
    i8: [0, 0]
}

outputs = {
    o0: [0, 2],
    o1: [0, 0],
    o2: [0, 2]
}

Example((inputs, outputs))
