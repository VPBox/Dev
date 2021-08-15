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

import re
import itertools

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    wimpy,
    skip_conditional,
)
from harness.assert_mixins import CoordinateAssertionsMixin

from reduce_common import (
    REDUCE_SCRIPT,
    REDUCE_AUTO_COMB_SCRIPT,
    ReductionMixin,
)


multithreaded = lambda: skip_conditional(
    lambda self: self.cpu_count == 1,
    "skipping multithreaded test (1 CPU present)"
)


class TestReduceCombinerMultithreaded(
        TestBaseRemote, CoordinateAssertionsMixin, ReductionMixin):
    """
    RenderScript reduction combiners are currently run only on the output of a
    parallel reduction step for the CPU reference implementation.  These
    testcases test LLDB's handling of breakpoints for the combiner function.
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
        """
        This test *must* be run on multiple threads, and is skipped if the
        device does not support multiple threads
        """
        cpu_spec = android.shell("cat /sys/devices/system/cpu/online").strip()
        match = re.search(r'(^0(-\d+)?(,\d+([-]\d*)?)*)$', cpu_spec)
        if not match or not match.groups():
            raise self.TestFail(
                "unable to parse number of available CPUs in %r" % cpu_spec)

        def parse_range(s):
            r = s.split('-')
            if len(r) == 1:
                return 1
            return int(r[1]) - int(r[0])

        self.cpu_count = sum(map(parse_range, cpu_spec.split(',')))
        android.push_prop('debug.rs.max-threads', self.cpu_count + 1)

    def teardown(self, android):
        """Reset the number of RS threads to the previous value."""
        android.pop_prop('debug.rs.max-threads')

    @multithreaded()
    @ordered_test(0)
    @wimpy
    def test_setup(self):
        self.try_command('language renderscript status', [])
        # first point of order: make sure the compiled script is properly
        # loaded and that we can set a breakpoint on the named reduction
        self.try_command(
            'language renderscript reduction breakpoint set find_min_user_type_auto_comb')
        self.try_command(
            'process continue',
            expected_regex=[
                r'Process \d+ stopped',
                r'frame #0: (0x[0-9a-fA-F]+ )?librs.reduce_auto_comb.so`'
            ]
        )

    @multithreaded()
    def test_function_role_breakpoints_combinations(self):
        func_role_combinations = itertools.combinations(
            ('accumulator', 'outconverter', 'initializer', 'combiner'),
            r=2
        )
        self._test_func_role_combinations(func_role_combinations)

    @multithreaded()
    def test_reduction_breakpoint_set_single_type_user_comb(self):
        return self._reduction_breakpoint_set_single_type(
            'reduce',
            REDUCE_SCRIPT,
            'find_min_user_type',
            (
                ('find_min_user_type_init', 'initializer'),
                ('find_min_user_type_accum', 'accumulator'),
                ('find_min_user_type_comb', 'combiner'),
                ('find_min_user_type_outc', 'outconverter')
            )
        )

    @multithreaded()
    def test_reduction_breakpoint_set_single_type_auto_comb(self):
        return self._reduction_breakpoint_set_single_type(
            'reduce_auto_comb',
            REDUCE_AUTO_COMB_SCRIPT,
            'find_min_user_type_auto_comb',
            (
                ('find_min_user_type_init', 'initializer'),
                ('find_min_user_type_accum', 'accumulator'),
                ('find_min_user_type_accum.combiner', 'combiner'),
                ('find_min_user_type_outc', 'outconverter')
            )
        )
