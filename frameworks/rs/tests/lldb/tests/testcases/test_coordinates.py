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

'''Module that contains the test TestCoordinates.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    cpp_only_test
)


class TestCoordinates(TestBaseRemote):
    '''Tests the inspection of coordinates.

    Tests the inspection of the range and dimension of coordinates as well
    as the current coordinates.'''

    bundle_target = {
        'java': 'JavaDebugWaitAttach',
        'jni': 'JNIDebugWaitAttach',
        'cpp': 'CppDebugWaitAttach'
    }

    def setup(self, android):
        '''This test requires to be run on one thread.

        Args:
            android: The android_util module.
        '''
        android.push_prop('debug.rs.max-threads', 1)

    def teardown(self, android):
        '''Reset the number of RS threads to the previous value.

        Args:
            android: The android_util module.
        '''
        android.pop_prop('debug.rs.max-threads')

    @ordered_test(0)
    def test_inspect_coordinates(self):
        # pylint: disable=line-too-long
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('language renderscript kernel breakpoint set simple_kernel',
                         ['Breakpoint(s) created',
                          '(pending)'])

        # Check the initial conditions.
        self._lldb_continue()
        self._inspect_coordinates(0, 0, 0)

        # Check two more steps.
        self._lldb_continue()
        self._inspect_coordinates(1, 0, 0)
        self._lldb_continue()
        self._inspect_coordinates(2, 0, 0)

        # After eight more steps we should have advanced one step in the y dimension.
        for _ in range(8):
            self._lldb_continue()
        self._inspect_coordinates(2, 1, 0)

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('process continue',
                         ['exited with status = 0'])

    def _lldb_continue(self):
        '''Try 'continue' lldb command. Expect to hit a breakpoint.'''
        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

    def _inspect_coordinates(self, x_coord, y_coord, z_coord):
        '''Run lldb commands to inspect kernel size and coordinates
        and match against expected values.

        Args:
            (x_coord, y_coord, z_coord): The expected coordinates (int triple)

        Raises:
            TestFail: One of the lldb commands did not provide the expected
                      output.
        '''
        self.try_command('language renderscript kernel coordinate',
                         ['Coordinate: (%d, %d, %d)'
                          % (x_coord, y_coord, z_coord)])

        self.try_command('frame select 1',
                         ['librs.simple.so`simple_kernel.expand',
                         'at generated.rs:1'])

        # Inspect the invocation length, should be the same every time.
        self.try_command('expr p->dim',
                         ['x = 8',
                          'y = 8',
                          'z = 0'])

        # The X coordinate is in the rsIndex variable.
        self.try_command('expr rsIndex',
                          ['= ' + str(x_coord)])

        # Inspect the Y and Z coordinates.
        self.try_command('expr p->current',
                         ['x = ' + str(0),
                          'y = ' + str(y_coord),
                          'z = ' + str(z_coord)])
