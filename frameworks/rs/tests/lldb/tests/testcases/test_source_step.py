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

'''Module that contains the test TestSourceStep.'''

from __future__ import absolute_import

import os
from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    cpp_only_test,
)


class TestSourceStep(TestBaseRemote):
    '''Test stepping through the source using step-in, -over and -out.'''

    bundle_target = {
        'java': 'BranchingFunCalls',
        'jni': 'JNIBranchingFunCalls',
        'cpp': 'CppBranchingFunCalls'

    }

    def script_dir(self):
        file_dir = os.path.dirname(os.path.realpath(__file__))
        app_root = os.path.join(file_dir, '..', '..')

        return {
            'java': os.path.join(app_root, 'java', 'BranchingFunCalls', 'src', 'rs'),
            'cpp': os.path.join(app_root, 'cpp', 'BranchingFunCalls'),
            'jni': os.path.join(app_root, 'jni', 'BranchingFunCalls', 'jnibranchingfuncalls')
        }[self.app_type]

    def setup(self, android):
        '''This test requires to be run on one thread.'''
        android.push_prop('debug.rs.max-threads', 1)

    def teardown(self, android):
        '''Reset the number of RS threads to the previous value.'''
        android.pop_prop('debug.rs.max-threads')

    def test_source_thread_step_in_out(self):
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('b -f scalars.rs -l 63',
                         ['(pending)'])

        self.try_command('process continue',
                         ['stopped',
                          'stop reason = breakpoint',
                          'scalars.rs:63'])

        # set the source mapping
        self.set_src_map('scalars.rs', self.script_dir())

        self.try_command('process status',
                         ['-> 63',
                          'int i = in;'])

        #63     int i = in;
        self.try_command('thread step-in',
                         ['-> 64'])
        #64     float f = (float) i;
        self.try_command('thread step-in',
                         ['-> 65'])
        #49     modify_f(&f);
        self.try_command('thread step-over',
                         ['-> 66'])
        #50  	modify_i(&i);
        self.try_command('thread step-in',
                         ['-> 49'])
        #49         int j = *i;
        self.try_command('b -f scalars.rs -l 54',
                         ['librs.scalars.so`modify_i',
                          'scalars.rs:54'])
        self.try_command('c',
                         ['stop reason = breakpoint',
                          'scalars.rs:54',
                          '-> 54'])
        #54    set_i(i, 0);
        # For the line number anything between #37 and #38 is fine
        self.try_command('thread step-in',
                         [],
                         [r'-> 3[678]'])
        #38    int tmp = b;
        self.try_command('thread step-out',
                         ['-> 54'])

    @cpp_only_test()
    @ordered_test('last')
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('breakpoint delete 2', ['1 breakpoints deleted'])

        self.try_command('process continue',
                         ['exited with status = 0'])

