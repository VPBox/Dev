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

'''Module that contains the test TestRSConsts.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    cpp_only_test,
)


class TestRSConsts(TestBaseRemote):
    '''Tests examining the RenderScript constants.'''

    bundle_target = {
        'java': 'KernelVariables',
        'jni': 'JNIKernelVariables',
        'cpp': 'CppKernelVariables'
    }

    def test_rs_consts(self):
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('language renderscript kernel breakpoint set kernel',
                         [])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

        # Constants
        self.try_command('expr M_1_PI',
                         ['0.318309'])

        self.try_command('expr M_2_PI',
                         ['0.636619'])

        self.try_command('expr M_2_SQRTPI',
                         ['1.128379'])

        self.try_command('expr M_E',
                         ['2.718281'])

        self.try_command('expr M_LN10',
                         ['2.302585'])

        self.try_command('expr M_LN2',
                         ['0.693147'])

        self.try_command('expr M_LOG10E',
                         ['0.434294'])

        self.try_command('expr M_LOG2E',
                         ['1.442695'])

        self.try_command('expr M_PI',
                         ['3.141592'])

        self.try_command('expr M_PI_2',
                         ['1.570796'])

        self.try_command('expr M_PI_4',
                         ['0.785398'])

        self.try_command('expr M_SQRT1_2',
                         ['0.707106'])

        self.try_command('expr M_SQRT2',
                         ['1.414213'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('process continue', ['exited with status = 0'])

