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

'''Module that contains the test TestBreakpointKernel1.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    cpp_only_test
)


class TestBreakpointKernel1(TestBaseRemote):
    '''Tests the setting of a breakpoint on a RS kernel.'''

    bundle_target = {
        'java': 'JavaDebugWaitAttach',
        'jni': 'JNIDebugWaitAttach',
        'cpp': 'CppDebugWaitAttach'
    }

    @ordered_test(0)
    def test_breakpoint_set_nonexistent_kernel(self):
        # pylint: disable=line-too-long
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('language renderscript kernel breakpoint set simple_kernel',
                         ['Breakpoint(s) created',
                          '(pending)'])

        # Try set a breakpoint on a kernel which doesn't exist
        self.try_command('language renderscript kernel breakpoint set imaginary_kernel',
                         ['Breakpoint(s) created',
                          '(pending)'])

        self.try_command('breakpoint list',
                         ["'simple_kernel', locations = 0 (pending)",
                          "'imaginary_kernel', locations = 0 (pending)"])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        self.try_command('bt',
                         ['stop reason = breakpoint',
                          'frame #0:',
                          'librs.simple.so',
                          'simple_kernel'])

        self.try_command('breakpoint list',
                         ["'imaginary_kernel', locations = 0 (pending)",
                          "'simple_kernel', locations = 1, resolved = 1"])

    @ordered_test(1)
    def test_breakpoint_delete_nonexistent_kernel(self):
        # Delete breakpoint on kernel which doesn't exist
        self.try_command('breakpoint delete 2',
                         ['1 breakpoints deleted'])

        self.try_command('breakpoint list',
                         ["'simple_kernel', locations = 1, resolved = 1"])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        self.try_command('breakpoint list',
                         ["'simple_kernel', locations = 1, resolved = 1"])

        self.try_command('breakpoint delete 1',
                         ['1 breakpoints deleted'])

        self.try_command('breakpoint list',
                         ['No breakpoints currently set'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('process continue', ['exited with status = 0'])
