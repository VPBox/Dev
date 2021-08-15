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
from harness.decorators import (ordered_test, wimpy)
from harness.exception import TestSuiteException


class TestSingleSource(TestBaseRemote):
    '''Tests debugging a function executed from Java using invoke_*.'''

    bundle_target = {
        'java': "SingleSource"
    }

    def setup(self, android):

        '''This test requires to be run on one thread.'''
        android.push_prop('debug.rs.max-threads', 1)

    def teardown(self, android):

        '''Reset the number of RS threads to the previous value.'''
        android.pop_prop('debug.rs.max-threads')

    @ordered_test(-1)
    @wimpy
    def test_startup(self):

        # pylint: disable=line-too-long
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('breakpoint set --name check_in',
                         ['(pending)'])

    @ordered_test(0)
    @wimpy
    def test_invoke_1(self):

        # enter script_invoke_1
        self.try_command('breakpoint set --name script_invoke_1',
                         ['(pending)'])

        self.try_command('process continue',
                         ['stopped',
                          'stop reason = breakpoint'],
                         [r'librs.rs_single_source.so`script_invoke_1'])

        self.try_command(
            'language renderscript allocation dump 1',
            ['(0, 0, 0) = 1',
             '(1, 0, 0) = 2',
             '(2, 0, 0) = 3',
             '(3, 0, 0) = 4'])

        self.try_command(
            'language renderscript allocation dump 2',
            ['(0, 0, 0) = 5',
             '(1, 0, 0) = 6',
             '(2, 0, 0) = 7',
             '(3, 0, 0) = 8'])

        self.try_command('breakpoint set --name `kernel_1',
                         ['address'])

        self.try_command('breakpoint set --name `kernel_2',
                         ['address'])

        # check our global allocation is visible
        self.try_command('p global_alloc',
                         ['(rs_allocation)',
                          'p = 0x'])

        # test kernel_1
        for _ in range(10):
            # continue as long as there are threads hitting kernel_1
            out = self.do_command('process continue')
            if 'librs.rs_single_source.so`kernel_1' in out:
                continue
            # if we hit check_in we have finished with kernel_1
            if 'librs.rs_single_source.so`check_in' in out:
                self.try_command(
                    'language renderscript allocation dump 1',
                    ['(0, 0, 0) = 25',
                     '(1, 0, 0) = 36',
                     '(2, 0, 0) = 49',
                     '(3, 0, 0) = 64'])
                break
            TestSuiteException('unexpected breakpoint')
        else:
            TestSuiteException('loop quota exceeded')

        # test kernel_2
        for _ in range(10):
            # continue as long as there are threads hitting kernel_2
            out = self.do_command('process continue')
            if 'librs.rs_single_source.so`kernel_2' in out:
                continue
            # if we hit check_in we have finished with kernel_2
            if 'librs.rs_single_source.so`check_in' in out:
                self.try_command(
                    'language renderscript allocation dump 2',
                    ['(0, 0, 0) = 125',
                     '(1, 0, 0) = 216',
                     '(2, 0, 0) = 343',
                     '(3, 0, 0) = 512'])
                break
            TestSuiteException('unexpected breakpoint')
        else:
            TestSuiteException('loop quota exceeded')

    @ordered_test(1)
    @wimpy
    def test_invoke_2(self):

        # enter script_invoke_2
        self.try_command('breakpoint set --name script_invoke_2',
                         ['address'])

        self.try_command('process continue',
                         ['stopped',
                          'stop reason = breakpoint'],
                         [r'librs.rs_single_source.so`script_invoke_2'])

        # test void_kernel_1
        self.try_command('breakpoint set --name void_kernel_1',
                         ['address'])

        for _ in range(10):
            out = self.do_command('process continue')

            # continue as long as there are threads hitting void_kernel_1
            if 'librs.rs_single_source.so`void_kernel_1' in out:
                continue

            # if we hit check_in we have finished with void_kernel_1
            if 'librs.rs_single_source.so`check_in' in out:
                self.try_command(
                    'language renderscript allocation dump 4',
                    ['(0, 0, 0) = 0',
                     '(1, 0, 0) = 1',
                     '(2, 0, 0) = 2',
                     '(3, 0, 0) = 3'])
                break

            TestSuiteException('unexpected breakpoint')
        else:
            TestSuiteException('loop quota exceeded')
