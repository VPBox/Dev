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

"""
Set of mixins for asserting common RenderScript lldb interactions
That should cut down boilerplate
To use these assertions simply inherit from them along with your
`TestBase`:

    >>> class MyLLDBRenderScriptTest(TestBaseRemote, CoordinateAssertionsMixin):
    >>>     pass

This will give you access to the useful assertion methods related to Coordinates

NOTE: These are strictly clean mixins for `TestBase`. All classes here should
strictly inherit only from `object`
"""


class CoordinateAssertionsMixin(object):
    def assert_coord_bp_set(
            self, breakpoint_expr, x, y=None, z=None, kernel_type='kernel'
        ):
        '''
        Assert that a breakpoint conditional on a given coordinate is confirmed
        by the renderscript breakpoint resolver.
        This does not assert test the breakpoint is hit, only registered.
            breakpoint_expr: the expression (e.g. the name of a function, or a
            file and line).
            kernel_type: The breakpoint resolver to use:
                (reduction|kernel|scriptgroup)
                default='kernel'
            x: x coordinate: required
            y, z: optional y, and z coordinates
        '''

        y = 0 if z is not None and y is None else y
        coord_text = ','.join(map(str, filter(lambda p: p is not None, (x, y, z))))
        self.try_command(
            'language renderscript %s breakpoint set %s -c %s' % (
                kernel_type, breakpoint_expr, coord_text
            ),
            [r'Breakpoint(s) created'],
            expected_regex=[
                r'Conditional kernel breakpoint on coordinate.+%d,\s*%d,\s*%d' % (
                    x or 0, y or 0, z or 0
                )
            ]
        )

    def assert_coord_stop(
            self, soname, func_name, x, y=None, z=None, stopped=True
        ):
        '''Run lldb commands to check that coordinates match expected values.

        Args:
            (x, y, z): The expected coordinates.
            soname: The name of the renderscript script module e.g. 'allocs'
            for librs.allocs.so
            func_name: String that is the name of the kernel function

        Raises:
            TestFail: One of the lldb commands did not provide the expected
                      output.
        '''

        if stopped:
            self.try_command(
                'process continue',
                expected_regex=[
                    r'resuming',
                    r'Process \d+ stopped',
                    r'stop reason = breakpoint',
                    r'frame #0: (0x[0-9a-fA-F]+ )?librs.%s.so`%s' % (
                        soname, func_name)
                ]
            )
        else:
            self.try_command(
                'bt',
                expected_regex=[
                    'stop reason = breakpoint',
                    'frame #0:',
                    'librs.*\.so`%s' % kernel
                ]
            )

        self.try_command(
            'language renderscript kernel coordinate',
            '(%d, %d, %d)' % (x, y or 0, z or 0)
        )
