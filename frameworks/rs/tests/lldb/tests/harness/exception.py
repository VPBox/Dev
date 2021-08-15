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

'''Module that contains TestSuiteException.'''

from __future__ import absolute_import

class TestSuiteException(Exception):
    '''Exception that is thrown whenever an internal error is encountered.

    Just contains a message.
    '''
    pass

class DisconnectedException(Exception):
    '''Exception that is thrown if lldb-server unexpectedly disconnected.

    Just contains a message.
    '''
    pass


class FailFastException(TestSuiteException):
    '''Quick Bailout'''
    pass


class TestIgnoredException(TestSuiteException):
    '''Raised when a testcase is ignored.'''
    pass
