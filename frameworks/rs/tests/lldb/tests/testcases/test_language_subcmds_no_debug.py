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

'''Module that contains the test TestLanguageSubcmdsNoDebug.'''

from __future__ import absolute_import

import os

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    cpp_only_test,
    ordered_test,
)


class TestLanguageSubcmdsNoDebug(TestBaseRemote):
    '''Tests the 'language renderscript' subcommands without debug info.

    In particular, module dump should report missing debug info.
    '''

    bundle_target = {
        'java': 'JavaNoDebugWaitAttach',
        'jni': 'JNINoDebugWaitAttach',
        'cpp': 'CppNoDebugWaitAttach'
    }

    def _pkg_name(self):
        return {
            'java': 'com.android.rs.waitattachnodebug',
            'jni': 'com.android.rs.jninodebugwaitattach',
            'cpp': 'com.android.rs.cppwaitattach'
        }[self.app_type]

    @ordered_test(0)
    def test_language_subcommands_no_debug(self):
        # pylint: disable=line-too-long
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered'])

        self.try_command('language renderscript kernel breakpoint set simple_kernel'
                         '',
                         ['(pending)'])

        self.try_command('process continue',
                         [])

        self.try_command('language renderscript kernel',
                         ['breakpoint',
                          'coordinate',
                          'list'])

        self.try_command('language renderscript kernel list',
                         ['RenderScript Kernels',
                          "Resource 'simple'",
                          'root',
                          'simple_kernel'])

        self.try_command('language renderscript context',
                         ['dump'])

        self.try_command('language renderscript context dump',
                         ['Inferred RenderScript Contexts',
                          '1 script instances'])

        self.try_command('language renderscript allocation',
                         ['list',
                          'load',
                          'save',
                          'dump',
                          'refresh'])

        self.try_command('language renderscript allocation list',
                         ['RenderScript Allocations:'])

        self.try_command('language renderscript allocation list -i 0',
                         ['RenderScript Allocations:'])

        self.try_command('language renderscript allocation list --id 0',
                         ['RenderScript Allocations:'])

        self.try_command('language renderscript allocation dump 1',
                         ['Data (X, Y, Z):'])

        output_file = self.get_tmp_file_path()
        self.try_command('language renderscript allocation dump 1 -f ' +
                         output_file,
                         ["Results written to '%s'" % output_file])

        if os.path.isfile(output_file):
            os.remove(output_file)

        self.try_command('language renderscript allocation dump 1 --file ' +
                         output_file,
                         ["Results written to '%s'" % output_file])

        self.try_command('language renderscript allocation save 1 ' +
                         output_file,
                         ["Allocation written to file '%s'" % output_file])

        self.try_command('language renderscript allocation load 1 ' +
                         output_file,
                         ["Contents of file '%s' read into allocation 1" %
                          output_file])

        self.try_command('language renderscript allocation refresh',
                         ['All allocations successfully recomputed'])

        # C++ tests have an additional kernel `other_kernel`
        kernel_count = 3 if self.app_type == 'cpp' else 2
        self.try_command('language renderscript module',
                         ['dump'])

        self.try_command('language renderscript module dump',
                         ['RenderScript Modules:',
                          'librs.simple.so',
                          'Debug info does not exist.',
                          'Globals: 1',
                          'gColor - variable identified, but not found in '
                            'binary (symbol exists)',
                          'Kernels: %s' % kernel_count,
                          'root',
                          'simple_kernel',
                          '',
                          'java_package_name: %s' % self._pkg_name(),
                          'version'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('process continue', ['exited with status = 0'])
