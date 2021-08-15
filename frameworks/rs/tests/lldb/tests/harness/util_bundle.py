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

'''Module that contains the class UtilBundle, representing a collection of RS
binaries.'''

from __future__ import absolute_import

import os
import time
from . import util_constants
from . import util_log
from .exception import TestSuiteException


class UtilBundle(object):
    '''Represents the collection of RS binaries that are debugged.'''

    # Map of binary name to package name of all Java apps debugged
    _tests_apk = {
        'JavaInfiniteLoop': 'com.android.rs.infiniteloop',
        'JavaDebugWaitAttach': 'com.android.rs.waitattachdebug',
        'JavaNoDebugWaitAttach': 'com.android.rs.waitattachnodebug',
        'BranchingFunCalls': 'com.android.rs.branchingfuncalls',
        'KernelVariables': 'com.android.rs.kernelvariables',
        'Allocations': 'com.android.rs.allocations',
        'MultipleRSFiles': 'com.android.rs.multiplersfiles',
        'SingleSource': 'com.android.rs.singlesource',
        'ScriptGroup': 'com.android.rs.scriptgroup',
        'Reduction': 'com.android.rs.lldbreductiontest',
    }

    _tests_jni = {
        'JNIInfiniteLoop': 'com.android.rs.jniinfiniteloop',
        'JNIDebugWaitAttach': 'com.android.rs.jnidebugwaitattach',
        'JNINoDebugWaitAttach': 'com.android.rs.jninodebugwaitattach',
        'JNIBranchingFunCalls': 'com.android.rs.jnibranchingfuncalls',
        'JNIKernelVariables': 'com.android.rs.jnikernelvariables',
        'JNIAllocations': 'com.android.rs.jniallocations',
        'JNIMultipleRSFiles': 'com.android.rs.jnimultiplersfiles'
    }

    _tests_ndk = {'CppInfiniteLoop', 'CppNoDebugWaitAttach',
                  'CppDebugWaitAttach', 'CppBranchingFunCalls',
                  'CppKernelVariables', 'CppAllocations', 'CppMultipleRSFiles'}

    _missing_path_msg = (
        'No product path has been provided. If using `lunch` ensure '
        'the `ANDROID_PRODUCT_OUT` environment variable has been set correctly. '
        'Alternatively, include it in the config file or specify it explicitly '
        'on the command line (`--aosp-product-path`)'
    )

    def __init__(self, android, aosp_product_path):
        assert android
        self._android = android # Link to the android module
        self._aosp_product_path = aosp_product_path
        self._log = util_log.get_logger()

    def is_apk(self, name):
        '''Checks if a binary of a given name is an apk.

        Checks whether the name of the apk is in the dictionary of apks.

        Args:
            name: The string that is the name of the binary to check.

        Returns:
            True if the binary is an apk, False if it is not.

        Raises:
            TestSuiteException: The string does not match any item in the list
            of APK or NDK binaries.
        '''
        if name in self._tests_apk:
            return True
        if name not in self._tests_ndk and name not in self._tests_jni:
            raise TestSuiteException('test not apk or ndk')
        return False

    def uninstall_all(self):
        '''Uninstall/Delete all the testsuite's apks and binaries on the device.

        Raises:
            TestSuiteException: One or more apks could not be uninstalled.
        '''
        self.uninstall_all_apk()
        self._delete_all_ndk()
        self._uninstall_all_jni()

    def uninstall_all_apk(self):
        '''Uninstall all apks used by the test suite from the device.

        Raises:
            TestSuiteException: An apk could not be uninstalled.
        '''
        max_num_attempts = 3
        timeout = 180

        for app, package in self._tests_apk.items():
            self._log.info('Uninstalling the application: %s', app)
            output = self._android.adb_retry('uninstall ' + package,
                                             max_num_attempts, timeout)

            if output is None:
                raise TestSuiteException('Repeated timeouts when uninstalling '
                                         'the application: ' + app)
            elif 'Success' not in output:
                outmsg = '\n' + output.rstrip() if output else '<empty>'
                self._log.error('Cannot match the string "Success" in the '
                                'output: %s', outmsg)
                raise TestSuiteException('Unable to uninstall app ' + app)
            else:
                self._log.debug('Application uninstalled: %r', app)

            if 'Success' not in output:
                self._log.warning('unable to uninstall app ' + app)

    def _uninstall_all_jni(self):
        '''Uninstall all apks used by the test suite from the device.

        Raises:
            TestSuiteException: An apk could not be uninstalled.
        '''
        for app, package in self._tests_jni.items():
            output = self._android.adb('uninstall ' + package)

            if 'Success' not in output:
                raise TestSuiteException('unable to uninstall app ' + app)

    def _delete_all_ndk(self):
        '''Delete all ndk binaries that were pushed to the device.

        Raises:
            TestSuiteException: A binary could not be deleted from the device.
        '''
        for app in self._tests_ndk:
            output = self._android.shell('rm /data/' + app)
            if 'No such file or directory' in output:
                self._log.warning('unable to uninstall app ' + app)


    def push_all(self):
        '''Push all apk and ndk binaries required by the testsuite to the device

        Raises:
            TestSuiteException: One or more apks could not be installed or
                                previously running processes thereof could not
                                be killed.
        '''
        self._push_all_java()
        self._push_all_ndk()
        self._push_all_jni()

    def _install_apk(self, app, package):
        '''Push an apk files to the device.

        This involves uninstalling any old installation and installing again.

        Args:
            app: A string that is the name of the apk.
            package: A string that is the name of the package of the apk.

        Raises:
            TestSuiteException: The apk could not be installed.
        '''
        self._log.info('pushing {0}'.format(app))

        self._android.stop_app(package)

        self._android.adb('uninstall ' + package)
        # Ignore the output of uninstall.
        # The app may not have been installed in the first place. That's ok.

        flags = ''

        product_folder = self._aosp_product_path
        if not product_folder:
            raise TestSuiteException(self._missing_path_msg)

        app_folder = os.path.join(product_folder, 'data/app')

        cmd = 'install {0} {1}/{2}/{2}.apk'.format(flags, app_folder, app)
        output = self._android.adb(cmd, False, True,
                                   util_constants.PUSH_TIMEOUT)
        if ('Success' not in output) or ("can't find" in output):
            raise TestSuiteException('unable to install app {}: {}'.format(
                app, output))

    def _push_all_java(self):
        '''Push all apk files to the device.

        This involves uninstalling any old installations and installing again.

        Raises:
            TestSuiteException: An apk could not be installed.
        '''
        for app, package in self._tests_apk.items():
            self._install_apk(app, package)

    def _push_all_ndk(self):
        '''Push all ndk binaries to the device.

        Raises:
            TestSuiteException: A binary could not be pushed to the device or
                                a previous process could not be killed.
        '''
        product_folder = self._aosp_product_path
        if not product_folder:
            raise TestSuiteException(self._missing_path_msg)

        bin_folder = os.path.join(product_folder, 'system/bin')

        for app in self._tests_ndk:
            self._log.info('pushing {0}'.format(app))

            self._android.kill_all_processes(app)

            cmd = 'push %s/%s /data' % (bin_folder, app)
            output = self._android.adb(cmd, False, True,
                                       util_constants.PUSH_TIMEOUT)
            if ('failed to copy' in output or
                'No such file or directory' in output):
                raise TestSuiteException('unable to push binary ' + app)

            # be sure to set the execute bit for NDK binaries
            self._android.shell('chmod 777 /data/{0}'.format(app))

    def _push_all_jni(self):
        '''Push all JNI apk files to the device.

        This involves uninstalling any old installations and installing again.

        Raises:
            TestSuiteException: An apk could not be installed.
        '''
        product_folder = self._aosp_product_path
        if not product_folder:
            raise TestSuiteException(self._missing_path_msg)

        app_folder = os.path.join(product_folder, 'system/lib')

        # Ensure the system/lib directory is writable
        self._android.make_device_writeable()

        for app, package in self._tests_jni.items():
            self._install_apk(app, package)

    def delete_ndk_cache(self):
        '''Deletes NDK cached scripts from the device.

        The NDK caches compiled scripts as shared libraries in
        the folder specified when calling `rs->init()`.

        For all out tests this is set to '/data/rscache'.
        '''
        self._android.shell('rm -r /data/rscache')

    def get_package(self, app_name):
        '''From a given apk name get the name of its package.

        Args:
            app_name: The string that is the name of the apk.

        Returns:
            A string representing the name of the package of the app.

        Raises:
            TestSuiteException: The app name is not in the list of apks.
        '''
        if app_name in self._tests_apk:
            return self._tests_apk[app_name]
        elif app_name in self._tests_jni:
            return self._tests_jni[app_name]
        else:
            msg = ('unknown app %s. (Do you need to add an '
                  'entry to bundle.py :: test_apps_?)' % app_name)
            raise TestSuiteException(msg)
        return self._tests_apk[app_name]

    def launch(self, app_name):
        '''Launch an apk/ndk app on a remote device.

        Args:
            app_name: The string that is the name of the APK or NDK executable.

        Returns:
            The Process ID of the launched executable, otherwise None

        Raises:
            TestSuiteException: Previous processes of this apk could not be
                                killed.
        '''
        process_name = ''
        success = False
        if app_name in self._tests_apk:
            process_name = self._tests_apk[app_name]

            self._android.kill_all_processes(process_name)

            success = self._android.launch_app(process_name, 'MainActivity')
        elif app_name in self._tests_ndk:
            process_name = app_name
            self._android.kill_all_processes(process_name)
            success = self._android.launch_elf(process_name)
        elif app_name in self._tests_jni:
            package = self._tests_jni[app_name]

            self._android.kill_process(package)

            success = self._android.launch_app(package, 'MainActivity')
            if not success:
                self._log.log_and_print(app_name +
                    ' is not installed. Try removing the --no-install option?')
                return None

            return self._android.find_app_pid(package)
        else:
            self._log.error('Executable {0} neither Java nor NDK.'
                            .format(app_name))

            self._log.fatal('Failed to launch test executable {0}'
                            .format(app_name))
            return None

        if not success:
            self._log.log_and_print(app_name +
                ' is not installed. Try removing the --no-install option?')
            return None

        return self._android.find_app_pid(process_name)

    def check_apps_installed(self, java_only):
        ''' Check whether all Java/JNI/NDK apps are installed on the device.

        Args:
            java_only: Boolean to specify whether only the Java apks should be
                       checked (in case of --wimpy mode for example).

        Raises:
            TestSuiteException: Not all apps are installed.
        '''
        java_and_jni_apks = self._tests_apk.copy()

        if not java_only:
            java_and_jni_apks.update(self._tests_jni)

        installed = self._android.shell('pm list packages -f')

        for app, package in java_and_jni_apks.items():
            if package not in installed:
                raise TestSuiteException('apk %s is not installed.' % app)

        if not java_only:
            ls_data = self._android.shell('ls /data')
            for app in self._tests_ndk:
                if app not in ls_data:
                    raise TestSuiteException('app %s is not installed.' % app)
