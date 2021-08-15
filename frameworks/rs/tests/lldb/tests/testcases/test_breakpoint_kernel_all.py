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

'''Module that contains the test TestBreakpointKernelAll.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    cpp_only_test,
)


class TestBreakpointKernelAll(TestBaseRemote):
    '''Tests setting breakpoints on every RS kernel.'''

    bundle_target = {
        'java': 'JavaDebugWaitAttach',
        'jni': 'JNIDebugWaitAttach',
        'cpp': 'CppDebugWaitAttach'
    }

    @ordered_test(0)
    def test_kernel_breakpoint_all_unloaded_kernels(self):
        # Test command works with no kernels currently loaded
        self.try_command('language renderscript kernel breakpoint all enable',
                         ['Breakpoints will be set on all kernels'])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        self.try_command('breakpoint list',
                         ["'simple_kernel', locations = 1, resolved = 1",
                          "'other_kernel', locations = 1, resolved = 1"])

        # Check disable doesn't delete breakpoints
        self.try_command('language renderscript kernel breakpoint all disable',
                         ['Breakpoints will not be set on any new kernels'])

        # Delete all breakpoints manually
        self.try_command('breakpoint delete 1',
                         ['1 breakpoints deleted'])

        self.try_command('breakpoint delete 2',
                         ['1 breakpoints deleted'])

        self.try_command('breakpoint list',
                         ["No breakpoints currently set"])

        # Test command works when kernels are loaded
        self.try_command('language renderscript kernel breakpoint all enable',
                         ['Breakpoints will be set on all kernels'])

        self.try_command('breakpoint list',
                         ["'simple_kernel', locations = 1, resolved = 1",
                          "'other_kernel', locations = 1, resolved = 1"])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        self.try_command('breakpoint delete 3',
                         ['1 breakpoints deleted'])

        # Check other_kernel breakpoint gets hit
        self.try_command('breakpoint list',
                         ["'other_kernel', locations = 1, resolved = 1"])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 4', ['1 breakpoints deleted'])

        self.try_command('process continue', ['exited with status = 0'])
