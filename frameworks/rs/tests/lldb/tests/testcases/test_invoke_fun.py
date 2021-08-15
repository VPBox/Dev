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

'''Module that contains the test TestInvokeFun.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    cpp_only_test
)


class TestInvokeFun(TestBaseRemote):
    '''Tests debugging a function executed from Java using invoke_*.'''

    bundle_target = {
        'java': 'BranchingFunCalls',
        'jni': 'JNIBranchingFunCalls',
        'cpp': 'CppBranchingFunCalls'
    }

    def test_invoke_fun(self):
        # pylint: disable=line-too-long
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('breakpoint set --name addToGlobal',
                         ['Breakpoint 1', '(pending)'])

        self.try_command('process continue',
                         ['stopped',
                          'stop reason = breakpoint'],
                         [r'scalars\.rs:7[345]'])

        self.try_command('language renderscript kernel breakpoint set simple_kernel',
                         ['Breakpoint 2', 'Breakpoint(s) created'])

        self.try_command('process continue',
                         ['stopped',
                          'stop reason = breakpoint',
                          'simple_kernel'],
                         [r'scalars\.rs:6[123]'])

        self.try_command('expr glob',
                         ['(int)',
                          '357'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('breakpoint delete 2', ['1 breakpoints deleted'])

        self.try_command('process continue', ['exited with status = 0'])
