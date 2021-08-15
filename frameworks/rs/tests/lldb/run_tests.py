#!/usr/bin/env python

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

'''Main test suite execution script.'''
import argparse
import inspect
import logging
import os
import signal
import subprocess
import sys
import time
import collections
import xml.etree.ElementTree as ET

from config import Config
from tests.harness import util_constants
from tests.harness.exception import TestSuiteException, FailFastException
from tests.harness import UtilAndroid
from tests.harness import UtilBundle
from tests.harness import util_log
from tests.harness.util_functions import load_py_module
from tests.harness.decorators import deprecated

# For some reason pylint is not able to understand the class returned by
# from util_log.get_logger() and generates a lot of false warnings
#pylint: disable=maybe-no-member

EMU_PROC = None

def _parse_args():
    '''Parse the command line arguments.

    Returns:
        A namespace object that contains the options specified to run_tests on
        the command line.
    '''

    parser = argparse.ArgumentParser(description='Run the test suite.')

    parser.add_argument('--config', '-c',
                        metavar='path',
                        help='Path to a custom config file.')
    parser.add_argument('--device', '-d',
                        help='Specify the device id of the device to test on.')
    parser.add_argument('--test', '-t',
                        metavar='path',
                        help='Specify a specific test to run.')
    group = parser.add_mutually_exclusive_group()
    group.add_argument('--wimpy', '-w',
                        action='store_true',
                        default=None,
                        help='Test only a core subset of features.')
    group.add_argument('--app-types',
                        default=['java', 'cpp', 'jni'],
                        nargs='*',
                        help='Specify a list of Android app types against which'
                             ' to run the tests',
                        dest='bundle_types')
    parser.add_argument('--install-only',
                        action='store_true',
                        default=False,
                        help='It only runs the pre-run stage of the test suite.'
                             ' It installs the required APKs but does not '
                             'execute the tests.',
                        dest='install_only')
    parser.add_argument('--no-install', '-n',
                        action='store_true',
                        default=False,
                        help='Stop the test suite installing apks to device.',
                        dest='noinstall')
    parser.add_argument('--no-uninstall',
                        action='store_true',
                        default=False,
                        help='Stop the test suite uninstalling apks after '
                             'completion.',
                        dest='nouninstall')
    parser.add_argument('--print-to-stdout',
                        action='store_true',
                        default=False,
                        help='Print all logging information to standard out.',
                        dest='print_to_stdout')
    parser.add_argument('--verbose', '-v',
                        action='store_true',
                        default=None,
                        help='Store extra info in the log.')
    parser.add_argument('--fail-fast',
                        action='store_true',
                        default=False,
                        help='Exit the test suite immediately on the first failure.')
    parser.add_argument('--run-emu',
                        action='store_true',
                        default=None,
                        help='Spawn an emulator and run the test suite on that.'
                             ' Specify the emulator command line in the config'
                             ' file or with -emu-cmd.',
                        dest='run_emu')

    # Get the properties of the Config class and add a command line argument
    # for each.
    this_module = sys.modules[__name__]
    for member_name, member_obj in inspect.getmembers(Config):
        if (inspect.isdatadescriptor(member_obj) and
            member_name not in ['__weakref__', 'device', 'verbose']):

            # List type properties can take one or more arguments
            num_args = None
            if (isinstance(member_obj, property)
                and isinstance(member_obj.fget(Config), list)):
                num_args = '+'

            opt_name = member_name.replace('_', '-')

            setattr(this_module, opt_name, '')

            parser.add_argument('--' + opt_name,
                                nargs=num_args,
                                help=member_obj.__doc__,
                                dest=member_name)

    return parser.parse_args()


def _choice(first_choice, second_choice):
    '''Return first_choice if it is not None otherwise return second_choice.

    Args:
        first_choice: The first choice value.
        second_choice: The alternative value.

    Returns:
        The first argument if it is not None, and the second otherwise.
    '''
    return first_choice if first_choice else second_choice


class State(object):
    '''This class manages all objects required by the test suite.'''

    # pylint: disable=too-many-instance-attributes
    # Since this is a state class many attributes are expected.

    def __init__(self):
        '''State constructor.

        Raises:
            TestSuiteException: When unable to load config file.

            AssertionError: When assertions fail.
        '''

        # Parse the command line options
        args = _parse_args()

        # create a config instance
        if args.config:
            # use the user supplied
            config = State.load_user_configuration(args.config)
        else:
            # use the default configuration
            config = Config()

        # save the test blacklist
        self.blacklist = _choice(args.blacklist, config.blacklist)

        # Allow any of the command line arguments to override the
        # values in the config file.
        self.adb_path = _choice(args.adb_path, config.adb_path)

        self.host_port = int(_choice(args.host_port, config.host_port))

        self.device = _choice(args.device, config.device)

        self.user_specified_device = self.device

        self.device_port = int(_choice(args.device_port, config.device_port))

        self.lldb_server_path_device = _choice(args.lldb_server_path_device,
                                               config.lldb_server_path_device)

        self.lldb_server_path_host = _choice(args.lldb_server_path_host,
                                             config.lldb_server_path_host)

        self.aosp_product_path = _choice(args.aosp_product_path,
                                         config.aosp_product_path)

        self.log_file_path = _choice(args.log_file_path, config.log_file_path)

        self.results_file_path = _choice(args.results_file_path,
                                         config.results_file_path)

        self.lldb_path = _choice(args.lldb_path, config.lldb_path)
        self.print_to_stdout = args.print_to_stdout
        self.verbose = _choice(args.verbose, config.verbose)
        self.timeout = int(_choice(args.timeout, config.timeout))
        self.emu_cmd = _choice(args.emu_cmd, config.emu_cmd)
        self.run_emu = args.run_emu
        self.wimpy = args.wimpy
        self.bundle_types = args.bundle_types if not self.wimpy else ['java']
        self.fail_fast = args.fail_fast

        # validate the param "verbose"
        if not isinstance(self.verbose, bool):
            raise TestSuiteException('The parameter "verbose" should be a '
                                     'boolean: {0}'.format(self.verbose))

        # create result array
        self.results = dict()
        self.single_test = args.test

        # initialise the logging facility
        log_level = logging.INFO if not self.verbose else logging.DEBUG
        util_log.initialise("driver",
                            print_to_stdout=self.print_to_stdout,
                            level=log_level,
                            file_mode='w', # open for write
                            file_path=self.log_file_path
                            )
        log = util_log.get_logger()

        if self.run_emu and not self.emu_cmd:
            log.TestSuiteException('Need to specify --emu-cmd (or specify a'
                ' value in the config file) if using --run-emu.')

        # create a results file
        self.results_file = open(self.results_file_path, 'w')

        # create an android helper object
        self.android = UtilAndroid(self.adb_path,
                                   self.lldb_server_path_device,
                                   self.device)
        assert self.android

        # create a test bundle
        self.bundle = UtilBundle(self.android,
                                 self.aosp_product_path)
        assert self.bundle

        # save the no pushing option
        assert isinstance(args.noinstall, bool)
        self.noinstall = args.noinstall

        assert isinstance(args.nouninstall, bool)
        self.nouninstall = args.nouninstall

        # install only option
        assert type(args.install_only) is bool
        self.install_only = args.install_only
        if self.install_only:
            log.log_and_print('Option --install-only set. The test APKs will '
                              'be installed on the device but the tests will '
                              'not be executed.')
            if self.noinstall:
                raise TestSuiteException('Conflicting options given: '
                                         '--install-only and --no-install')

        # TCP port modifier which is used to increment the port number used for
        # each test case to avoid collisions.
        self.port_mod = 0

        # total number of test files that have been executed
        self.test_count = 0

    def get_android(self):
        '''Return the android ADB helper instance.

        Returns:
            The android ADB helper, instance of UtilAndroid.
        '''
        assert self.android
        return self.android

    def get_bundle(self):
        '''Return the test executable bundle.

        Returns:
            The test exectable collection, instance of UtilBundle.
        '''
        return self.bundle

    def add_result(self, name, app_type, result):
        '''Add a test result to the collection.

        Args:
            name: String name of the test that has executed.
            app_type: type of app i.e. java, jni, or cpp
            result: String result of the test, "pass", "fail", "error".
        '''
        key = (name, app_type)
        assert key not in self.results
        self.results[key] = result

    def get_single_test(self):
        '''Get the name of the single test to run.

        Returns:
            A string that is the name of the python file containing the test to
            be run. If all tests are to be run this returns None.
        '''
        return self.single_test

    @staticmethod
    def load_user_configuration(path):
        '''Load the test suite config from the give path.

        Instantiate the Config class found in the module at the given path.
        If no suitable class is available, it raises a TestSuiteException.

        Args:
            path: String location of the module.

        Returns:
            an instance of the Config class, defined in the module.

        Raises:
            TestSuiteException: when unable to import the module or when a
                                subclass of Config is not found inside it.
        '''

        # load the module
        config_module = load_py_module(path)
        if not config_module:
            raise TestSuiteException('Unable to import the module from "%s"'
                                     % (path))

        # look for a subclass of Config
        for name, value in inspect.getmembers(config_module):
            if (inspect.isclass(value)
                and name != 'Config'
                and issubclass(value, Config)):
                # that's our candidate
                return value()

        # otherwise there are no valid candidates
        raise TestSuiteException('The provided user configuration is not '
                                 'valid. The module must define a subclass '
                                 'of Config')


def _kill_emulator():
    ''' Kill the emulator process. '''
    global EMU_PROC
    if EMU_PROC:
        try:
            EMU_PROC.terminate()
        except OSError:
            # can't kill a dead proc
            log = util_log.get_logger()
            log.debug('Trying to kill an emulator but it is already dead.')


def _check_emulator_terminated():
    ''' Throw an exception if the emulator process has ended.

    Raises:
        TestSuiteException: If the emulator process has ended.
    '''
    global EMU_PROC
    assert EMU_PROC
    if EMU_PROC.poll():
        stdout, stderr = EMU_PROC.communicate()
        raise TestSuiteException('The emulator terminated with output:'
            '\nstderr: {0}\nstdout: {1}.'.format(stderr, stdout))


@deprecated()
def _launch_emulator(state):
    '''Launch the emulator and wait for it to boot.

    Args:
        emu_cmd: The command line to run the emulator.

    Raises:
        TestSuiteException: If an emulator already exists or the emulator
                            process terminated before we could connect to it, or
                            we failed to copy lldb-server to the emulator.
    '''
    global EMU_PROC
    android = state.android
    if state.user_specified_device:
        if android.device_with_substring_exists(state.user_specified_device):
            raise TestSuiteException(
                'A device with name {0} already exists.',
                state.user_specified_device)
    else:
        if android.device_with_substring_exists('emulator'):
            raise TestSuiteException('An emulator already exists.')

    assert state.emu_cmd
    EMU_PROC = subprocess.Popen(state.emu_cmd.split(),
                                stdout=None,
                                stderr=subprocess.STDOUT)

    log = util_log.get_logger()
    log.info('Launching emulator with command line {0}'.format(state.emu_cmd))

    tries_number = 180
    tries = tries_number
    found_device = False
    while not found_device:
        try:
            android.validate_device(False, 'emulator')
            found_device = True
        except TestSuiteException as ex:
            tries -= 1
            if tries == 0:
                # Avoid infinitely looping if the emulator won't boot
                log.warning(
                    'Giving up trying to validate device after {0} tries.'
                    .format(tries_number))
                raise ex
            _check_emulator_terminated()
            # wait a bit and try again, maybe it has now booted
            time.sleep(10)

    tries = 500
    while not android.is_booted():
        tries -= 1
        if tries == 0:
            # Avoid infinitely looping if the emulator won't boot
            raise TestSuiteException('The emulator has failed to boot.')
        _check_emulator_terminated()
        time.sleep(5)

    # Need to be root before we can push lldb-server
    android.adb_root()
    android.wait_for_device()

    # Push the lldb-server executable to the device.
    output = android.adb('push {0} {1}'.format(state.lldb_server_path_host,
                                               state.lldb_server_path_device))

    if 'failed to copy' in output or 'No such file or directory' in output:
        raise TestSuiteException(
            'unable to push lldb-server to the emulator: {0}.'
            .format(output))

    output = android.shell('chmod a+x {0}'
                           .format(state.lldb_server_path_device))

    if 'No such file or directory' in output:
        raise TestSuiteException('Failed to copy lldb-server to the emulator.')


def _restart_emulator(state):
    '''Kill the emulator and start a new instance.

    Args:
        state: Test suite state collection, instance of State.
    '''
    _kill_emulator()
    _launch_emulator(state)


def _run_test(state, name, bundle_type):
    '''Execute a single test case.

    Args:
        state: Test suite state collection, instance of State.
        name: String file name of the test to execute.
        bundle_type: string for the installed app type (cpp|jni|java)

    Raises:
        AssertionError: When assertion fails.
    '''
    assert isinstance(name, str)

    try:
        state.android.check_adb_alive()
    except TestSuiteException as expt:
        global EMU_PROC
        if EMU_PROC:
            _restart_emulator(state)
        else:
            raise expt

    log = util_log.get_logger()
    sys.stdout.write('Running {0}\r'.format(name))
    sys.stdout.flush()
    log.info('Running {0}'.format(name))

    run_tests_dir = os.path.dirname(os.path.realpath(__file__))
    run_test_path = os.path.join(run_tests_dir, 'tests', 'run_test.py')

    # Forward port for lldb-server on the device to our host
    hport = int(state.host_port) + state.port_mod
    dport = int(state.device_port) + state.port_mod
    state.android.forward_port(hport, dport)
    state.port_mod += 1

    log.debug('Giving up control to {0}...'.format(name))

    params = map(str, [
        sys.executable,
        run_test_path,
        name,
        state.log_file_path,
        state.adb_path,
        state.lldb_server_path_device,
        state.aosp_product_path,
        dport,
        state.android.get_device_id(),
        state.print_to_stdout,
        state.verbose,
        state.wimpy,
        state.timeout,
        bundle_type
    ])

    return_code = subprocess.call(params)
    state.test_count += 1
    state.android.remove_port_forwarding()
    log.seek_to_end()

    # report in sys.stdout the result
    success = return_code == util_constants.RC_TEST_OK
    status_handlers = collections.defaultdict(lambda: ('error', log.error), (
            (util_constants.RC_TEST_OK, ('pass', log.info)),
            (util_constants.RC_TEST_TIMEOUT, ('timeout', log.error)),
            (util_constants.RC_TEST_IGNORED, ('ignored', log.info)),
            (util_constants.RC_TEST_FAIL, ('fail', log.critical))
        )
    )
    status_name, status_logger = status_handlers[return_code]
    log.info('Running %s: %s', name, status_name.upper())
    status_logger("Test %r: %s", name, status_name)

    # Special case for ignored tests - just return now
    if return_code == util_constants.RC_TEST_IGNORED:
        return

    state.add_result(name, bundle_type, status_name)

    if state.fail_fast and not success:
        raise FailFastException(name)

    # print a running total pass rate
    passes = sum(1 for key, value in state.results.items() if value == 'pass')
    log.info('Current pass rate: %s of %s executed.', passes, len(state.results))


def _check_lldbserver_exists(state):
    '''Check lldb-server exists on the target device and it is executable.

    Raises:
        TestSuiteError: If lldb-server does not exist on the target.
    '''
    assert state

    message = 'Unable to verify valid lldb-server on target'

    android = state.get_android()
    assert android

    cmd = state.lldb_server_path_device
    out = android.shell(cmd, False)
    if not isinstance(out, str):
        raise TestSuiteException(message)
    if out.find('Usage:') < 0:
        raise TestSuiteException(message)


def _suite_pre_run(state):
    '''This function is executed before the test cases are run (setup).

    Args:
        state: Test suite state collection, instance of State.

    Return:
        True if the pre_run step completes without error.
        Checks made:
            - Validating that adb exists and runs.
            - Validating that a device is attached.
            - We have root access to the device.
            - All test binaries were pushed to the device.
            - The port for lldb-server was forwarded correctly.

    Raises:
        AssertionError: When assertions fail.
    '''
    assert state
    log = util_log.get_logger()

    try:
        android = state.get_android()
        bundle = state.get_bundle()
        assert android
        assert bundle

        # validate ADB helper class
        android.validate_adb()
        log.log_and_print('Located ADB')

        if state.run_emu:
            log.log_and_print('Launching emulator...')
            _launch_emulator(state)
            log.log_and_print('Started emulator ' + android.device)
        else:
            android.validate_device()
            log.log_and_print('Located device ' + android.device)

        if state.noinstall and not state.single_test:
            bundle.check_apps_installed(state.wimpy)

        # elevate to root user
        android.adb_root()
        android.wait_for_device()
        # check that lldb-server exists on device
        android.kill_servers()
        _check_lldbserver_exists(state)

        if not state.noinstall:
            # push all tests to the device
            log.log_and_print('Pushing all tests...')
            bundle.push_all()
            log.log_and_print('Pushed all tests')
        log.log_and_print('Pre run complete')

    except TestSuiteException as expt:
        log.exception('Test suite pre run failure')

        # Even if we are logging the error, it may be helpful and more
        # immediate to find out the error into the terminal
        log.log_and_print('ERROR: Unable to set up the test suite: %s\n'
                          % expt.message, logging.ERROR)

        return False
    return True


def _suite_post_run(state):
    '''This function is executed after the test cases have run (teardown).

    Args:
        state: Test suite state collection, instance of State.
    Returns:
        Number of failures
    '''
    log = util_log.get_logger()

    if not state.noinstall and not state.nouninstall:
        if state.wimpy:
            state.bundle.uninstall_all_apk()
        else:
            state.bundle.uninstall_all()
        log.log_and_print('Uninstalled/Deleted all tests')

    total = 0
    passes = 0
    failures = 0

    results = ET.Element('testsuite')
    results.attrib['name'] = 'LLDB RS Test Suite'

    for key, value in state.results.items():
        total += 1
        if value == 'pass':
            passes += 1
        else:
            failures += 1

        # test case name, followed by pass, failure or error elements
        testcase = ET.Element('testcase')
        testcase.attrib['name'] = "%s:%s" % key
        result_element = ET.Element(value)
        result_element.text = "%s:%s" % key
        testcase.append(result_element)
        results.append(testcase)

    assert passes + failures == total, 'Invalid test results status'
    if failures:
        log.log_and_print(
            'The following failures occurred:\n%s\n' %
            '\n'.join('failed: %s:%s' % test_spec
                for test_spec, result in state.results.items() if result != 'pass'
        ))

    log.log_and_print('{0} of {1} passed'.format(passes, total))
    if total:
        log.log_and_print('{0}% rate'.format((passes*100)/total))

    results.attrib['tests'] = str(total)
    state.results_file.write(ET.tostring(results, encoding='iso-8859-1'))

    return failures


def _discover_tests(state):
    '''Discover all tests in the tests directory.

    Returns:
        List of strings, test file names from the 'tests' directory.
    '''
    tests = []

    single_test = state.get_single_test()
    if single_test is None:
        file_dir = os.path.dirname(os.path.realpath(__file__))
        tests_dir = os.path.join(file_dir, 'tests')

        for sub_dir in os.listdir(tests_dir):
            current_test_dir = os.path.join(tests_dir, sub_dir)
            if os.path.isdir(current_test_dir):
                dir_name = os.path.basename(current_test_dir)

                if dir_name == 'harness':
                    continue

                for item in os.listdir(current_test_dir):
                    if (item.startswith('test')
                        and item.endswith('.py')
                        and not item in state.blacklist):
                        tests.append(item)
    else:
        if single_test.endswith('.py'):
            tests.append(single_test)
        else:
            tests.append(single_test + '.py')

    return tests


def _deduce_python_path(state):
    '''Try to deduce the PYTHONPATH environment variable via the LLDB binary.

    Args:
        state: Test suite state collection, instance of State.

    Returns:
        True if PYTHONPATH has been updated, False otherwise.

    Raises:
        TestSuiteException: If lldb path provided in the config or command line
                            is incorrect.
        AssertionError: If an assertion fails.
    '''

    lldb_path = state.lldb_path
    if not lldb_path:
        # lldb may not be provided in preference of a manual $PYTHONPATH
        return False

    params = [lldb_path, '-P']

    try:
        proc = subprocess.Popen(params, stdout=subprocess.PIPE)
    except OSError as err:
        error_string = 'Could not run lldb at %s: %s' % (lldb_path, str(err))
        raise TestSuiteException(error_string)

    stdout = proc.communicate()[0]
    if stdout:
        os.environ['PYTHONPATH'] = stdout.strip()
        return True

    return False


def main():
    '''The lldb-renderscript test suite entry point.'''
    log = None

    try:
        # parse the command line
        state = State()
        assert state

        # logging is initialised in State()
        log = util_log.get_logger()

        # if we can, set PYTHONPATH for lldb bindings
        if not _deduce_python_path(state):
            log.log_and_print('Unable to deduce PYTHONPATH', logging.WARN)

        # pre run step
        if not _suite_pre_run(state):
            raise TestSuiteException('Test suite pre-run step failed')
        # discover all tests and execute them
        tests = _discover_tests(state)
        log.log_and_print('Found {0} tests'.format(len(tests)))
        if state.install_only:
            log.log_and_print('Test applications installed. Terminating due to '
                              '--install-only option')
        else:
            # run the tests
            for bundle_type in state.bundle_types:
                log.info("Running bundle type '%s'", bundle_type)
                for item in tests:
                    _run_test(state, item, bundle_type)
                # post run step
            quit(0 if _suite_post_run(state) == 0 else 1)

    except AssertionError:
        if log:
            log.exception('Internal test suite error')

        print('Internal test suite error')
        quit(1)

    except FailFastException:
        log.exception('Early exit after first test failure')
        quit(1)

    except TestSuiteException as error:
        if log:
            log.exception('Test suite exception')

        print('{0}'.format(str(error)))
        quit(2)

    finally:
        _kill_emulator()
        logging.shutdown()

def signal_handler(_, _unused):
    '''Signal handler for SIGINT, caused by the user typing Ctrl-C.'''
    # pylint: disable=unused-argument
    # pylint: disable=protected-access
    print('Ctrl+C!')
    os._exit(1)


# execution trampoline
if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    main()
