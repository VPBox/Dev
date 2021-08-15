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

'''Module that contains the test TestMultipleRSFiles.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote
from harness.decorators import (
    ordered_test,
    cpp_only_test,
)

class TestMultipleRSFiles(TestBaseRemote):
    '''Tests some commands on an apk which has two rs files.'''

    bundle_target = {
        'java': 'MultipleRSFiles',
        'jni': 'JNIMultipleRSFiles',
        'cpp': 'CppMultipleRSFiles'
    }

    def _binary_name(self):
        return {
            'java': 'multiplersfiles',
            'jni': 'multiplersfiles',
            'cpp': 'CppMultipleRSFi'
        }[self.app_type]

    def _pkg_name(self):
        return {
            'java': 'com.android.rs.multiplersfiles',
            'jni': 'com.android.rs.jnimultiplersfiles',
            'cpp': 'com.android.rs.cppmultiplersfiles'
        }[self.app_type]

    def test_multiple_rs_files(self):
        self.try_command('language renderscript status',
                         ['Runtime Library discovered',
                          'Runtime Driver discovered',
                          'Runtime functions hooked'])

        self.try_command('breakpoint set --file first.rs --line 28',
                         ['(pending)'])

        self.try_command('process continue',
                         ['stopped',
                          'librs.first.so`first_kernel',
                          'at first.rs:28',
                          "name = '%s'" % self._binary_name(),
                          'stop reason = breakpoint 1'])

        self.try_command('language renderscript kernel list',
                         ['RenderScript Kernels',
                          "Resource 'first'",
                          "Resource 'second'",
                          'root',
                          'first_kernel',
                          'second_kernel'])

        self.try_command('language renderscript context dump',
                         ['Inferred RenderScript Contexts',
                          '2 script instances'])

        self.try_command('language renderscript module dump',
                         ['RenderScript Modules:',
                          'librs.first.so',
                          'librs.second.so',
                          'Debug info loaded',
                          'Globals: 1',
                          'gColor - float4',
                          'Kernels: 2',
                          'root',
                          'first_kernel',
                          'second_kernel',
                          'java_package_name: %s' % self._pkg_name(),
                          'version:'])

    @ordered_test('last')
    @cpp_only_test()
    def test_cpp_cleanup(self):
        self.try_command('breakpoint delete 1', ['1 breakpoints deleted'])

        self.try_command('process continue', ['exited with status = 0'])
