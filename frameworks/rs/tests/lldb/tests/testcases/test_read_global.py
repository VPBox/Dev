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

'''Module that contains the test TestReadGlobal.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    wimpy,
    ordered_test,
    cpp_only_test,
)


class TestReadGlobal(TestBaseRemote):
    '''Tests inspecting global variables of all types.'''

    bundle_target = {
        'java': 'KernelVariables',
        'jni': 'JNIKernelVariables',
        'cpp': 'CppKernelVariables'
    }

    def _try_inspecting_global(self, global_name, expected_output,
                              expected_regex=None):
        '''Inspect a global and check for the output.

        Run the "expr" and "target variable" commands on a given global and
        with a given output. (The commands should be equivalent.)

        Args:
            global_name: String which is the name of the global to inspect.
            expected_output: List of strings that should be found in the output.
            expected_regex: List of regular expressions that should match lldb's
                            output.

        Raises:
            TestFail: One of the lldb commands did not provide the expected
                      output.
        '''
        self.try_command('expr ' + global_name,
                         expected_output,
                         expected_regex)

        self.try_command('target variable ' + global_name,
                         expected_output,
                         expected_regex)

    @wimpy
    @ordered_test(0)
    def test_setup(self):
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('b -f simple.rs -l 145', [])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

    @wimpy
    def test_list_script_globals(self):
        # pylint: disable=line-too-long

        self.try_command('target variable',
                         ['Global variables for',
                          'librs.simple.so',
                          "(uchar) uchar_global = '\\xea'",
                          '(short) short_global = -321',
                          '(ushort) ushort_global = 432',
                          '(int) int_global = 1234',
                          '(uint) uint_global = 2345',
                          '(float) float_global = 4.5',
                          '(ulong) ulong_global = 8888',
                          '(double) double_global = -456.5',
                          '(char2) char2_global = (11, -22)',
                          '(uchar2) uchar2_global = (0x21, 0x2c)',
                          '(short2) short2_global = (-555, 666)',
                          '(ushort2) ushort2_global = (777, 888)',
                          '(int2) int2_global = (999, -1111)',
                          '(uint2) uint2_global = (2222, 3333)',
                          '(float2) float2_global = (4.5, -5)',
                          '(long2) long2_global = (-4444, 5555)',
                          '(ulong2) ulong2_global = (6666, 7777)',
                          '(double2) double2_global = (88.5, -99)',
                          '(char3) char3_global = (11, -22, -33,',
                          '(uchar3) uchar3_global = (0x21, 0x2c, 0x37,',
                          '(short3) short3_global = (-555, 666, 777,',
                          '(ushort3) ushort3_global = (777, 888, 999,',
                          '(int3) int3_global = (999, -1111, 2222,',
                          '(uint3) uint3_global = (2222, 3333, 4444,',
                          '(float3) float3_global = (4.5, -5, -6.5,',
                          '(long3) long3_global = (-4444, 5555, 6666,',
                          '(ulong3) ulong3_global = (6666, 7777, 8888,',
                          '(double3) double3_global = (88.5, -99, 111.5,',
                          '(char4) char4_global = (55, 11, -22, -33)',
                          '(uchar4) uchar4_global = (0xde, 0x21, 0x2c, 0x37)',
                          '(short4) short4_global = (-444, -555, 666, 777)',
                          '(ushort4) ushort4_global = (666, 777, 888, 999)',
                          '(int4) int4_global = (888, 999, -1111, 2222)',
                          '(uint4) uint4_global = (1111, 2222, 3333, 4444)',
                          '(float4) float4_global = (3, 4.5, -5, -6.5)',
                          '(long4) long4_global = (-3333, -4444, 5555, 6666)',
                          '(ulong4) ulong4_global = (5555, 6666, 7777, 8888)',
                          '(double4) double4_global = (-77, 88.5, -99, 111.5)',
                          '(rs_matrix2x2) matrix2x2_global = (m = (1, 2.5, 3, 4.5))',
                          '(rs_matrix3x3) matrix3x3_global = {\n'
                          '  m = ([0] = 5, [1] = 6.5, [2] = 7, [3] = 8.5, [4] = 9, [5] = 1.5, [6] = 2, [7] = 3.5, [8] = 4)',
                          '(rs_matrix4x4) matrix4x4_global = {\n'
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
                          '(rs_quaternion) quaternion_global = (4.5, 5.5, 6, 3)'],
                         [r"\((signed )?char\) char_global = '\\f'",
                          r'\((long )?long\) long_global = -77777'])

    @wimpy
    def test_read_char_global(self):
        # Use expr to inspect locals
        self._try_inspecting_global('char_global',
                         ["'\\f'"],
                         [r'\((signed )?char\)'])

    def test_read_primitive_global(self):
        self._try_inspecting_global('uchar_global',
                         ['(uchar)', "'\\xea'"])

        self._try_inspecting_global('short_global',
                         ['(short)', '-321'])

        self._try_inspecting_global('ushort_global',
                         ['(ushort)', '432'])

        self._try_inspecting_global('int_global',
                         ['(int)', '1234'])

        self._try_inspecting_global('uint_global',
                         ['(uint)', '2345'])

        self._try_inspecting_global('float_global',
                         ['(float)', '4.5'])

        self._try_inspecting_global('long_global',
                         ['-77777'],
                         [r'\((long )?long\)'])

        self._try_inspecting_global('ulong_global',
                         ['(ulong)', '8888'])

        self._try_inspecting_global('double_global',
                         ['(double)', '-456.5'])

        self._try_inspecting_global('char2_global',
                                   ['(char2)', '(11, -22)'])

    @wimpy
    def test_write_global2(self):
        self._try_inspecting_global('uchar2_global',
                               ['(uchar2)', '(0x21, 0x2c)'])

    def test_write_global3(self):
        self._try_inspecting_global('short2_global',
                                   ['(short2)', '(-555, 666)'])

        self._try_inspecting_global('ushort2_global',
                                   ['(ushort2)', '(777, 888)'])

        self._try_inspecting_global('int2_global',
                                   ['(int2)', '(999, -1111)'])

        self._try_inspecting_global('uint2_global',
                                   ['(uint2)', '(2222, 3333)'])

        self._try_inspecting_global('float2_global',
                                   ['(float2)', '(4.5, -5)'])

        self._try_inspecting_global('long2_global',
                                   ['(long2)', '(-4444, 5555)'])

        self._try_inspecting_global('ulong2_global',
                                   ['(ulong2)', '(6666, 7777)'])

        self._try_inspecting_global('double2_global',
                                   ['(double2)', '(88.5, -99)'])

        self._try_inspecting_global('char3_global',
                                   ['(char3)',
                                    '(11, -22, -33,'])

        self._try_inspecting_global('uchar3_global',
                                   ['(uchar3)',
                                    '(0x21, 0x2c, 0x37,'])

    @wimpy
    def test_global_write_short3(self):
        self._try_inspecting_global('short3_global',
                                   ['(short3)',
                                   '(-555, 666, 777,'])

    def test_read_vec3(self):
        self._try_inspecting_global('ushort3_global',
                                   ['(ushort3)',
                                    '(777, 888, 999,'])

        self._try_inspecting_global('int3_global',
                                   ['(int3)',
                                    '(999, -1111, 2222,'])

        self._try_inspecting_global('uint3_global',
                                   ['(uint3)',
                                    '(2222, 3333, 4444,'])

        self._try_inspecting_global('float3_global',
                                   ['(float3)',
                                    '(4.5, -5, -6.5,'])

        self._try_inspecting_global('long3_global',
                                   ['(long3)',
                                    '(-4444, 5555, 6666,'])

        self._try_inspecting_global('ulong3_global',
                                   ['(ulong3)',
                                    '(6666, 7777, 8888,'])

        self._try_inspecting_global('double3_global',
                                   ['(double3)',
                                    '(88.5, -99, 111.5,'])

        self._try_inspecting_global('char4_global',
                                   ['(char4)',
                                    '(55, 11, -22, -33)'])

        self._try_inspecting_global('uchar4_global',
                                   ['(uchar4)',
                                    '(0xde, 0x21, 0x2c, 0x37)'])

        self._try_inspecting_global('short4_global',
                                   ['(short4)',
                                    '(-444, -555, 666, 777)'])

    @wimpy
    def test_read_ushort4(self):
        self._try_inspecting_global('ushort4_global',
                                   ['(ushort4)',
                                    '(666, 777, 888, 999)'])

    def test_read_vec4(self):
        self._try_inspecting_global('int4_global',
                                   ['(int4)',
                                    '(888, 999, -1111, 2222)'])

        self._try_inspecting_global('uint4_global',
                                   ['(uint4)',
                                    '(1111, 2222, 3333, 4444)'])

        self._try_inspecting_global('float4_global',
                                   ['(float4)',
                                    '(3, 4.5, -5, -6.5)'])

        self._try_inspecting_global('long4_global',
                                   ['(long4)',
                                    '(-3333, -4444, 5555, 6666)'])

        self._try_inspecting_global('ulong4_global',
                                   ['(ulong4)',
                                    '(5555, 6666, 7777, 8888)'])

        self._try_inspecting_global('double4_global',
                                   ['(double4)',
                                    '(-77, 88.5, -99, 111.5)'])

        self._try_inspecting_global('matrix2x2_global',
                                   ['(rs_matrix2x2)',
                                    '= (m = (1, 2.5, 3, 4.5))'])

        self._try_inspecting_global('matrix3x3_global',
                                       ['(rs_matrix3x3)',
                                        '= {\n'
                                        '  m = ([0] = 5, [1] = 6.5, [2] = 7, [3] = 8.5, [4] = 9, [5] = 1.5, [6] = 2, [7] = 3.5, [8] = 4)'])
    @wimpy
    def test_read_matrix(self):
        self._try_inspecting_global('matrix4x4_global',
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
    def test_read_quaternion(self):
        self._try_inspecting_global('quaternion_global',
                                   ['(rs_quaternion)',
                                    '(4.5, 5.5, 6, 3)'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('process continue', ['exited with status = 0'])
