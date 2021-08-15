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

'''Module that contains the test TestBreakpointCoordinate.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    wimpy,
    ordered_test,
    cpp_only_test,
)
from harness.assert_mixins import CoordinateAssertionsMixin


class TestBreakpointCoordinate(TestBaseRemote, CoordinateAssertionsMixin):
    '''Tests breaking on a specific kernel invocation.

    Uses the -c option to specify the coordinate.
    '''

    bundle_target = {
        'java': 'Allocations',
        'jni': 'JNIAllocations',
        'cpp': 'CppAllocations'
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

    @wimpy
    @ordered_test(0)
    def test_breakpoint_coordinate_2d_swizzle_kernel(self):
        # pylint: disable=line-too-long

        # test conditional coordinate in two dimensions
        # breakpoint 1
        self.assert_coord_bp_set('swizzle_kernel', 3, 7)

        # we will delete this breakpoint before we hit it.
        # breakpoint 2
        self.assert_coord_bp_set('swizzle_kernel', 199, 190)

        self.assert_coord_stop('allocs', 'swizzle_kernel', x=3, y=7)

        # check breakpoints that have been hit are disabled
        self.try_command(
            'breakpoint list',
            [
                "1: RenderScript kernel breakpoint for 'swizzle_kernel', locations = 1 Options: disabled",
                "2: RenderScript kernel breakpoint for 'swizzle_kernel', locations = 1"
            ]
        )

        # delete breakpoint on 199,199,0
        self.try_command('breakpoint delete 2', ['1 breakpoints deleted'])

        # check breakpoints that have been hit are disabled
        self.try_command(
            'breakpoint list',
            ["1: RenderScript kernel breakpoint for 'swizzle_kernel', locations = 1 Options: disabled"]
        )

        # test conditional coordinate in a single dimension
        # breakpoint 3
        self.assert_coord_bp_set('square_kernel', 8)

        # check breakpoints that have been hit are disabled
        self.try_command(
            'breakpoint list',
            [
                "1: RenderScript kernel breakpoint for 'swizzle_kernel', locations = 1 Options: disabled",
                "3: RenderScript kernel breakpoint for 'square_kernel', locations = 1"
            ]
        )

        self.assert_coord_stop('allocs', 'square_kernel', x=8)

        # check breakpoints that have been hit are disabled
        self.try_command(
            'breakpoint list',
            [
                "1: RenderScript kernel breakpoint for 'swizzle_kernel', locations = 1 Options: disabled",
                "3: RenderScript kernel breakpoint for 'square_kernel', locations = 1 Options: disabled"
            ]
        )

    @wimpy
    @ordered_test(1)
    def test_breakpoint_coordinate_3d_add_half_kernel(self):
        # test conditional coordinate in three dimensions
        # breakpoint 4
        self.assert_coord_bp_set('add_half_kernel', 0, 0, 1)
        # test we can set more than one conditional kernel breakpoint
        # and both will be hit;
        # breakpoint 5
        self.assert_coord_bp_set('add_half_kernel', 0, 1, 2)

        # Now assert that the next two continue/stop cycles hit our conditionals
        self.assert_coord_stop('allocs', 'add_half_kernel', x=0, y=0, z=1)
        self.assert_coord_stop('allocs', 'add_half_kernel', x=0, y=1, z=2)

        # check we can see the coordinate from a function invoked by the kernel
        # breakpoint 6
        self.try_command(
            'break set -n half_helper',
            ['librs.allocs.so`half_helper']
        )

        # continue till we hit breakpoint 6
        self.assert_coord_stop('allocs', 'half_helper', x=0, y=1, z=2)

        self.try_command(
            'breakpoint list',
            [
                "1: RenderScript kernel breakpoint for 'swizzle_kernel', locations = 1 Options: disabled",
                "3: RenderScript kernel breakpoint for 'square_kernel', locations = 1 Options: disabled",
                "4: RenderScript kernel breakpoint for 'add_half_kernel', locations = 1 Options: disabled",
                "5: RenderScript kernel breakpoint for 'add_half_kernel', locations = 1 Options: disabled",
                "6: name = 'half_helper', locations = 1, resolved = 1, hit count = 1"
            ]
        )

        self.try_command('breakpoint delete 3', ['1 breakpoints deleted'])

        self.try_command(
            'breakpoint list',
            [
                "1: RenderScript kernel breakpoint for 'swizzle_kernel', locations = 1 Options: disabled",
                "4: RenderScript kernel breakpoint for 'add_half_kernel', locations = 1 Options: disabled",
                "5: RenderScript kernel breakpoint for 'add_half_kernel', locations = 1 Options: disabled",
                "6: name = 'half_helper', locations = 1, resolved = 1, hit count = 1"
            ]
        )

        self.try_command('breakpoint delete 6', ['1 breakpoints deleted'])

        self.try_command(
            'breakpoint list',
            [
                "1: RenderScript kernel breakpoint for 'swizzle_kernel', locations = 1 Options: disabled",
                "4: RenderScript kernel breakpoint for 'add_half_kernel', locations = 1 Options: disabled",
                "5: RenderScript kernel breakpoint for 'add_half_kernel', locations = 1 Options: disabled"
            ]
        )

    @cpp_only_test()
    @ordered_test('last')
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 4', ['1 breakpoints deleted'])
        self.try_command('breakpoint delete 5', ['1 breakpoints deleted'])
        self.try_command('process continue', ['exited with status = 0'])
