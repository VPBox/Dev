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

'''Module that contains the test TestBacktrace.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    cpp_only_test,
)


class TestBacktrace(TestBaseRemote):
    '''Tests breaking on a kernel and a function, and viewing the call stack.'''

    bundle_target = {
        'java': 'BranchingFunCalls',
        'jni': 'JNIBranchingFunCalls',
        'cpp': 'CppBranchingFunCalls'
    }

    def test_kernel_backtrace(self):
        # pylint: disable=line-too-long
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('language renderscript kernel breakpoint set simple_kernel',
                         ['Breakpoint(s) created',
                          '(pending)'])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        self.try_command('bt',
                         ['stop reason = breakpoint',
                          # We should be able to see three functions in bt:
                          # libRSCpuRef, kernel.expand and the kernel
                          'frame #2:',
                          'librs.scalars.so',
                          'simple_kernel'],
                         [r'scalars\.rs:6[123]'])

        self.try_command('breakpoint delete 1',
                         ['1 breakpoints deleted'])

        self.try_command('b set_i',
                         ['Breakpoint 2',
                          'set_i'],
                         [r'scalars\.rs:3[678]'])

        self.try_command('breakpoint list',
                         ['set_i', 'resolved'])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        self.try_command('bt',
                         ['stop reason = breakpoint',
                          # We should be able to see five functions in bt:
                          # libRSCpuRef, kernel.expand, kernel and two functions
                          'frame #4:',
                          'librs.scalars.so',
                          'modify_i',
                          'set_i'],
                         [r'scalars\.rs:3[678]'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 2',
                         ['1 breakpoints deleted'])

        self.try_command('process continue',
                         ['exited with status = 0'])
