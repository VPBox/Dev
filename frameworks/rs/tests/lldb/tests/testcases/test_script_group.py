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

'''Module that contains the test TestScriptGroup.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import wimpy


class TestScriptGroup(TestBaseRemote):
    bundle_target = {
        'java': 'ScriptGroup'
    }

    def setup(self, android):
        '''This test requires to be run on one thread.'''
        android.push_prop('debug.rs.max-threads', 1)

    def teardown(self, android):
        '''Reset the number of RS threads to the previous value.'''
        android.pop_prop('debug.rs.max-threads')

    @wimpy
    def test_kernel_backtrace(self):
        # number of allocation elements
        array_size = 8

        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered',
                          'rsdDebugHintScriptGroup2'])

        self.try_command('language renderscript scriptgroup breakpoint set scriptgroup_test',
                         ['Breakpoint 1: no locations (pending)'])

        self.try_command('language renderscript scriptgroup list',
                         ['0 script groups'])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint',
                          'librs.scriptgroup.so`foo',
                          'scriptgroup.rs'])

        self.try_command('breakpoint list',
                         ['scriptgroup_test',
                          'locations = 1'])

        self.try_command('language renderscript scriptgroup list',
                         ['1 script group',
                          'scriptgroup_test',
                          'foo',
                          'goo'])

        self.try_command('language renderscript scriptgroup breakpoint set --stop-on-all scriptgroup_test',
                         ['Breakpoint 2: 2 locations'])

        self.try_command('breakpoint list',
                         ['scriptgroup_test',
                          'librs.scriptgroup.so`foo',
                          'librs.scriptgroup.so`goo'])

        # iterate over foo kernels
        self.try_command('bt',
                         ['scriptgroup.rs:',
                          'frame #0', 'librs.scriptgroup.so`foo',
                          'frame #1', 'librs.scriptgroup.so`foo.expand'])

        for x in range(array_size):
            self.try_command('frame var',
                             ['(int) a = {0}'.format(x)])
            self.try_command('process continue',
                             ['resuming',
                              'stopped',
                              'stop reason = breakpoint',
                              'librs.scriptgroup.so`{0}'.format(
                                  'foo' if x < 7 else 'goo')])

        # iterate over goo kernels
        self.try_command('bt',
                         ['stop reason = breakpoint',
                          'scriptgroup.rs:',
                          'frame #0', 'librs.scriptgroup.so`goo',
                          'frame #1', 'librs.scriptgroup.so`goo.expand'])

        for x in range(array_size):
            self.try_command('frame var',
                             ['(int) a = {0}'.format(x * x)])

            if x < 7:
                self.try_command('process continue',
                                 ['resuming',
                                  'stopped',
                                  'stop reason = breakpoint',
                                  'librs.scriptgroup.so`goo'])
