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

'''Module that contains the base class TestBaseRemote'''

from __future__ import absolute_import

import os
import re

from .test_base import TestBase
from . import util_log


class TestBaseRemote(TestBase):
    '''Base class for all tests that connect to a remote device.

    Provides common functionality to set up the connection and tear it down.
    '''

    def __init__(self, device_port, device, timer, *args, **kwargs):
        super(TestBaseRemote, self).__init__(device_port, device, timer, *args, **kwargs)
        # port used by lldb-server on the device.
        self._device_port = device_port
        self._platform = None
        # id of the device that adb will communicate with.
        self._device = device

    def set_src_map(self, file_name, new_src_path):
        '''Call lldb to set the source mapping of a given file.

        Set lldb's source mapping of a given file to a given path. This can be
        used to make the test suite independent of where an APK was compiled.

        Args:
            file_name: String, which is the name of the file whose mapping is
                to be changed
            new_src_path: String which is the new absolute path to the source
                file.
        '''
        line_table = self.do_command('target modules dump line-table '
                                     + file_name)

        lines = line_table.split('\n')
        if 'Line table for' not in lines[0]:
            raise self.TestFail('Could not determine source path of '
                                + file_name)

        # Expecting output like:
        # (lldb) target modules dump line-table scalars.rs
        # Line table for /home/jenkins/workspace/grd-aosp-parameterised-build/
        # merge_151216/frameworks/rs/tests/lldb/java/BranchingFunCalls/src/rs/
        # frameworks/rs/tests/lldb/java/BranchingFunCalls/src/rs/scalars.rs in
        # `librs.scalars.so
        # 0xb30f2374: /home/jenkins/workspace/grd-aosp-parameterised-build/
        # merge_151216/frameworks/rs/tests/lldb/java/BranchingFunCalls/src/rs/
        # scalars.rs:46
        # ...
        # For some reason the first line contains a mangled path?
        old_path = re.findall(r"[^ :]+", lines[1])[1]
        old_dir = os.path.dirname(old_path)

        self.try_command('settings set target.source-map %s %s'
                         % (old_dir, new_src_path), [''])

    def post_run(self):
        '''Clean up after execution.'''
        if self._platform:
            self._platform.DisconnectRemote()

    def _connect_to_platform(self, lldb_module, dbg, remote_pid):
        '''Connect to an lldb platform that has been started elsewhere.

        Args:
            lldb_module: A handle to the lldb module.
            dbg: The instance of the SBDebugger that should connect to the
                 server.
            remote_pid: The integer that is the process id of the binary that
                        the debugger should attach to.

        Returns:
            True if the debugger successfully attached to the server and
            process.
        '''
        # pylint: disable=too-many-return-statements
        remote_pid = str(remote_pid)

        log = util_log.get_logger()

        err1 = dbg.SetCurrentPlatform('remote-android')
        if err1.Fail():
            log.fatal(err1.GetCString())
            return False

        self._platform = dbg.GetSelectedPlatform()
        if not self._platform:
            return False

        connect_string = \
            'adb://{0}:{1}'.format(self._device, self._device_port)
        opts = lldb_module.SBPlatformConnectOptions(connect_string)

        for _ in range(2):
            err2 = self._platform.ConnectRemote(opts)
            if err2.Fail():
                log.error(err2.GetCString())

                if 'Connection refused' in err2.GetCString():
                    log.warning('Connection to lldb server was refused. '
                                'Trying again.')
                else:
                    # Unknown error. Don't try again.
                    return False
            else:
                # Success
                break
        else:
            log.fatal('Not trying again, maximum retries exceeded.')
            return False

        target = dbg.CreateTarget(None)
        if not target:
            return False

        dbg.SetSelectedTarget(target)
        listener = lldb_module.SBListener()
        err3 = lldb_module.SBError()
        process = target.AttachToProcessWithID(listener, int(remote_pid), err3)
        if err3.Fail() or not process:
            log.fatal(err3.GetCString())
            return False

        return True

    def run(self, dbg, remote_pid, lldb):
        '''Execute the actual testsuite.

        Args:
            dbg: The instance of the SBDebugger that is used to test commands.
            remote_pid: The integer that is the process id of the binary that
                        the debugger is attached to.
            lldb: A handle to the lldb module.

        Returns: list of (test, failure) tuples.

        '''
        assert dbg
        assert remote_pid
        assert lldb

        self._lldb = lldb

        self.assert_true(self._connect_to_platform(lldb, dbg, remote_pid))
        self._ci = dbg.GetCommandInterpreter()
        assert self._ci

        self.assert_true(self._ci.IsValid())
        self.assert_true(self._ci.HasCommands())

        return super(TestBaseRemote, self).run(dbg, remote_pid, lldb)

