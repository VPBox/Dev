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

'''Module that contains the test TestCallApiFuns.'''

from __future__ import absolute_import

import re
import string

from harness.test_base_remote import TestBaseRemote
from harness import RS_funs
from harness.decorators import (
    wimpy,
    ordered_test,
    cpp_only_test,
)


class _APIFunsExprTestsMeta(type):
    """
    Generate unique, standalone test methods from a list of lldb expressions.
    The lldb expression evaluation engine for calling RenderScript
    builtins need to be tested thoroughly; rather than manually
    write the 1000s of individual test cases, we automatically generate them
    and their variants to add to the test class. This is done from a list
    of expressions that are all tested in the same way.
    """
    def __new__(self, name, bases, class_dict):
        func_name_sub = re.compile(r'[%s\s]+' % string.punctuation)

        for count, line in enumerate(RS_funs.FUNC_LIST):
            def make_test(line):
                """
                We use an extra level of indirection here to properly
                close over the *value* of the loop variable, `line`
                """
                @ordered_test(count)
                def test(self):
                    # build the expression
                    ret, expr = RS_funs.build_expr(line)
                    try:
                        # evaluate the expression with expected return value
                        self.try_command(expr, [], [RS_funs.TYPE_MAP[ret]])
                    except KeyError:
                        # or just check the return type if no return value
                        # specified
                        self.try_command(expr, '(%s)' % ret)
                return test

            # Make a pretty python method that adheres to the testcase standard
            # Use the `count` parameter to ensure the name is unique in the class
            test_name = 'test_%s_%s' % (re.sub(func_name_sub, '_', line), count)
            test = make_test(line)
            test.func_name = test_name
            # We mark every 10th test case as runnable in wimpy mode
            class_dict[test_name] = wimpy(test) if count % 10 == 0 else test

        return type(name, bases, class_dict)


class TestCallApiFuns(TestBaseRemote):
    '''Tests calling of some RS API functions. This tests that JITing works.'''

    __metaclass__ = _APIFunsExprTestsMeta

    bundle_target = {
        'java': "KernelVariables",
        'jni': "JNIKernelVariables",
        'cpp': "CppKernelVariables"
    }

    @wimpy
    @ordered_test(-2)
    def test_setup(self):
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('b -f simple.rs -l 145', [])

        self.try_command('process continue',
                         ['resuming',
                          'stopped',
                          'stop reason = breakpoint'])

    @wimpy
    @ordered_test(-1)
    def test_call_api_funs_atomic(self):
        # Test the atomics separately because we want to check the output
        # AtomicAdd(1234, 2)
        self.try_command('expr rsAtomicAdd(&int_global, 2)',
                         ['1234'],
                         [r'\(int(32_t)?\)'])

        self.try_command('expr int_global',
                         ['(int)',
                          '1236'])

        # AtomicAnd(2345, 333)
        self.try_command('expr rsAtomicAnd(&uint_global, 333)',
                         ['2345'],
                         [r'\(int(32_t)?\)'])

        self.try_command('expr uint_global',
                         ['(uint)',
                          '265'])

        # AtomicCas(1236, 1236, 2345)
        self.try_command('expr rsAtomicCas(&int_global, 1236, 2345)',
                         ['1236'],
                         [r'\(int(32_t)?\)'])

        self.try_command('expr int_global',
                         ['(int)',
                          '2345'])

        # AtomicDec(265)
        self.try_command('expr rsAtomicDec(&uint_global)',
                         ['265'],
                         [r'\(int(32_t)?\)'])

        self.try_command('expr uint_global',
                         ['(uint)',
                          '264'])

        # AtomicInc(2345)
        self.try_command('expr rsAtomicInc(&int_global)',
                         ['2345'],
                         [r'\(int(32_t)?\)'])

        self.try_command('expr int_global',
                         ['(int)',
                          '2346'])

        # AtomicMax(264, 3456)
        self.try_command('expr rsAtomicMax(&uint_global, 3456)',
                         ['264'],
                         [r'\(uint(32_t)?\)'])

        self.try_command('expr uint_global',
                         ['(uint)',
                          '3456'])

        # AtomicMin(2346, 3)
        self.try_command('expr rsAtomicMin(&int_global, 3)',
                         ['2346'],
                         [r'\(int(32_t)?\)'])

        self.try_command('expr int_global',
                         ['(int)',
                          '3'])

        # AtomicOr(3, 456)
        self.try_command('expr rsAtomicOr(&int_global, 456)',
                         ['3'],
                         [r'\(int(32_t)?\)'])

        self.try_command('expr int_global',
                         ['(int)',
                          '459'])

        # AtomicSub(3456, 7)
        self.try_command('expr rsAtomicSub(&uint_global, 7)',
                         ['3456'],
                         [r'\(int(32_t)?\)'])

        self.try_command('expr uint_global',
                         ['(uint)',
                          '3449'])

        # AtomicXor(459, 89)
        self.try_command('expr rsAtomicXor(&int_global, 89)',
                         ['459'],
                         [r'\(int(32_t)?\)'])

        self.try_command('expr int_global',
                         ['(int)',
                          '402'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('process continue', ['exited with status = 0'])
