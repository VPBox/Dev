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

'''Module that contains the test TestBreakpointKernelAllMultipleRSFiles.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    wimpy,
    ordered_test,
    cpp_only_test,
)


class TestBreakpointKernelAllMultipleRSFiles(TestBaseRemote):
    '''Tests setting breakpoints on every RS kernel in multiple kernel files.'''

    bundle_target = {
        'java': 'MultipleRSFiles',
        'jni': 'JNIMultipleRSFiles',
        'cpp': 'CppMultipleRSFiles'
    }

    @ordered_test(0)
    def test_deferred_breakpoint_resolution(self):
        # Test command works with no kernels currently loaded
        self.try_command('language renderscript kernel breakpoint all enable',
                         ['Breakpoints will be set on all kernels'])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        self.try_command('breakpoint list',
                         ["'first_kernel', locations = 1, resolved = 1",
                          "'second_kernel', locations = 1, resolved = 1"])

    @ordered_test(1)
    def test_disable_all_kernel_breakpoint_doesnt_delete_breakpoints(self):
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

    @ordered_test(2)
    def test_enable_breakpoint_on_loaded_kernels(self):
        # Test command works when kernels are loaded
        self.try_command('language renderscript kernel breakpoint all enable',
                         ['Breakpoints will be set on all kernels'])

        self.try_command('breakpoint list',
                         ["'first_kernel', locations = 1, resolved = 1",
                          "'second_kernel', locations = 1, resolved = 1"])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        self.try_command('breakpoint delete 3',
                         ['1 breakpoints deleted'])

        # Check other_kernel breakpoint gets hit
        self.try_command('breakpoint list',
                         ["'second_kernel', locations = 1, resolved = 1"])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 4', ['1 breakpoints deleted'])

        self.try_command('process continue', ['exited with status = 0'])

