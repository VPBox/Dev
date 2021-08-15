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

'''Module that contains the test TestWriteLocal.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    wimpy
)


class TestWriteLocal(TestBaseRemote):
    '''Tests modifying local variables of all types.'''

    bundle_target = {
        'java': 'KernelVariables',
        'jni': 'JNIKernelVariables',
        'cpp': 'CppKernelVariables'
    }

    def _try_modifying_local(self, local_name, new_value, data_type_in,
                             expected_output, expected_output_regex=None):
        '''Try getting lldb to modify a local and check the output.

        Run the "expr" command to set a given local to a new value and
        check that it is set afterwards by running the "target variable"
        command.

        Args:
            local_name: String which is the name of the local to modify.
            new_value: A string that is the new value of the local.
            data_type_in: A string containing a c-style parenthesised data type
                          representing the type of the local.
            expected_output: List of strings that should be found in the output
                             of both commands.
            expected_output_regex: List of regular expressions that should be
                                   found in the output of the target variable
                                   command.

        Raises:
            TestFail: One of the lldb commands did not provide the expected
                      output.
        '''
        # pylint: disable=too-many-arguments
        self.try_command('expr %s = %s%s'
                         % (local_name, data_type_in, new_value),
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
    def test_modify_char(self):
        self._try_modifying_local('char_local', '-2',
                                 '(signed char)', ['\'\\xfe\''],
                                 [r'\((signed )?char\)'])

    def test_modify_primitive(self):
        self._try_modifying_local('uchar_local', '22',
                                 '(uchar)', ['(uchar)', '\'\\x16\''])

        self._try_modifying_local('short_local', '-33',
                                 '(short)', ['(short)', '-33'])

        self._try_modifying_local('ushort_local', '44',
                                 '(ushort)', ['(ushort)', '44'])

        self._try_modifying_local('int_local', '-55',
                                 '(int)', ['(int)', '-55'])

        self._try_modifying_local('uint_local', '66',
                                 '(uint)', ['(uint)', '66'])

        self._try_modifying_local('float_local', '-7.5',
                                 '(float)', ['(float)', '-7.5'])

        self._try_modifying_local('long_local', '-888888',
                                 '(long long)', ['-888888'],
                                 [r'\((long )?long\)'])

        self._try_modifying_local('ulong_local', '99999999',
                                 '(ulong)', ['(ulong)', '99999999'])

        self._try_modifying_local('double_local', '-10101.5',
                                 '(double)', ['(double)', '-10101.5'])

        self._try_modifying_local('char2_local', '{22, 4}',
                                 '(char2)', ['(char2)', '(22, 4)'])

    @wimpy
    def test_modify_uchar2(self):
        self._try_modifying_local('uchar2_local', '{44, 55}',
                             '(uchar2)', ['(uchar2)', '(0x2c, 0x37)'])

    def test_modify_vec2(self):
        self._try_modifying_local('short2_local', '{-66, 77}',
                                 '(short2)', ['(short2)', '(-66, 77)'])

        self._try_modifying_local('ushort2_local', '{88, 99}',
                                 '(ushort2)', ['(ushort2)', '(88, 99)'])

        self._try_modifying_local('int2_local', '{111, -222}',
                                 '(int2)', ['(int2)', '(111, -222)'])

        self._try_modifying_local('uint2_local', '{333, 444}',
                                 '(uint2)', ['(uint2)', '(333, 444)'])

        self._try_modifying_local('float2_local', '{-55.5f, 6.0}',
                                 '(float2)', ['(float2)', '(-55.5, 6)'])

        self._try_modifying_local('long2_local', '{666666, -777777}',
                                 '(long2)', ['(long2)', '(666666, -777777)'])

        self._try_modifying_local('ulong2_local', '{888888, 999999}',
                                 '(ulong2)', ['(ulong2)', '(888888, 999999)'])

        self._try_modifying_local('double2_local', '{11.0000000, -0.0l}',
                                 '(double2)', ['(double2)', '(11, -0)'])

        self._try_modifying_local('char3_local', '{2, -3, 4}',
                                 '(char3)', ['(char3)', '(2, -3, 4,'])

        self._try_modifying_local('uchar3_local', '{\'a\', \'b\', \'c\'}',
                                 '(uchar3)', ['(uchar3)', '(0x61, 0x62, 0x63,'])

    @wimpy
    def test_modify_short3(self):
        self._try_modifying_local('short3_local', '{44, -55, 66}',
                             '(short3)', ['(short3)', '(44, -55, 66,'])

    def test_modify_vec3(self):
        self._try_modifying_local('ushort3_local', '{88, 99, 111}',
                                 '(ushort3)', ['(ushort3)', '(88, 99, 111,'])

        self._try_modifying_local('int3_local', '{-111, 222, -333}',
                                 '(int3)', ['(int3)', '(-111, 222, -333,'])

        self._try_modifying_local('uint3_local', '{444, 555, 666}',
                                 '(uint3)', ['(uint3)', '(444, 555, 666,'])

        self._try_modifying_local('float3_local', '{7.5F, 0008.000, 9}',
                                 '(float3)', ['(float3)', '(7.5, 8, 9,'])

        self._try_modifying_local('long3_local', '{111111, -22222222, 3333333}',
                                 '(long3)', ['(long3)', '(111111, -22222222, 3333333,'])

        self._try_modifying_local('ulong3_local', '{4444444, 5555555, 66666666}',
                                 '(ulong3)', ['(ulong3)', '(4444444, 5555555, 66666666,'])

        self._try_modifying_local('double3_local', '{7.5L, -0, 8.9e1}',
                                 '(double3)', ['(double3)', '(7.5, 0, 89,'])

        self._try_modifying_local('char4_local', '{0x1, 0x2, 0x3, 0x4}',
                                 '(char4)',
                                 ['(char4)', '(1, 2, 3, 4)'])

        self._try_modifying_local('uchar4_local', '{0x5, 0x6, 0x7, 0x8}',
                                 '(uchar4)',
                                 ['(uchar4)', '(0x05, 0x06, 0x07, 0x08)'])

        self._try_modifying_local('short4_local', '{0x9, 0xa, 0xb, 0xc}',
                                 '(short4)',
                                 ['(short4)', '(9, 10, 11, 12)'])

    @wimpy
    def test_modify_ushort4(self):
        self._try_modifying_local('ushort4_local', '{0xd, 0xe, 0xf, 0x10}',
                             '(ushort4)',
                             ['(ushort4)', '(13, 14, 15, 16)'])

    def test_modify_vec4(self):
        self._try_modifying_local('int4_local', '{0x11, 0x12, 0x13, 0x14}',
                                 '(int4)',
                                 ['(int4)', '(17, 18, 19, 20)'])

        self._try_modifying_local('uint4_local', '{0x15, 0x16, 0x17, 0x18}',
                                 '(uint4)',
                                 ['(uint4)', '(21, 22, 23, 24)'])

        self._try_modifying_local('float4_local', '{19.0, 20.5, -21, -22.5}',
                                 '(float4)',
                                 ['(float4)', '(19, 20.5, -21, -22.5)'])

        self._try_modifying_local('long4_local', '{0x1d, 0x1e, 0x1f, 0x20}',
                                 '(long4)',
                                 ['(long4)', '(29, 30, 31, 32)'])

        self._try_modifying_local('ulong4_local', '{0x21, 0x22, 0x23, 0x24}',
                                 '(ulong4)',
                                 ['(ulong4)', '(33, 34, 35, 36)'])

        self._try_modifying_local('double4_local', '{25.000, -26, -27.5, 28.0}',
                                 '(double4)',
                                 ['(double4)', '(25, -26, -27.5, 28)'])
