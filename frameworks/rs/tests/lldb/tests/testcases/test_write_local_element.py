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

'''Module that contains the test TestWriteLocalElement.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    wimpy,
    ordered_test
)


class TestWriteLocalElement(TestBaseRemote):
    '''Tests modifying elements of local variables of all types.'''

    bundle_target = {
        'java': 'KernelVariables',
        'jni': 'JNIKernelVariables',
        'cpp': 'CppKernelVariables'
    }

    def _try_inspecting_local(self, local_name, expected_output):
        '''Run the "expr" command on a given local and with a given output.

        Args:
            local_name: String which is the name of the local to inspect.
            expected_output: List of strings that should be found in the output.

        Raises:
            TestFail: The lldb command did not provide the expected output.
        '''
        self.try_command('expr ' + local_name, expected_output)

    def _try_modifying_local(self, local_name, new_value, expected_output,
                             expected_output_regex=None):
        '''Modify and then inspect a local and check for the output.

        Run the "expr" command to set a given local to a new value and
        check that it is set afterwards by running the "target variable"
        command.

        Args:
            local_name: String which is the name of the local to modify.
            new_value: A string that is the new value of the local.
            expected_output: List of strings that should be found in the output
                             of both commands.
            expected_output_regex: List of regular expressions that should be
                                   found in the output of the target variable
                                   command.

        Raises:
            TestFail: One of the lldb commands did not provide the expected
                      output.
        '''
        self.try_command('expr %s = %s' % (local_name, new_value),
                         expected_output,
                         expected_output_regex)
        self.try_command('frame variable ' + local_name,
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
    def test_modify_char2(self):
        self._try_modifying_local('char2_local[0]', '2',
                                 ['2'], [r'\((signed )?char\)'])
        self._try_inspecting_local('char2_local',
                                 ['(char2)', '(2, -22)'])

    def test_modify_vec2(self):
        self._try_modifying_local('uchar2_local[1]', '3',
                                 ['3'], [r'\(u(nsigned )?char\)'])
        self._try_inspecting_local('uchar2_local',
                                  ['(uchar2)', '(0x21, 0x03)'])

        self._try_modifying_local('short2_local[0]', '-44',
                                 ['(short)', '-44'])
        self._try_inspecting_local('short2_local',
                                  ['(short2)', '(-44, 666)'])

        self._try_modifying_local('ushort2_local[1]', '55',
                                 ['55'], [r'\(u(nsigned )?short\)'])
        self._try_inspecting_local('ushort2_local',
                                  ['(ushort2)', '(777, 55)'])

        self._try_modifying_local('int2_local[0]', '666',
                                 ['(int)', '666'])
        self._try_inspecting_local('int2_local',
                                  ['(int2)', '(666, -1111)'])

        self._try_modifying_local('uint2_local[1]', '777',
                                 ['777'], [r'\(u(nsigned )?int\)'])
        self._try_inspecting_local('uint2_local',
                                  ['(uint2)', '(2222, 777)'])

        self._try_modifying_local('float2_local[0]', '-8.5',
                                 ['(float)', '-8.5'])
        self._try_inspecting_local('float2_local',
                                  ['(float2)', '(-8.5, -5)'])

        self._try_modifying_local('long2_local[1]', '999999',
                                 ['999999'],
                                 [r'\((long )?long\)'])
        self._try_inspecting_local('long2_local',
                                  ['(long2)', '(-4444, 999999)'])

        self._try_modifying_local('ulong2_local[0]', '10101010101',
                                 ['10101010101'],
                                 [r'\(u(nsigned )?(long )?long\)'])
        self._try_inspecting_local('ulong2_local',
                                  ['(ulong2)', '(10101010101, 7777)'])

        self._try_modifying_local('double2_local[1]', '-11.000',
                                 ['(double)', '-11'])
        self._try_inspecting_local('double2_local',
                                  ['(double2)', '(88.5, -11)'])

        # For some reason the result of some char and uchar expr is in hex
        # and that of frame variable in decimal, so calling
        # try_modifying_local doesn't work, because it reuses the expected
        # output for both commands.
        self.try_command('expr char3_local[0] = 12',
                         ['\'\\f\''],
                         [r'\((signed )?char\)'])
        self.try_command('frame variable char3_local[0]',
                         ['12'],
                         [r'\((signed )?char\)'])

        self._try_inspecting_local('char3_local',
                                  ['(char3)',
                                   '(12, -22, -33,'])

    @wimpy
    def test_modify_uchar3(self):
        self.try_command('expr uchar3_local[1] = \'d\'',
                         ['\'d\''],
                         [r'\(u(nsigned )?char\)'])
        self.try_command('frame variable uchar3_local[1]',
                         ['0x64'],
                         [r'\(u(nsigned )?char\)'])


    def test_modify_vec3(self):
        self._try_inspecting_local('uchar3_local',
                                  ['(uchar3)',
                                   '(0x21, 0x64, 0x37,'])

        self._try_modifying_local('short3_local[2]', '-131',
                                 ['(short)', '-131'])
        self._try_inspecting_local('short3_local',
                                  ['(short3)',
                                   '(-555, 666, -131,'])

        self._try_modifying_local('ushort3_local[0]', '1414',
                                 ['1414'], [r'\(u(nsigned )?short\)'])
        self._try_inspecting_local('ushort3_local',
                                  ['(ushort3)',
                                   '(1414, 888, 999,'])

        self._try_modifying_local('int3_local[0]', '151515',
                                 ['(int)', '151515'])
        self._try_inspecting_local('int3_local',
                                  ['(int3)',
                                   '(151515, -1111, 2222,'])

        self._try_modifying_local('uint3_local[1]', '161616',
                                 ['161616'], [r'\(u(nsigned )?int\)'])
        self._try_inspecting_local('uint3_local',
                                  ['(uint3)',
                                   '(2222, 161616, 4444,'])

        self._try_modifying_local('float3_local[2]', '17.5',
                                 ['(float)', '17.5'])
        self._try_inspecting_local('float3_local',
                                  ['(float3)',
                                   '(4.5, -5, 17.5,'])

        self._try_modifying_local('long3_local[0]', '-181818181818',
                                 ['-181818181818'], [r'\((long )?long\)'])
        self._try_inspecting_local('long3_local',
                                  ['(long3)',
                                   '(-181818181818, 5555, 6666,'])

        self._try_modifying_local('ulong3_local[1]', '191919191919',
                                 ['191919191919'],
                                 [r'\(u(nsigned )?(long )?long\)'])
        self._try_inspecting_local('ulong3_local',
                                  ['(ulong3)',
                                   '(6666, 191919191919, 8888,'])

        self._try_modifying_local('double3_local[2]', '20.5',
                                 ['(double)', '20.5'])
        self._try_inspecting_local('double3_local',
                                  ['(double3)',
                                   '(88.5, -99, 20.5,'])

        self.try_command('expr char4_local[0] = -21',
                         ['\'\\xeb\''],
                         [r'\((signed )?char\)'])
        self.try_command('frame variable char4_local[0]',
                         ['-21'],
                         [r'\((signed )?char\)'])

        self._try_inspecting_local('char4_local',
                                  ['(char4)',
                                   '(-21, 11, -22, -33)'])

        self.try_command('expr uchar4_local[1] = 22',
                         ['\'\\x16\''],
                         [r'\(u(nsigned )?char\)'])
        self.try_command('frame variable uchar4_local[1]',
                         ['0x16'],
                         [r'\(u(nsigned )?char\)'])

        self._try_inspecting_local('uchar4_local',
                                  ['(uchar4)',
                                   '(0x16, 0x16, 0x2c, 0x37)'])

    @wimpy
    def test_modify_short4(self):
        self._try_modifying_local('short4_local[2]', '23',
                                 ['(short)', '23'])
        self._try_inspecting_local('short4_local',
                                  ['(short4)',
                                   '(-444, -555, 23, 777)'])

    def test_modify_vec4(self):
        self._try_modifying_local('ushort4_local[3]', '24',
                                 ['24'], [r'\(u(nsigned )?short\)'])
        self._try_inspecting_local('ushort4_local',
                                  ['(ushort4)',
                                   '(666, 777, 888, 24)'])

        self._try_modifying_local('int4_local[0]', '-2525',
                                 ['(int)', '-2525'])
        self._try_inspecting_local('int4_local',
                                  ['(int4)',
                                   '(-2525, 999, -1111, 2222)'])

        self._try_modifying_local('uint4_local[1]', '26262',
                                 ['26262'], [r'\(u(nsigned )?int\)'])
        self._try_inspecting_local('uint4_local',
                                  ['(uint4)',
                                   '(1111, 26262, 3333, 4444)'])

        self._try_modifying_local('float4_local[2]', '27.0f',
                                 ['(float)', '27'])
        self._try_inspecting_local('float4_local',
                                  ['(float4)',
                                   '(3, 4.5, 27, -6.5)'])

        self._try_modifying_local('long4_local[3]', '-28282828282',
                                 ['-28282828282'], [r'\((long )?long\)'])
        self._try_inspecting_local('long4_local',
                                  ['(long4)',
                                   '(-3333, -4444, 5555, -28282828282)'])

        self._try_modifying_local('ulong4_local[0]', '2929292929',
                                 ['2929292929'],
                                 [r'\(u(nsigned )?(long )?long\)'])
        self._try_inspecting_local('ulong4_local',
                                  ['(ulong4)',
                                   '(2929292929, 6666, 7777, 8888)'])

        self._try_modifying_local('double4_local[1]', '30.5',
                                 ['(double)', '30.5'])
        self._try_inspecting_local('double4_local',
                                  ['(double4)',
                                   '(-77, 30.5, -99, 111.5)'])
