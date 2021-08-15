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

'''Module that contains the test TestBreakpointKernel2.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote


class TestBreakpointKernel2(TestBaseRemote):
    '''Tests the setting of a breakpoint on a RS kernel.'''

    bundle_target = {
        'java': 'JavaInfiniteLoop',
        'jni': 'JNIInfiniteLoop',
        'cpp': 'CppInfiniteLoop'
    }

    def test_breakpoint_resolution_simple_kernel(self):
        # pylint: disable=line-too-long
        self.try_command('language renderscript kernel breakpoint set simple_kernel',
                         ['Breakpoint(s) created'])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        self.try_command('bt',
                         ['stop reason = breakpoint',
                          'frame #0:',
                          'librs.infiniteloop.so',
                          'simple_kernel'],
                         [r'infiniteloop\.rs:4[34]'])

        self.try_command('breakpoint list',
                         ['simple_kernel',
                          'resolved = 1'])

        self.try_command('process status',
                         ['stopped',
                          '.so`simple_kernel',
                          'stop reason = breakpoint'])
