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

'''Module that contains the class UtilLLDB, which provides lldb utility
methods.'''

from __future__ import absolute_import

from . import util_constants

try:
    import lldb
except ImportError:
    print('unable to import lldb')
    print('please run "lldb -P" and add to $PYTHONPATH')
    quit(util_constants.RC_TEST_FATAL)


class UtilLLDB(object):
    '''Provides utility methods to interface with lldb's python bindings.'''

    @staticmethod
    def start():
        '''Initialise the lldb debugger framework.'''
        lldb.SBDebugger_Initialize()

    @staticmethod
    def stop():
        '''Terminate the lldb debugger framework.

        Raises:
            AssertionError: If an assertion fails.
        '''
        assert lldb
        lldb.SBDebugger_Terminate()

    @staticmethod
    def create_debugger():
        '''Create an lldb debugger instance.

        Returns:
            The SBDebugger instance that was created.

        Raises:
            AssertionError: If an assertion fails.
        '''
        assert lldb
        inst = lldb.SBDebugger_Create()
        inst.SetAsync(False)
        return inst

    @staticmethod
    def destroy_debugger(dbg):
        '''Destroy the lldb debugger instance.

        Args:
            dbg: Instance of SBDebugger that is to be destroyed.

        Raises:
            AssertionError: If an assertion fails.
        '''
        assert lldb
        lldb.SBDebugger_Destroy(dbg)

    @staticmethod
    def get_module():
        '''Get the lldb module.

        Returns:
            The lldb module.

        Raises:
            AssertionError: If an assertion fails.
        '''
        assert lldb
        return lldb
