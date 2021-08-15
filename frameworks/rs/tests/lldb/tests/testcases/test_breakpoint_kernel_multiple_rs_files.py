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

'''Module that contains the test TestBreakpointKernelMultipleRSFiles.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    cpp_only_test
)


class TestBreakpointKernelMultipleRSFiles(TestBaseRemote):
    '''Tests the setting of a breakpoint on RS kernels in multiple files.'''

    bundle_target = {
        'java': 'MultipleRSFiles',
        'jni': 'JNIMultipleRSFiles',
        'cpp': 'CppMultipleRSFiles'
    }

    def _binary_name(self):
        return {
             'java': 'multiplersfiles',
             'jni': 'multiplersfiles',
             'cpp': 'CppMultipleRSFi'
         }[self.app_type]

    def test_kernel_breakpoint_multiple_rs_files(self):
        # pylint: disable=line-too-long
        self.try_command('language renderscript kernel breakpoint set first_kernel',
                         ['Breakpoint(s) created',
                          '(pending)'])

        self.try_command('breakpoint list',
                         ["'first_kernel', locations = 0 (pending)"])

        self.try_command('process continue',
                         ['stopped',
                          'librs.first.so`first_kernel',
                          "name = '%s'" % self._binary_name(),
                          'stop reason = breakpoint 1'],
                          [r'at first\.rs:2[678]'])

        self.try_command('breakpoint list',
                         ["'first_kernel', locations = 1, resolved = 1"])

        self.try_command('language renderscript kernel breakpoint set second_kernel',
                         ['Breakpoint(s) created',
                          'Breakpoint 2',
                          'Breakpoint(s) created'],
                          [r"librs\.second\.so`second_kernel at second\.rs:2[012]",])

        self.try_command('breakpoint list',
                         ["'first_kernel', locations = 1, resolved = 1",
                          "'second_kernel', locations = 1, resolved = 1"])

        self.try_command('breakpoint delete 1',
                         ['1 breakpoints deleted'])

        self.try_command('breakpoint list',
                         ["'second_kernel', locations = 1, resolved = 1"])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint',
                          "librs.second.so`second_kernel"],
                          [r'second\.rs:2[012]'])

        self.try_command('breakpoint delete 2',
                         ['1 breakpoints deleted'])

        self.try_command('breakpoint list',
                         ['No breakpoints currently set'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('process continue', ['exited with status = 0'])
