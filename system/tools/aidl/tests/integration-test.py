#!/usr/bin/env python

"""
Test that aidl generates functional code by running it on an Android device.
"""

import argparse
import pipes
import subprocess
import shlex

JAVA_OUTPUT_READER = 'aidl_test_sentinel_searcher'
NATIVE_TEST_CLIENT = 'aidl_test_client'
NATIVE_TEST_SERVICE = 'aidl_test_service'

TEST_FILTER_ALL = 'all'
TEST_FILTER_JAVA = 'java'
TEST_FILTER_NATIVE = 'native'

JAVA_CLIENT_TIMEOUT_SECONDS = 30
JAVA_LOG_FILE = '/data/data/android.aidl.tests/files/test-client.log'
JAVA_SUCCESS_SENTINEL = '>>> Java Client Success <<<'
JAVA_FAILURE_SENTINEL = '>>> Java Client Failure <<<'

class TestFail(Exception):
    """Raised on test failures."""
    pass


class ShellResult(object):
    """Represents the result of running a shell command."""

    def __init__(self, exit_status, stdout, stderr):
        """Construct an instance.

        Args:
            exit_status: integer exit code of shell command
            stdout: string stdout of shell command
            stderr: string stderr of shell command
        """
        self.stdout = stdout
        self.stderr = stderr
        self.exit_status = exit_status

    def printable_string(self):
        """Get a string we could print to the logs and understand."""
        output = []
        output.append('stdout:')
        for line in self.stdout.splitlines():
            output.append('  > %s' % line)
        output.append('stderr:')
        for line in self.stderr.splitlines():
            output.append('  > %s' % line)
        return '\n'.join(output)


class AdbHost(object):
    """Represents a device connected via ADB."""

    def __init__(self, device_serial=None, verbose=None):
        """Construct an instance.

        Args:
            device_serial: options string serial number of attached device.
            verbose: True iff we should print out ADB commands we run.
        """
        self._device_serial = device_serial
        self._verbose = verbose

    def run(self, command, background=False, ignore_status=False):
        """Run a command on the device via adb shell.

        Args:
            command: string containing a shell command to run.
            background: True iff we should run this command in the background.
            ignore_status: True iff we should ignore the command's exit code.

        Returns:
            instance of ShellResult.

        Raises:
            subprocess.CalledProcessError on command exit != 0.
        """
        if background:
            command = '( %s ) </dev/null >/dev/null 2>&1 &' % command
        return self.adb('shell %s' % pipes.quote(command),
                        ignore_status=ignore_status)

    def mktemp(self):
        """Make a temp file on the device.

        Returns:
            path to created file as a string

        Raises:
            subprocess.CalledProcessError on failure.
        """
        # Work around b/19635681
        result = self.run('source /system/etc/mkshrc && mktemp')
        return result.stdout.strip()

    def adb(self, command, ignore_status=False):
        """Run an ADB command (e.g. `adb sync`).

        Args:
            command: string containing command to run
            ignore_status: True iff we should ignore the command's exit code.

        Returns:
            instance of ShellResult.

        Raises:
            subprocess.CalledProcessError on command exit != 0.
        """
        command = 'adb %s' % command
        if self._verbose:
            print(command)
        p = subprocess.Popen(command, shell=True, close_fds=True,
                             stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                             universal_newlines=True)
        stdout, stderr = p.communicate()
        if not ignore_status and p.returncode:
            raise subprocess.CalledProcessError(p.returncode, command)
        return ShellResult(p.returncode, stdout, stderr)


def run_test(host, test_native, test_java):
    """Body of the test.

    Args:
        host: AdbHost object to run tests on
        test_native: True iff we should test native Binder clients.
        test_java: True iff we shoudl test Java Binder clients.
    """

    print('Starting aidl integration testing...')

    # Kill any previous test context
    host.run('rm -f %s' % JAVA_LOG_FILE, ignore_status=True)
    host.run('pkill %s' % NATIVE_TEST_SERVICE, ignore_status=True)

    # Start up a native server
    host.run(NATIVE_TEST_SERVICE, background=True)

    # Start up clients
    if test_native:
        host.run('pkill %s' % NATIVE_TEST_CLIENT, ignore_status=True)
        result = host.run(NATIVE_TEST_CLIENT, ignore_status=True)
        if result.exit_status:
            print(result.printable_string())
            raise TestFail('%s returned status code %d' %
                           (NATIVE_TEST_CLIENT, result.exit_status))

    if test_java:
        host.run('am start -S -a android.intent.action.MAIN '
                 '-n android.aidl.tests/.TestServiceClient '
                 '--es sentinel.success "%s" '
                 '--es sentinel.failure "%s"' %
                 (JAVA_SUCCESS_SENTINEL, JAVA_FAILURE_SENTINEL))
        result = host.run('%s %d %s "%s" "%s"' %
                          (JAVA_OUTPUT_READER, JAVA_CLIENT_TIMEOUT_SECONDS,
                           JAVA_LOG_FILE, JAVA_SUCCESS_SENTINEL,
                           JAVA_FAILURE_SENTINEL),
                          ignore_status=True)
        if result.exit_status:
            print(result.printable_string())
            raise TestFail('Java client did not complete successfully.')

    print('Success!')


def main():
    """Main entry point."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
            '--test-filter', default=TEST_FILTER_ALL,
            choices=[TEST_FILTER_ALL, TEST_FILTER_JAVA, TEST_FILTER_NATIVE])
    parser.add_argument('--verbose', '-v', action='store_true', default=False)
    args = parser.parse_args()
    host = AdbHost(verbose=args.verbose)
    try:
        # Tragically, SELinux interferes with our testing
        host.run('setenforce 0')
        run_test(host,
                 args.test_filter in (TEST_FILTER_ALL, TEST_FILTER_NATIVE),
                 args.test_filter in (TEST_FILTER_ALL, TEST_FILTER_JAVA))
    finally:
        host.run('setenforce 1')


if __name__ == '__main__':
    main()
