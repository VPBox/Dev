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

from __future__ import absolute_import

import itertools

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    wimpy,
)
from harness.assert_mixins import CoordinateAssertionsMixin

from reduce_common import (
    REDUCE_ITERATIONS,
    REDUCE_STARTVAL,
    REDUCE_SCRIPT,
    X_TESTS,
    Y_TESTS,
    Z_TESTS,
    ReductionMixin,
)


def coords_range_3d(x_range, y_range, z_range):
    count = max((x_range, y_range, z_range))
    x = itertools.cycle(range(x_range))
    y = itertools.cycle(range(y_range))
    z = itertools.cycle(range(z_range))
    return itertools.islice(
        itertools.izip(x, y, z),
        count
    )


class TestReduce1DSingleThreaded(
        TestBaseRemote, CoordinateAssertionsMixin, ReductionMixin):
    """
    Reduction kernels for RenderScript are launched using
    a different `.expand` function than regular `ForEach` kernels and reflect a
    different API to the invoking program

    Although the debugger implementation for accessing these features tracks
    this slightly differently for reduction kernels, the user interface should
    still offer the basic functionality:
        - breakpoints on a coordinate
        - tracking, viewing and dumping allocations
        - listing modules and constituent kernels and types
    """

    bundle_target = {
        'java': 'Reduction',
    }

    def _delete_breakpoints(self):
        try:
            self.do_command('breakpoint delete -f')
        except self.TestFail:
            pass

    def setup(self, android):
        """This test requires to be run on one thread."""
        android.push_prop('debug.rs.max-threads', 1)

    def teardown(self, android):
        """Reset the number of RS threads to the previous value."""
        android.pop_prop('debug.rs.max-threads')

    @ordered_test(0)
    @wimpy
    def test_setup(self):
        self.try_command('language renderscript status', [])
        self.try_command('b find_min_user_type_accum', [])
        self.try_command('c', [])

    @ordered_test(1)
    @wimpy
    def test_renderscript_module_dump(self):
        """
        Generalised Reduction kernels for RenderScript are not tracked in the
        same way as `ForEach` kernels, and do not have `__attribute__((kernel))`
        so we need to make sure that when a module contains reduction kernels,
        `language renderscript module dump` in lldb prints the correct kernels.
        """
        self.try_command(
            'language renderscript module dump',
            [
                'Reductions: 1',
                'find_min_user_type',
                'accumulator: find_min_user_type_accum',
                'combiner: find_min_user_type_comb',
                'outconverter: find_min_user_type_outc'
            ]
        )

    @ordered_test(2)
    @wimpy
    def test_module_dump_with_foreach_kernel_separate(self):
        """
        The reduction breakpoint is separate from that of a standard kernel
        function breakpoint, so we need to make sure that when we dump a module,
        reductions are properly collected and displayed alongside the standard
        __attribute__((kernel)) functions.
        Assert that `... module dump` can correctly distinguish between `reduce`
        kernels and `ForEach` kernels.
        """
        self.try_command(
            'language renderscript module dump',
            [
                'Kernels: 2',
                'Reductions: 1',
                'accumulator: find_min_user_type_accum',
                'initializer: find_min_user_type_init',
                'combiner: find_min_user_type_comb',
                'outconverter: find_min_user_type_outc'
            ]
        )

    @wimpy
    @ordered_test(3)
    def test_reduction_breakpoint_set_all_roles_resolved(self):
        """
        Assert that a reduction breakpoint successfully resolves all the
        functions that make up the reduction kernel
        """
        self.try_command(
            'language renderscript reduction breakpoint set find_min_user_type',
            ['Breakpoint(s) created']
        )

        self.try_command(
            'process continue',
            expected_regex=[
                r'Process \d+ stopped',
                r'librs.reduce.so`find_min_user_type',
                r'stop reason = breakpoint'
            ]
        )
        name = REDUCE_SCRIPT
        self.try_command(
            'breakpoint list',
            expected_regex=[
                "RenderScript reduce breakpoint for 'find_min_user_type', locations = 4, resolved = 4",
                'where = librs.reduce.so`find_min_user_type_init (\+ \d+ )?at %s(.+, resolved,)' % name,
                'where = librs.reduce.so`find_min_user_type_accum (\+ \d+ )?at %s(.+, resolved,)' % name,
                'where = librs.reduce.so`find_min_user_type_comb (\+ \d+ )?at %s(.+, resolved,)' % name,
                'where = librs.reduce.so`find_min_user_type_outc (\+ \d+ )?at %s(.+, resolved,)' % name,
            ]
        )

    @ordered_test(4)
    def test_reduce_iterations(self):
        """
        Given a reduction, we want to make sure that we break on
        every accumulator invocation before seeing the outconverter called.
        This requires the tests to be run single threaded
        """
        self._delete_breakpoints()
        self.try_command(
            'language renderscript reduction breakpoint set find_min_user_type -t initializer',
        )
        self.try_command(
            'process continue',
            expected_regex=[
                r'Process \d+ stopped',
                r'librs.reduce.so`find_min_user_type_init',
                r'stop reason = breakpoint',
            ]
        )
        self._delete_breakpoints()

        self.try_command((
            'language renderscript reduction breakpoint '
            'set find_min_user_type --function-role accumulator,outconverter'),
            ['Breakpoint(s) created']
        )
        for i in range(REDUCE_ITERATIONS):
            self.try_command(
                'process continue',
                expected_regex=[
                    r'Process \d+ resuming',
                    r'Process \d+ stopped',
                    r'librs.reduce.so`find_min_user_type_accum',
                    r'stop reason = breakpoint'
                ]
            )
            self.try_command('p val')
            self.try_command(
                'p val.b',
                expected_regex=[
                    r'^\((const )?int32_t\)\s*\$\d+ = %s\s*$' % (
                        i + REDUCE_STARTVAL)
                ]
            )
        # We should then finally break on the outconverter
        self.try_command(
            'process continue',
            expected_regex=[
                r'Process \d+ resuming',
                r'Process \d+ stopped',
                r'librs.reduce.so`find_min_user_type_outc',
                r'stop reason = breakpoint'
            ]
        )

    @ordered_test(5)
    def test_function_role_breakpoints_combinations(self):
        func_role_combinations = itertools.combinations(
            ('accumulator', 'initializer'),
            r=2
        )
        self._test_func_role_combinations(func_role_combinations)

    @wimpy
    @ordered_test(6)
    def test_resolve_function_role_all_reduce_functions(self):
        """
        Assert that a reduction breakpoint successfully resolves all the
        functions that make up the reduction kernel when the parameter `all` is
        passed to `--function-role` for the breakpoint command
        """
        self._delete_breakpoints()
        self.try_command(
            'language renderscript reduction breakpoint set find_min_user_type -t all',
            [r'Breakpoint(s) created']
        )
        self.try_command('c', [])
        breakpoints_match = [
            r"where = librs.reduce.so`%s (\+ \d+ )?at %s:\d+, address = 0x[0-9a-fA-F]+, resolved" % (
                 'find_min_user_type_%s' % func_match,
                 REDUCE_SCRIPT
            )
            for func_match in ('accum', 'init', 'comb', 'outc')
        ]
        self.try_command(
            'breakpoint list',
            expected_regex=[
                r"Current breakpoints:",
                r"RenderScript reduce breakpoint for 'find_min_user_type', locations = 4, resolved = 4",
                r"Names:",
                r"RenderScriptReduction",
            ] + breakpoints_match
        )

    @ordered_test(8)
    def test_reduce_breakpoint_conditional_1d_coordinate(self):
        """
        Assert that breakpoints conditional on an allocation coordinate
        are only triggered on that coordinate
        """
        for x, _, __ in sorted(coords_range_3d(X_TESTS, Y_TESTS, Z_TESTS)):
            self._delete_breakpoints()
            self.assert_coord_bp_set(
                'find_min_user_type -t accumulator',
                x,
                kernel_type='reduction'
            )
            self.assert_coord_stop('reduce', 'find_min_user_type', x)
            # Step *into* the function so locals are available
            # FIXME remove the need for `next` here; skip the function prologue
            self.try_command('n')
            self.try_command('p accum->a')
            self.try_command('p accum->b')

    @ordered_test('last')
    def test_exit(self):
        self.try_command('process kill', [])
