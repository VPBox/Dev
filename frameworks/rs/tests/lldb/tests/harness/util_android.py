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

'''Module that contains the class UtilAndroid, providing utility method to
interface with Android ADB.'''

from __future__ import absolute_import

import logging
import re
import subprocess
import time
import collections
import multiprocessing
try:
    # Python 3
    import queue
except ImportError:
    import Queue as queue

from .exception import TestSuiteException
from . import util_log


class UtilAndroid(object):
    '''Provides some utility methods that interface with Android using adb.'''
    # pylint: disable=too-many-public-methods

    def __init__(self, adb_path, lldb_server_path_device, device):
        # The path to the adb binary on the local machine
        self._path_adb = adb_path
        # The path to the lldb server binary on the device
        self._path_lldbserver = lldb_server_path_device
        self._log = util_log.get_logger()
        self.device = device
        self._prop_stacks = collections.defaultdict(list)
        return

    @staticmethod
    def _validate_string(string):
        '''Check that a string is valid and not empty.

        Args:
            string: The string to be checked.
        '''
        assert isinstance(string, str)
        assert len(string) > 0

    def adb(self, args, async=False, device=True, timeout=None):
        '''Run an adb command (async optional).

        Args:
            args: The command (including arguments) to run in adb.
            async: Boolean to specify whether adb should run the command
                   asynchronously.
            device: boolean to specify whether the serial id of the android
                    device should be inserted in the adb command.
            timeout: it specifies the number of seconds to wait for
                     a synchronous invocation before aborting. If unspecified or
                     None it waits indefinitely for the command to complete.

        Raises:
            ValueError: it can be caused by any of the following situations:
                        - when both the combination async=True and timeout are
                          given.
                        - when a timeout <= 0 is specified.

        Returns:
            If adb was synchronously run and the command completed by the
            specified timeout, a string which is the output (standard out and
            error) from adb. Otherwise it returns None.
        '''

        # Form the command
        if device:
            cmd = '{0} -s {1} {2}'.format(self._path_adb, self.device, args)
        else:
            cmd = '{0} {1}'.format(self._path_adb, args)

        self._log.debug('Execute ADB: %s', cmd)

        if timeout is None:
            # local invocation
            return_code, output = UtilAndroid._execute_command_local(cmd, async)

        else:
            # remote invocation
            if async:
                raise ValueError('Invalid combination: asynchronous invocation '
                                 'with timeout specified')

            return_code, output = UtilAndroid._execute_command_remote(cmd,
                                                                      timeout)

            if return_code is None:
                self._log.warn('[ADB] The command timed out: %s', cmd)

        # log the output message
        if output is not None:
            self._adb_log_output(cmd, output, return_code)

        return output

    def adb_retry(self, args, max_num_attempts, timeout):
        '''Attempt to execute the given adb command a certain number of times.

        The function executes the given command through adb, waiting for its
        completion up to 'timeout' seconds. If the command completes then it
        returns its output. Otherwise it aborts the execution of the adb
        command and re-issues it anew with the same parameters. In case of
        timeout this process is repeated up to 'max_num_attempts'.

        The purpose of this function is to handle the cases when, for some
        reason, a command sent to 'adb' freezes, blocking the whole test suite
        indefinitely.

        Args:
            args: The command (including arguments) to run in adb.
            max_num_attempts: the max number of attempts to repeat the command
                              in case of timeout.
            timeout: it specifies the number of seconds to wait for the adb
                     command to complete.

        Raises:
            ValueError: when the parameter timeout is invalid (None or <= 0).

        Returns:
            If adb was synchronously run and the command completes by the
            specified timeout, a string which is the output (standard out and
            error) from adb. Otherwise it returns None.
        '''
        if timeout is None or timeout <= 0:
            raise ValueError('Invalid value for timeout')

        output = None

        for attempt in range(max_num_attempts):
            self._log.debug('[ADB] Attempt #%d: %s', attempt + 1, args)
            output = self.adb(args, False, True, timeout)
            if output:
                break

        return output

    def _adb_log_output(self, cmd, output, return_code):
        '''Save in the log the command & output from `adb`.

        Internal function, helper to record in the log the issued adb command
        together with its output and return code.

        Params:
            cmd: string, the command issued to `adb`.
            output: string, the output retrieved from `adb`.
            return_code: int, the return code from `adb`.
        '''

        message = output.strip()

        # if return_code != 0, we wish to also record the command executed
        # (which occurs if and only if we are in verbose mode)
        is_warning = return_code != 0
        threshold = self._log.getEffectiveLevel()
        if is_warning and threshold > logging.DEBUG:
            self._log.warn("[ADB] Command executed: {0}".format(cmd))

        level = logging.WARNING if is_warning else logging.DEBUG
        if message:
            # if message is composed by multiple lines, then print it after
            # the log preamble
            if re.search('\n', message):
                message = '\n' + message
        else:
            message = '<empty>'

        self._log.log(level, 'RC: {0}, Output: {1}'.format(return_code,
                                                           message))

    def check_adb_alive(self):
        '''Ping the device and raise an exception in case of timeout.

        It sends a ping message through 'adb shell'. The emulator/device should
        echo the same message back by one minute. If it does not, it raises
        a TestSuiteException.

        Purpose of this method is to check whether 'adb' became frozen or
        stuck.

        Raises:
            TestSuiteException: in case the device/emulator does not reply by
                                one minute or the `ping' message is not echoed
                                back.
        '''
        token = 'PING'
        log = util_log.get_logger()
        cmd = "echo {0}".format(token)

        tries = 10
        try_number = tries
        while try_number > 0:
            log.debug('Sending a ping through "adb shell" (try #%s)...',
                      try_number)
            output = self.shell(cmd, False, 60)

            if output is None:
                raise TestSuiteException(
                    'Timeout when pinging the device/emulator through '
                    '"adb shell".  Is "adb" stuck or dead?')
            elif token not in output:
                log.debug('Ping failed. Cannot match the token "%s" in "adb '
                          'shell %s"', token, cmd)
            else:
                log.debug('Pong message received')
                return

            try_number -= 1
            time.sleep(5)

        raise TestSuiteException('Cannot ping the device/emulator through '
                                 '"adb shell". Tried %s times. Is "adb" stuck '
                                 'or dead?' % tries)

    def shell(self, cmd, async=False, timeout=None):
        '''Run a command via the adb shell.

        Args:
            cmd: The command (including arguments) to run in the adb shell.
            async: Boolean to specify whether adb should run the command
                   asynchronously.
            timeout: it specifies the number of seconds to wait for
                     a synchronous invocation before aborting. If unspecified or
                     None it waits indefinitely for the command to complete

        Returns:
            If adb was synchronously run, a string which is the output (standard
            out and error) from adb. Otherwise None.
        '''
        return self.adb('shell "{0}"'.format(cmd), async, True, timeout)

    def find_app_pid(self, process_name):
        '''Find the process ID of a process with a given name.

        If more than one instance of the process is running return the first pid
        it finds.

        Args:
            process_name: A string representing the name of the package or
                          binary for which the id should be found. I.e. the
                          string or part of the string that shows up in the "ps"
                          command.

        Returns:
            An integer representing the id of the process, or None if it was not
            found.
        '''
        self._validate_string(process_name)

        pid_output = self.shell('pidof ' + process_name)
        pid_output = re.sub(r'\*.+\*', '', pid_output)
        pids = pid_output.split()

        if len(pids) < 1:
            self._log.warn('Unable to find pid of: {0}'.format(process_name))
            return None

        if len(pids) > 1:
            self._log.warn('Found multiple instances of {0} running: {1}'
                           .format(process_name, pids))

        try:
            pid = int(pids[0])
            self._log.info('App pid found: {0}'.format(pids[0]))
            return pid
        except ValueError:
            return None

    def adb_root(self):
        '''Set adb to be in root mode.'''
        self.adb('root')

    def _adb_remount(self):
        '''Remount the filesystem of the device.'''
        self.adb('remount')

    def validate_adb(self):
        '''Validate adb that it can be run.

        Raises:
            TestSuiteException: Unable to validate that adb exists and runs
                                successfully.
        '''
        out = self.adb('version', False, False)
        if out and 'Android' in out and 'version' in out:
            self._log.info('adb found: {0}'.format(out))
            return None
        raise TestSuiteException('unable to validate adb')

    def is_booted(self):
        ''' Check if the device/emulator has finished booting.

        Returns: True if the property sys.boot_completed is true, False
                 otherwise.
        '''
        return self._get_prop('sys.boot_completed').strip() == '1'

    def validate_device(self, check_boot=True, device_substring=''):
        '''Validate that there is at least one device.

        Args:
            check_boot: Boolean to specify whether to check whether the device
                        has finished booting as well as being present.
            device_substring: String that needs to be part of the name of the
                              device.

        Raises:
            TestSuiteException: There was a failure to run adb to list the
                                devices or there is no device connected or
                                multiple devices connected without the user
                                having specified the device to use.
        '''

        out = self.adb('devices', False, False)
        if not 'List of devices attached' in out:
            raise TestSuiteException('Unable to list devices')

        lines = out.split('\n')
        found_device = False # True if the specified device is found
        devices = []

        for line in lines[1:]:
            if '\tdevice' in line and device_substring in line:
                device = line.split()[0]
                devices.append(device)
                if self.device:
                    if self.device == device:
                        found_device = True

        if len(devices) == 0:
            raise TestSuiteException('adb is unable to find a connected '
                                     'device/emulator to test.')

        if not self.device:
            if len(devices) == 1:
                self.device = devices[0]
            else:
                raise TestSuiteException('Multiple devices connected,'
                                         'specify -d device id.')
        else:
            if not found_device:
                raise TestSuiteException('Couldn\'t find the device {0} that '
                                         'was specified, please check -d '
                                         'argument'.format(self.device))

        if check_boot and not self.is_booted():
            raise TestSuiteException(
                'The device {0} has not yet finished booting.'
                .format(self.device))

    def device_with_substring_exists(self, device_substring):
        '''Check whether a device exists whose name contains a given string.

        Args:
            device_substring: String that is part of the name of the device to
                              look for.

        Raises:
            TestSuiteException: There was a failure to run adb to list the
                                devices.
        '''
        out = self.adb('devices', False, False)
        if not 'List of devices attached' in out:
            raise TestSuiteException('Unable to list devices')

        lines = out.split('\n')

        for line in lines[1:]:
            if '\tdevice' in line:
                device = line.split()[0]
                if device.find(device_substring) != -1:
                    return True

        return False

    def get_device_id(self):
        '''Return ID of the device that will be used for running the tests on.

        Returns:
            String representing device ID.
        '''
        return self.device

    def _kill_pid(self, pid):
        '''Kill a process identified by its pid by issuing a "kill" command.

        Args:
            pid: The integer that is the process id of the process to be killed.
        '''
        self.shell('kill -9 ' + str(pid))

    def stop_app(self, package_name):
        '''Terminate an app by calling am force-stop.

        Args:
            package_name: The string representing the name of the package of the
                          app that is to be stopped.
        '''
        self._validate_string(package_name)
        self.shell('am force-stop ' + package_name)

    def kill_process(self, name):
        '''Kill a process identified by its name (package name in case of apk).

        Issues the "kill" command.

        Args:
            name: The string representing the name of the binary of the process
                  that is to be killed.

        Returns:
            True if the kill command was executed, False if it could not be
            found.
        '''
        pid = self.find_app_pid(name)
        if pid:
            self._kill_pid(pid)
            return True
        return False

    def kill_all_processes(self, name):
        '''Repeatedly try to call "kill" on a process to ensure it is gone.

        If the process is still there after 5 attempts reboot the device.

        Args:
            name: The string representing the name of the binary of the process
                  that is to be killed.

        Raises:
            TestSuiteException: If the process could not be killed after 5
                                attempts and the device then failed to boot
                                after rebooting.
        '''

        # try 5 times to kill this process
        for _ in range(1, 5):
            if not self.kill_process(name):
                return
        # stalled process must reboot
        self._reboot_device()

    def kill_servers(self):
        '''Kill all gdbserver and lldb-server instances.

        Raises:
            TestSuiteException: If gdbserver or lldb-server could not be killed
                                after 5 attempts and the device then failed to
                                boot after rebooting.
        '''
        self.kill_all_processes('gdbserver')
        self.kill_all_processes('lldb-server')

    def launch_elf(self, binary_name):
        '''Launch a binary (compiled with the NDK).

        Args:
            binary_name: The string representing the name of the binary that is
                         to be launched.

        Returns:
            Boolean, failure if the app is not installed, success otherwise.
        '''
        # Ensure the apk is actually installed.
        output = self.shell('ls /data/ | grep ' + binary_name)
        if binary_name not in output:
            return False

        stdout = self.shell('exec /data/' + binary_name, True)
        self._log.info(str(stdout))

        return True

    def wait_for_device(self):
        '''Ask ADB to wait for a device to become ready.'''
        self.adb('wait-for-device')

    def _reboot_device(self):
        '''Reboot the remote device.

        Raises:
            TestSuiteException: If the device failed to boot after rebooting.
        '''
        self.adb('reboot')
        self.wait_for_device()
        # Allow 20  mins boot time to give emulators such as MIPS enough time
        sleeping_countdown = 60*20
        while not self.is_booted():
            time.sleep(1)
            sleeping_countdown -= 1
            if sleeping_countdown == 0:
                raise TestSuiteException('Failed to reboot. Terminating.')

        self.adb_root()
        self.wait_for_device()
        self._adb_remount()
        self.wait_for_device()

    def launch_app(self, name, activity):
        '''Launch a Renderscript application.

        Args:
            name: The string representing the name of the app that is to be
                  launched.
            activity: The string representing the activity of the app that is to
                      be started.

        Returns:
            Boolean, failure if the apk is not installed, success otherwise.
        '''
        assert name and activity

        # Ensure the apk is actually installed.
        output = self.shell('pm list packages ' + name)
        if not output:
            return False

        cmd = 'am start -S -W {0}/{0}.{1}'.format(name, activity)
        stdout = self.shell(cmd)

        self._log.info(str(stdout))

        return True

    def launch_lldb_platform(self, port):
        '''Launch lldb server and attach to target app.

        Args:
            port: The integer that is the port on which lldb should listen.
        '''
        cmd = "export LLDB_DEBUGSERVER_PATH='{0}';{0} p --listen *:{1}"\
            .format(self._path_lldbserver, port)
        self.shell(cmd, True)
        time.sleep(5)

    def forward_port(self, local, remote):
        '''Use adb to forward a device port onto the local machine.

        Args:
            local: The integer that is the local port to forward.
            remote: The integer that is the remote port to which to forward.
        '''
        cmd = 'forward tcp:%s tcp:%s' % (str(local), str(remote))
        self.adb(cmd)

    def remove_port_forwarding(self):
        '''Remove all of the forward socket connections open in adb.

        Avoids a windows adb error where we can't bind to a listener
        because too many files are open.
        '''
        self.adb('forward --remove-all')

    def _get_prop(self, name):
        '''Get the value of an Android system property.

        Args:
            name: Name of the property of interest [string].

        Returns:
            Current value of the property [string].
        '''
        return self.shell('getprop %s' % str(name))

    def _set_prop(self, name, value):
        '''Set the value of an Android system property.

        Args:
            name: Name of the property of interest [string].
            value: Desired new value for the property [string or integer].
        '''
        self.shell("setprop %s '%s'" % (str(name), str(value)))

    def push_prop(self, name, new_value):
        '''Save the value of an Android system property and set a new value.

        Saves the old value onto a stack so it can be restored later.

        Args:
            name: Name of the property of interest [string].
            new_value: Desired new value for the property [string or integer].
        '''
        old_value = self._get_prop(name)
        self._set_prop(name, new_value)
        self._prop_stacks[name].append(old_value.strip())

    def pop_prop(self, name):
        '''Restore the value of an Android system property previously set by
        push_prop.

        Args:
            name: Name of the property of interest [string].

        Returns:
            Current value of the property [string].
        '''
        old_value = self._prop_stacks[name].pop()
        self._set_prop(name, old_value)

    def reset_all_props(self):
        '''Restore all the android properties to the state before the first push

        This is equivalent to popping each property the number of times it has
        been pushed.
        '''
        for name in self._prop_stacks:
            if self._prop_stacks[name] != []:
                self._set_prop(name, self._prop_stacks[name][0])
                self._prop_stacks[name] = []

    def make_device_writeable(self):
        ''' Ensure the device is full writable, in particular the system folder.

        This disables verity and remounts.
        '''
        output = self.adb('disable-verity')

        # if the remote is an emulator do not even try to reboot
        # otherwise check whether a reboot is advised
        if (self._get_prop('ro.kernel.qemu') != '1' and output and
                'Now reboot your device for settings to take effect' in output):
            self._reboot_device()

        self._adb_remount()
        self.wait_for_device()
        self.adb_root()
        self.wait_for_device()

    @staticmethod
    def _execute_command_local(command, async=False):
        '''Execute the given shell command in the same process.

        Args:
            command: String, the command to execute
            async: Boolean to specify whether adb should run the command
                   asynchronously.

        Returns:
            if async == False, it returns a tuple with the return code and
            the output from the executed command. Otherwise the tuple
            (None, None).
        '''
        proc = subprocess.Popen(command,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT,
                                shell=True)
        if async:
            return None, None

        # read the whole output from the command
        with proc.stdout as file_proc:
            output = ''.join(line for line in file_proc)

        # release the process state
        proc.terminate()
        return_code = proc.wait()

        return return_code, output

    @staticmethod
    def _execute_command_remote(command, timeout):
        '''Execute the given shell command remotely, in a separate process.

        It spawns an ad hoc process to execute the given command. It waits up
        to timeout for the command to complete, otherwise it aborts the
        execution and returns None.

        Args:
            command: String, the command to execute.
            timeout: the number of seconds to wait for the command to complete.

        Returns:
            a pair with the return code and the output from the command, if it
            completed by the specified 'timeout' seconds. Otherwise the tuple
            (None, None).
        '''

        channel = multiprocessing.Queue()
        proc = multiprocessing.Process(
            target=_handle_remote_request,
            name="Executor of `{0}'".format(command),
            args=(command, channel)
        )

        # execute the command
        proc.start()
        return_code = None
        output = None

        # wait for the result
        try:
            return_code, output = channel.get(True, timeout)
        except queue.Empty:
            # timeout hit, the remote process has not fulfilled our request by
            # the given time. We are going to return <None, None>, nothing to
            # do here as it already holds return_code = output = None.
            pass

        # terminate the helper process
        proc.terminate()

        return return_code, output


def _handle_remote_request(command, channel):
    '''Entry point for the remote process.

    It executes the given command and reports the result into the channel.
    This function is supposed to be only called by
    UtilAndroid._execute_command_remote to handle the inter-process
    communication.

    Args:
        command: the command to execute.
        channel: the channel to communicate with the caller process.
    '''
    channel.put(UtilAndroid._execute_command_local(command))

