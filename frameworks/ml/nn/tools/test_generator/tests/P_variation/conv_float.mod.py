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
i1 = Input("op1", "TENSOR_FLOAT32", "{1, 2, 2, 2}")
f1 = Parameter("op2", "TENSOR_FLOAT32", "{2, 2, 2, 2}", [1, 2, 3, 4, 5, 6, 7, 8, 8, 7, 6, 5, 4, 3, 2, 1])
b1 = Parameter("op3", "TENSOR_FLOAT32", "{1}", [-200])
output = Output("op4", "TENSOR_FLOAT32", "{1, 2, 2, 2}")
act = Int32Scalar("act", 0) # None activation
layout = Int32Scalar("layout", 0) # NHWC
pad = Int32Scalar("param", 1)
stride0 = Int32Scalar("param1", 1)
stride1 = Int32Scalar("param2", 1)

model = model.Operation("CONV_2D", i1, f1, b1, pad, stride0, stride1, act, layout).To(output)

# Example 1. Input in operand 0,
input0 = {i1: # input 0
          [1, 2, 3, 4, 5, 6, 7, 8]}

output0 = {output: # output 0
           [204, 120, 94, 104, 70, 164, 23, 112]}

quant8 = DataTypeConverter().Identify({
    i1: ("TENSOR_QUANT8_ASYMM", 0.5, 128),
    f1: ("TENSOR_QUANT8_ASYMM", 0.25, 128),
    b1: ("TENSOR_INT32", 0.125, 0),
    output: ("TENSOR_QUANT8_ASYMM", 2, 100)
})

# Instantiate an example
Example((input0, output0)).AddNchw(i1, f1, output, layout).AddAllActivations(
    output, act).AddInput(f1).AddVariations(RelaxedModeConverter(True), quant8)
