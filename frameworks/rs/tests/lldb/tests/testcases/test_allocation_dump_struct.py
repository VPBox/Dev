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

'''Module that contains the test TestAllocationDumpStruct.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote


class TestAllocationDumpStruct(TestBaseRemote):
    '''Tests printing the contents of a struct allocation.'''

    bundle_target = {
        'java': 'Allocations'
    }

    def setup(self, android):
        '''This test requires to be run on one thread.'''
        android.push_prop('debug.rs.max-threads', 1)

    def teardown(self, android):
        '''Reset the number of RS threads to the previous value.'''
        android.pop_prop('debug.rs.max-threads')

    def test_dump_complex_struct_allocation(self):
        # Hit struct_kernel on last coordinate, so almost all elements have been initalised
        self.try_command(
            'language renderscript kernel breakpoint set struct_kernel -c 23',
            ['Conditional kernel breakpoint on coordinate (23, 0, 0)',
            'Breakpoint(s) created'])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        # complex_struct output allocation
        self.try_command('language renderscript allocation dump 49',
                         ['(0, 0, 0) = (complexStruct)  {\n'
                          '   (i = 0, j = 0)\n'
                          '   (0x00, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 0, [1] = 0.5)\n'
                          '}',
                          '(1, 0, 0) = (complexStruct)  {\n'
                          '   (i = 1, j = 1)\n'
                          '   (0x01, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 1, [1] = 1.5)\n'
                          '}',
                          '(2, 0, 0) = (complexStruct)  {\n'
                          '   (i = 2, j = 2)\n'
                          '   (0x02, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 2, [1] = 2.5)\n'
                          '}',
                          '(3, 0, 0) = (complexStruct)  {\n'
                          '   (i = 3, j = 3)\n'
                          '   (0x03, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 3, [1] = 3.5)\n'
                          '}',
                          '(4, 0, 0) = (complexStruct)  {\n'
                          '   (i = 4, j = 4)\n'
                          '   (0x04, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 4, [1] = 4.5)\n'
                          '}',
                          '(5, 0, 0) = (complexStruct)  {\n'
                          '   (i = 5, j = 5)\n'
                          '   (0x05, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 5, [1] = 5.5)\n'
                          '}',
                          '(6, 0, 0) = (complexStruct)  {\n'
                          '   (i = 6, j = 6)\n'
                          '   (0x06, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 6, [1] = 6.5)\n'
                          '}',
                          '(7, 0, 0) = (complexStruct)  {\n'
                          '   (i = 7, j = 7)\n'
                          '   (0x07, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 7, [1] = 7.5)\n'
                          '}',
                          '(8, 0, 0) = (complexStruct)  {\n'
                          '   (i = 8, j = 8)\n'
                          '   (0x08, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 8, [1] = 8.5)\n'
                          '}',
                          '(9, 0, 0) = (complexStruct)  {\n'
                          '   (i = 9, j = 9)\n'
                          '   (0x09, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 9, [1] = 9.5)\n'
                          '}',
                          '(10, 0, 0) = (complexStruct)  {\n'
                          '   (i = 10, j = 10)\n'
                          '   (0x0a, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 10, [1] = 10.5)\n'
                          '}',
                          '(11, 0, 0) = (complexStruct)  {\n'
                          '   (i = 11, j = 11)\n'
                          '   (0x0b, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 11, [1] = 11.5)\n'
                          '}',
                          '(12, 0, 0) = (complexStruct)  {\n'
                          '   (i = 12, j = 12)\n'
                          '   (0x0c, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 12, [1] = 12.5)\n'
                          '}',
                          '(13, 0, 0) = (complexStruct)  {\n'
                          '   (i = 13, j = 13)\n'
                          '   (0x0d, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 13, [1] = 13.5)\n'
                          '}',
                          '(14, 0, 0) = (complexStruct)  {\n'
                          '   (i = 14, j = 14)\n'
                          '   (0x0e, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 14, [1] = 14.5)\n'
                          '}',
                          '(15, 0, 0) = (complexStruct)  {\n'
                          '   (i = 15, j = 15)\n'
                          '   (0x0f, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 15, [1] = 15.5)\n'
                          '}',
                          '(16, 0, 0) = (complexStruct)  {\n'
                          '   (i = 16, j = 16)\n'
                          '   (0x10, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 16, [1] = 16.5)\n'
                          '}',
                          '(17, 0, 0) = (complexStruct)  {\n'
                          '   (i = 17, j = 17)\n'
                          '   (0x11, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 17, [1] = 17.5)\n'
                          '}',
                          '(18, 0, 0) = (complexStruct)  {\n'
                          '   (i = 18, j = 18)\n'
                          '   (0x12, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 18, [1] = 18.5)\n'
                          '}',
                          '(19, 0, 0) = (complexStruct)  {\n'
                          '   (i = 19, j = 19)\n'
                          '   (0x13, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 19, [1] = 19.5)\n'
                          '}',
                          '(20, 0, 0) = (complexStruct)  {\n'
                          '   (i = 20, j = 20)\n'
                          '   (0x14, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 20, [1] = 20.5)\n'
                          '}',
                          '(21, 0, 0) = (complexStruct)  {\n'
                          '   (i = 21, j = 21)\n'
                          '   (0x15, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 21, [1] = 21.5)\n'
                          '}',
                          '(22, 0, 0) = (complexStruct)  {\n'
                          '   (i = 22, j = 22)\n'
                          '   (0x16, 0x41, 0x42, 0x43)\n'
                          '   ([0] = 22, [1] = 22.5)\n'
                          '}'])
