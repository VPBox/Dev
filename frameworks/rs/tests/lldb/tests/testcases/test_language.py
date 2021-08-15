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

'''Module that contains the test TestLanguage.'''

from __future__ import absolute_import

from harness.test_base import TestBaseNoTargetProcess


class TestLanguage(TestBaseNoTargetProcess):
    '''
    Tests the "language" command and "language renderscript" subcommand.
    '''

    def test_lldb_has_language_commands(self):
        ci = self._ci
        self.assert_true(
            ci.HasCommands() and
            ci.CommandExists('language')
        )

        self.try_command('language', ['renderscript'])
        self.try_command('language renderscript', ['kernel',
                                                   'context',
                                                   'module',
                                                   'status'])

