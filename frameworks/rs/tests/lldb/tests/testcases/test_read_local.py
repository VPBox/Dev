# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

'''Module that contains the test TestReadLocal.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    wimpy,
    ordered_test,
    skip_conditional,
    cpp_only_test
)


class TestReadLocal(TestBaseRemote):
    '''Tests inspecting local variables of all types.'''

    bundle_target = {
        'java': 'KernelVariables',
        'jni': 'JNIKernelVariables',
        'cpp': 'CppKernelVariables'
    }

    def _try_inspecting_local(self, local_name, expected_output,
                             expected_regex=None):
        '''Inspect a local and check for the output.

        Run the "expr" and "frame variable" commands on a given local and
        with a given output. (The commands should be equivalent.)

        Args:
            local_name: String which is the name of the global to inspect.
            expected_output: List of strings that should be found in the output.
            expected_regex: List of regular expressions that should match lldb's
                            output.

        Raises:
            TestFail: One of the lldb commands did not provide the expected
                      output.
        '''
        self.try_command('expr ' + local_name,
                         expected_output,
                         expected_regex)

        self.try_command('frame variable ' + local_name,
                         expected_output,
                         expected_regex)

    @wimpy
    @ordered_test(0)
    def test_setup(self):
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('breakpoint set --file simple.rs --line 145', [])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

    @wimpy
    def test_list_rs_kernel_frame_variables(self):
        # pylint: disable=line-too-long

        self.try_command('frame variable',
                         ["(uchar) uchar_local = 'b'",
                          '(short) short_local = -321',
                          '(ushort) ushort_local = 432',
                          '(int) int_local = 1234',
                          '(uint) uint_local = 2345',
                          '(float) float_local = 4.5',
                          '(ulong) ulong_local = 8888',
                          '(double) double_local = -456.5',
                          '(char2) char2_local = (-11, -22)',
                          '(uchar2) uchar2_local = (0x21, 0x2c)',
                          '(short2) short2_local = (-555, 666)',
                          '(ushort2) ushort2_local = (777, 888)',
                          '(int2) int2_local = (999, -1111)',
                          '(uint2) uint2_local = (2222, 3333)',
                          '(float2) float2_local = (4.5, -5)',
                          '(long2) long2_local = (-4444, 5555)',
                          '(ulong2) ulong2_local = (6666, 7777)',
                          '(double2) double2_local = (88.5, -99)',
                          '(char3) char3_local = (11, -22, -33,',
                          '(uchar3) uchar3_local = (0x21, 0x2c, 0x37,',
                          '(short3) short3_local = (-555, 666, 777,',
                          '(ushort3) ushort3_local = (777, 888, 999,',
                          '(int3) int3_local = (999, -1111, 2222,',
                          '(uint3) uint3_local = (2222, 3333, 4444,',
                          '(float3) float3_local = (4.5, -5, -6.5,',
                          '(long3) long3_local = (-4444, 5555, 6666,',
                          '(ulong3) ulong3_local = (6666, 7777, 8888,',
                          '(double3) double3_local = (88.5, -99, 111.5,',
                          '(char4) char4_local = (55, 11, -22, -33)',
                          '(uchar4) uchar4_local = (0x16, 0x21, 0x2c, 0x37)',
                          '(short4) short4_local = (-444, -555, 666, 777)',
                          '(ushort4) ushort4_local = (666, 777, 888, 999)',
                          '(int4) int4_local = (888, 999, -1111, 2222)',
                          '(uint4) uint4_local = (1111, 2222, 3333, 4444)',
                          '(float4) float4_local = (3, 4.5, -5, -6.5)',
                          '(long4) long4_local = (-3333, -4444, 5555, 6666)',
                          '(ulong4) ulong4_local = (5555, 6666, 7777, 8888)',
                          '(double4) double4_local = (-77, 88.5, -99, 111.5)',
                          '(rs_matrix2x2) matrix2x2_local = (m = (1, 2.5, 3, 4.5))',
                          '(rs_matrix3x3) matrix3x3_local = {\n'
                          '  m = ([0] = 5, [1] = 6.5, [2] = 7, [3] = 8.5, [4] = 9, [5] = 1.5, [6] = 2, [7] = 3.5, [8] = 4)',
                          '(rs_matrix4x4) matrix4x4_local = {\n'
                          '  m = {\n'
                          '    [0] = 5.5\n'
                          '    [1] = 6\n'
                          '    [2] = 7.5\n'
                          '    [3] = 8\n'
                          '    [4] = 9\n'
                          '    [5] = 1.5\n'
                          '    [6] = 2\n'
                          '    [7] = 3.5\n'
                          '    [8] = 4.5\n'
                          '    [9] = 5.5\n'
                          '    [10] = 6.5\n'
                          '    [11] = 7\n'
                          '    [12] = 8\n'
                          '    [13] = 9.5\n'
                          '    [14] = 1.5\n'
                          '    [15] = 2.5\n'
                          '  }\n',
                          '(rs_quaternion) quaternion_local = (8, 9, 0.5, 7.5)'],
                         [r"\((signed )?char\) char_local = 'a'",
                          r'\((long )?long\) long_local = -77777'])


    @wimpy
    def test_inspect_primitive_types(self):
        # Use expr to inspect locals
        self._try_inspecting_local('char_local',
                                  ["'a'"],
                                  [r'\((signed )?char\)'])

        self._try_inspecting_local('uchar_local',
                                  ['(uchar)', "'b'"])

        self._try_inspecting_local('short_local',
                                  ['(short)', '-321'])

        self._try_inspecting_local('ushort_local',
                                  ['(ushort)', '432'])

        self._try_inspecting_local('int_local',
                                  ['(int)', '1234'])

        self._try_inspecting_local('uint_local',
                                  ['(uint)', '2345'])

        self._try_inspecting_local('float_local',
                                  ['(float)', '4.5'])

        self._try_inspecting_local('long_local',
                                  ['-77777'], [r'\((long )?long\)'])

        self._try_inspecting_local('ulong_local',
                                  ['(ulong)', '8888'])

        self._try_inspecting_local('double_local',
                                  ['(double)', '-456.5'])


    @wimpy
    def test_inspect_uchar2(self):
        self._try_inspecting_local('uchar2_local',
                                  ['(uchar2)', '(0x21, 0x2c)'])

    def test_inspect_vec2_types(self):
        self._try_inspecting_local('char2_local',
                                  ['(char2)', '(-11, -22)'])

        self._try_inspecting_local('short2_local',
                                  ['(short2)', '(-555, 666)'])

        self._try_inspecting_local('ushort2_local',
                                  ['(ushort2)', '(777, 888)'])

        self._try_inspecting_local('int2_local',
                                  ['(int2)', '(999, -1111)'])

        self._try_inspecting_local('uint2_local',
                                  ['(uint2)', '(2222, 3333)'])

        self._try_inspecting_local('float2_local',
                                  ['(float2)', '(4.5, -5)'])

        self._try_inspecting_local('long2_local',
                                  ['(long2)', '(-4444, 5555)'])

        self._try_inspecting_local('ulong2_local',
                                  ['(ulong2)', '(6666, 7777)'])

        self._try_inspecting_local('double2_local',
                                  ['(double2)', '(88.5, -99)'])

        self._try_inspecting_local('char3_local',
                                  ['(char3)',
                                   '(11, -22, -33,'])

        self._try_inspecting_local('uchar3_local',
                                  ['(uchar3)',
                                   '(0x21, 0x2c, 0x37,'])

    @wimpy
    def test_inspect_short3(self):
        self._try_inspecting_local('short3_local',
                                  ['(short3)',
                                   '(-555, 666, 777,'])

    def test_inspect_vec3_types(self):
        self._try_inspecting_local('ushort3_local',
                                  ['(ushort3)',
                                   '(777, 888, 999,'])

        self._try_inspecting_local('int3_local',
                                  ['(int3)',
                                   '(999, -1111, 2222,'])

        self._try_inspecting_local('uint3_local',
                                  ['(uint3)',
                                   '(2222, 3333, 4444,'])

        self._try_inspecting_local('float3_local',
                                  ['(float3)',
                                   '(4.5, -5, -6.5,'])

        self._try_inspecting_local('long3_local',
                                  ['(long3)',
                                   '(-4444, 5555, 6666,'])

        self._try_inspecting_local('ulong3_local',
                                  ['(ulong3)',
                                   '(6666, 7777, 8888,'])

        self._try_inspecting_local('double3_local',
                                  ['(double3)',
                                   '(88.5, -99, 111.5,'])

        self._try_inspecting_local('char4_local',
                                  ['(char4)',
                                   '(55, 11, -22, -33)'])

        self._try_inspecting_local('uchar4_local',
                                  ['(uchar4)',
                                   '(0x16, 0x21, 0x2c, 0x37)'])

        self._try_inspecting_local('short4_local',
                                  ['(short4)',
                                   '(-444, -555, 666, 777)'])

    @wimpy
    def test_inspect_ushort4(self):
        self._try_inspecting_local('ushort4_local',
                                  ['(ushort4)',
                                   '(666, 777, 888, 999)'])

    def test_inspect_vec4_types(self):
        self._try_inspecting_local('int4_local',
                                  ['(int4)',
                                   '(888, 999, -1111, 2222)'])

        self._try_inspecting_local('uint4_local',
                                  ['(uint4)',
                                   '(1111, 2222, 3333, 4444)'])

        self._try_inspecting_local('float4_local',
                                  ['(float4)',
                                   '(3, 4.5, -5, -6.5)'])

        self._try_inspecting_local('long4_local',
                                  ['(long4)',
                                   '(-3333, -4444, 5555, 6666)'])

        self._try_inspecting_local('ulong4_local',
                                  ['(ulong4)',
                                   '(5555, 6666, 7777, 8888)'])

        self._try_inspecting_local('double4_local',
                                  ['(double4)',
                                   '(-77, 88.5, -99, 111.5)'])
    def test_inspect_matrix_types(self):
        self._try_inspecting_local('matrix2x2_local',
                                   ['(rs_matrix2x2)',
                                    '= (m = (1, 2.5, 3, 4.5))'])

        self._try_inspecting_local('matrix3x3_local',
                                   ['(rs_matrix3x3)',
                                    '= {\n'
                                    '  m = ([0] = 5, [1] = 6.5, [2] = 7, [3] = 8.5, [4] = 9, [5] = 1.5, [6] = 2, [7] = 3.5, [8] = 4)'])

    @wimpy
    def test_inspect_matrix_4x4_local(self):
        self._try_inspecting_local('matrix4x4_local',
                                   ['(rs_matrix4x4)',
                                    '= {\n'
                                    '  m = {\n'
                                    '    [0] = 5.5\n'
                                    '    [1] = 6\n'
                                    '    [2] = 7.5\n'
                                    '    [3] = 8\n'
                                    '    [4] = 9\n'
                                    '    [5] = 1.5\n'
                                    '    [6] = 2\n'
                                    '    [7] = 3.5\n'
                                    '    [8] = 4.5\n'
                                    '    [9] = 5.5\n'
                                    '    [10] = 6.5\n'
                                    '    [11] = 7\n'
                                    '    [12] = 8\n'
                                    '    [13] = 9.5\n'
                                    '    [14] = 1.5\n'
                                    '    [15] = 2.5\n'
                                    '  }\n'])

    @wimpy
    def test_inspect_quaternion_local(self):
        self._try_inspecting_local('quaternion_local',
                                  ['(rs_quaternion)',
                                   '(8, 9, 0.5, 7.5)'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('process continue', ['exited with status = 0'])
