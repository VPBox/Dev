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

'''Module that contains the test DWARF language attribute test.'''

from __future__ import absolute_import

from harness.test_base_remote import TestBaseRemote


class TestDWARFLang(TestBaseRemote):
    '''Tests the DWARF language attribute is present in RenderScript kernels.'''

    bundle_target = {
        'java': 'JavaDebugWaitAttach',
        'jni': 'JNIDebugWaitAttach',
        'cpp': 'CppDebugWaitAttach'
    }

    def test_renderscript_kernel_frame_dwarf_language(self):
        self.try_command('language renderscript status', [])
        self.try_command('b simple_kernel', [])
        self.try_command('process continue', [])

        self.assert_lang_renderscript()
