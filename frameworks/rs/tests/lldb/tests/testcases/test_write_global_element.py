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

'''Module that contains the test TestWriteGlobalElement.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    wimpy,
    cpp_only_test
)


class TestWriteGlobalElement(TestBaseRemote):
    '''Tests modifying elements of global variables of all types.'''

    bundle_target = {
        'java': 'KernelVariables',
        'jni': 'JNIKernelVariables',
        'cpp': 'CppKernelVariables'
    }

    def _try_inspecting_global(self, global_name, expected_output):
        '''Run the "expr" command on a given global and with a given output.

        Args:
            global_name: String which is the name of the global to inspect.
            expected_output: List of strings that should be found in the output.

        Raises:
            TestFail: The lldb command did not provide the expected output.
        '''
        self.try_command('expr ' + global_name, expected_output)

    def _try_modifying_global(self, global_name, new_value, expected_output,
                             expected_output_regex=None):
        '''Modify and then inspect a global and check for the output.

        Run the "expr" command to set a given global to a new value and
        check that it is set afterwards by running the "target variable"
        command.

        Args:
            global_name: String which is the name of the global to modify.
            new_value: A string that is the new value of the global.
            expected_output: List of strings that should be found in the output
                             of both commands.
            expected_output_regex: List of regular expressions that should be
                                   found in the output of the target variable
                                   command.

        Raises:
            TestFail: One of the lldb commands did not provide the expected
                      output.
        '''
        self.try_command('expr %s = %s' % (global_name, new_value),
                         expected_output,
                         expected_output_regex)
        self.try_command('target variable ' + global_name,
                         expected_output,
                         expected_output_regex)

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
    def test_modify_global(self):
        self._try_modifying_global('char2_global[0]', '2',
                                  ['\'\\x02\''],
                                  [r'\((signed )?char\)'])
        self._try_inspecting_global('char2_global', ['(char2)', '(2, -22)'])

    def test_vec2(self):
        self._try_modifying_global('uchar2_global[1]', '3',
                                  ['\'\\x03\''],
                                  [r'\(u(nsigned )?char\)'])
        self._try_inspecting_global('uchar2_global',
                                   ['(uchar2)', '(0x21, 0x03)'])

        self._try_modifying_global('short2_global[0]', '-44',
                                  ['(short)', '-44'])
        self._try_inspecting_global('short2_global',
                                   ['(short2)', '(-44, 666)'])

        self._try_modifying_global('ushort2_global[1]', '55',
                                  ['55'],
                                  [r'\(u(nsigned )?short\)'])
        self._try_inspecting_global('ushort2_global',
                                   ['(ushort2)', '(777, 55)'])

        self._try_modifying_global('int2_global[0]', '666',
                                  ['(int)', '666'])
        self._try_inspecting_global('int2_global',
                                   ['(int2)', '(666, -1111)'])

        self._try_modifying_global('uint2_global[1]', '777',
                                  ['777'],
                                  [r'\(u(nsigned )?int\)'])
        self._try_inspecting_global('uint2_global',
                                   ['(uint2)', '(2222, 777)'])

        self._try_modifying_global('float2_global[0]', '-8.5',
                                  ['(float)', '-8.5'])
        self._try_inspecting_global('float2_global',
                                   ['(float2)', '(-8.5, -5)'])

        self._try_modifying_global('long2_global[1]', '999999',
                                  ['999999'],
                                  [r'\((long )?long\)'])
        self._try_inspecting_global('long2_global',
                                   ['(long2)', '(-4444, 999999)'])

        self._try_modifying_global('ulong2_global[0]', '10101010101',
                                  ['10101010101'],
                                  [r'\(u(nsigned )?(long )?long\)'])
        self._try_inspecting_global('ulong2_global',
                                   ['(ulong2)', '(10101010101, 7777)'])

        self._try_modifying_global('double2_global[1]', '-11.000',
                                  ['(double)', '-11'])
        self._try_inspecting_global('double2_global',
                                   ['(double2)', '(88.5, -11)'])

        self._try_modifying_global('char3_global[0]', '12',
                                  ['\'\\f\''],
                                  [r'\((signed )?char\)'])
        self._try_inspecting_global('char3_global',
                                   ['(char3)',
                                    '(12, -22, -33,'])

    @wimpy
    def test_uchar3(self):
        self._try_modifying_global('uchar3_global[1]', '\'d\'',
                                  ['\'d\''],
                                  [r'\(u(nsigned )?char\)'])
        self._try_inspecting_global('uchar3_global',
                                   ['(uchar3)',
                                    '(0x21, 0x64, 0x37,'])

    def test_vec3(self):
        self._try_modifying_global('short3_global[2]', '-131',
                                  ['(short)', '-131'])
        self._try_inspecting_global('short3_global',
                                   ['(short3)',
                                    '(-555, 666, -131,'])

        self._try_modifying_global('ushort3_global[0]', '1414',
                                  ['1414'],
                                  [r'\(u(nsigned )?short\)'])
        self._try_inspecting_global('ushort3_global',
                                   ['(ushort3)',
                                    '(1414, 888, 999,'])

        self._try_modifying_global('int3_global[0]', '151515',
                                  ['(int)', '151515'])
        self._try_inspecting_global('int3_global',
                                   ['(int3)',
                                    '(151515, -1111, 2222,'])

        self._try_modifying_global('uint3_global[1]', '161616',
                                  ['161616'],
                                  [r'\(u(nsigned )?int\)'])
        self._try_inspecting_global('uint3_global',
                                   ['(uint3)',
                                    '(2222, 161616, 4444,'])

        self._try_modifying_global('float3_global[2]', '17.5',
                                  ['(float)', '17.5'])
        self._try_inspecting_global('float3_global',
                                   ['(float3)',
                                    '(4.5, -5, 17.5,'])

        self._try_modifying_global('long3_global[0]', '-181818181818',
                                  ['-181818181818'],
                                  [r'\((long )?long\)'])
        self._try_inspecting_global('long3_global',
                                   ['(long3)',
                                    '(-181818181818, 5555, 6666,'])

        self._try_modifying_global('ulong3_global[1]', '191919191919',
                                  ['191919191919'],
                                  [r'\(u(nsigned )?(long )?long\)'])
        self._try_inspecting_global('ulong3_global',
                                   ['(ulong3)',
                                    '(6666, 191919191919, 8888,'])

        self._try_modifying_global('double3_global[2]', '20.5',
                                  ['(double)', '20.5'])
        self._try_inspecting_global('double3_global',
                                   ['(double3)',
                                    '(88.5, -99, 20.5,'])

        self._try_modifying_global('char4_global[0]', '-21',
                                  ['\'\\xeb\''],
                                  [r'\((signed )?char\)'])
        self._try_inspecting_global('char4_global',
                                   ['(char4)',
                                    '(-21, 11, -22, -33)'])

        self._try_modifying_global('uchar4_global[1]', '22',
                                  ['\'\\x16\''],
                                  [r'\(u(nsigned )?char\)'])
        self._try_inspecting_global('uchar4_global',
                                   ['(uchar4)',
                                    '(0xde, 0x16, 0x2c, 0x37)'])

    @wimpy
    def test_short4(self):
        self._try_modifying_global('short4_global[2]', '23',
                                  ['(short)', '23'])
        self._try_inspecting_global('short4_global',
                                   ['(short4)',
                                    '(-444, -555, 23, 777)'])

    def test_vec4(self):
        self._try_modifying_global('ushort4_global[3]', '24',
                                  ['24'],
                                  [r'\(u(nsigned )?short\)'])
        self._try_inspecting_global('ushort4_global',
                                   ['(ushort4)',
                                    '(666, 777, 888, 24)'])

        self._try_modifying_global('int4_global[0]', '-2525',
                                  ['(int)', '-2525'])
        self._try_inspecting_global('int4_global',
                                   ['(int4)',
                                    '(-2525, 999, -1111, 2222)'])

        self._try_modifying_global('uint4_global[1]', '26262',
                                  ['26262'],
                                  [r'\(u(nsigned )?int\)'])
        self._try_inspecting_global('uint4_global',
                                   ['(uint4)',
                                    '(1111, 26262, 3333, 4444)'])

        self._try_modifying_global('float4_global[2]', '27.0f',
                                  ['(float)', '27'])
        self._try_inspecting_global('float4_global',
                                   ['(float4)',
                                    '(3, 4.5, 27, -6.5)'])

        self._try_modifying_global('long4_global[3]', '-28282828282',
                                  ['-28282828282'],
                                  [r'\((long )?long\)'])
        self._try_inspecting_global('long4_global',
                                   ['(long4)',
                                    '(-3333, -4444, 5555, -28282828282)'])

        self._try_modifying_global('ulong4_global[0]', '2929292929',
                                  ['2929292929'],
                                  [r'\(u(nsigned )?(long )?long\)'])
        self._try_inspecting_global('ulong4_global',
                                   ['(ulong4)',
                                    '(2929292929, 6666, 7777, 8888)'])

        self._try_modifying_global('double4_global[1]', '30.5',
                                  ['(double)', '30.5'])
        self._try_inspecting_global('double4_global',
                                   ['(double4)',
                                    '(-77, 30.5, -99, 111.5)'])

    @cpp_only_test()
    @ordered_test('last')
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('process continue',
                         ['exited with status = 0'])
