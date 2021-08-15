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

'''Module that contains the test TestBreakpointFileLine.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    cpp_only_test,
    ordered_test
)


class TestBreakpointFileLine(TestBaseRemote):
    '''Tests the setting of a breakpoint on a specific line of a RS file.'''

    bundle_target = {
        'java': 'JavaDebugWaitAttach',
        'jni': 'JNIDebugWaitAttach',
        'cpp': 'CppDebugWaitAttach'
    }

    @ordered_test(0)
    def test_breakpoint_fileline(self):
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('breakpoint set --file simple.rs --line 28',
                         ['(pending)'])

        self.try_command('process continue',
                         [])

        self.try_command('bt',
                         ['librs.simple.so',
                          'simple_kernel',
                          'stop reason = breakpoint'])

        self.try_command('breakpoint list',
                         ['simple.rs',
                          'resolved = 1'])

        self.try_command('process status',
                         ['stopped',
                          'stop reason = breakpoint'])

        self.try_command('breakpoint delete 1',
                         ['1 breakpoints deleted'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('process continue', ['exited with status = 0'])
