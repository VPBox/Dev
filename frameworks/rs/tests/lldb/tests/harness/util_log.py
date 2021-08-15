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

'''Initialise the Python logging facility for the test suite.

from __future__ import absolute_import

It provides the function to initialise the logging facility and retrieve an
instance of the logger class. It also contains the definition of the internal
logger class.
'''
from __future__ import print_function

import io
import sys
import logging


INITIALISED = False
NAMESPACE = 'RS_LLDB_TESTSUITE'

def initialise(identifier, level=logging.INFO, print_to_stdout=False,
               file_path=None, file_mode='a'):
    '''Initialise the logging facility for the test suite.

    This function should be invoked only once, at the start of the program, and
    before emitting any log.

    Args:
        identifier: String, a label that will be part of each record. It is
                    usually the test case name.
        level: Integer, all messages above this log level will be discarded.
               Valid values are those recognised by the python logging module:
               https://docs.python.org/2/library/logging.html#levels .
        print_to_stdout: Boolean, whether the logs should be redirected to
                         sys.stdout (true) or stored into a text file (false).
        file_path: String, path to the text file in which to store the logs.
                   This option is only meaningful when print_to_stdout = False.
        file_mode: String, the mode to open the text file. Valid modes are
                   those recognised by the standard Python `open' function.
                   This option is only meaningful when print_to_stdout = False.

    Raises:
        RuntimeError: If the logging has already been initialised
        ValueError: If the argument "file_path" has not been provided when
                    print_to_stdout=False
    '''
    # pylint: disable=global-statement
    global INITIALISED
    if INITIALISED:
        raise RuntimeError('Already initialised')

    # set the logging class
    old_logger_class = logging.getLoggerClass()
    logging.setLoggerClass(RsLogger)

    # initialise the Logger
    log = logging.getLogger(NAMESPACE)
    log.setLevel(level) # reject all logs below

    # don't propagate the log records to the logging root
    log.propagate = False

    # restore the previous class
    logging.setLoggerClass(old_logger_class)

    # handler
    if print_to_stdout:
        handler_default = logging.StreamHandler(sys.stdout)
    else:
        if file_path is None:
            raise ValueError('Missing mandatory argument "file_path"')

        handler_default = logging.FileHandler(file_path, file_mode)

    # Do not filter records in the handler because of the level
    handler_default.setLevel(logging.NOTSET)

    # format the message
    handler_default.setFormatter(
        logging.Formatter(
            '%(asctime)s [{0}] [%(levelname)s] %(message)s'
                .format(identifier)
    ))

    log.addHandler(handler_default)

    INITIALISED = True


class RsLogger(logging.getLoggerClass()):
    '''Internal logging class.

    This is an internal class to enhance the logging facility with the methods
    "log_and_print" and "seek_to_end".
    '''
    # pylint: disable=too-many-public-methods

    def log_and_print(self, msg, level=logging.INFO):
        '''Print "msg" to stdout and emit a log record.

        Args:
            msg: The message to emit.
            level: The level to use. By default it is logging.INFO.
        '''
        print(msg)
        self.log(level, msg)

    def seek_to_end(self):
        '''Reset the cursor position to the end for all handlers that are
        Text File managers.'''
        for hndlr in self.handlers:
            if isinstance(hndlr, logging.FileHandler):
                hndlr.stream.seek(0, io.SEEK_END)


def get_logger():
    '''Retrieves the Logger instance related to the testsuite.

    Throws:
        RuntimeError: If the logging facility has not been initialised with
                      "initialise" beforehand.

    Returns:
        An instance of logging.Logger to write the logs.
    '''
    if not INITIALISED:
        raise RuntimeError('Logging facility not initialised')

    return logging.getLogger(NAMESPACE)
