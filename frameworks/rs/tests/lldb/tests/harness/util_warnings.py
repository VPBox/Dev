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

'''Redirect the Python warnings into the log.'''

from __future__ import absolute_import

import warnings

from . import util_log

_OLD_WARNINGS_HANDLER = None


def redirect_warnings():
    '''Redirect all warnings issued by warnings::warn to the log.

    By default all python warnings are printed into sys.stderr. This method
    will force to redirect them into the test suite logger.
    '''

    # pylint: disable=global-statement
    global _OLD_WARNINGS_HANDLER

    # Already redirecting?
    if _OLD_WARNINGS_HANDLER:
        return None

    _OLD_WARNINGS_HANDLER = warnings.showwarning

    log = util_log.get_logger()

    def _redirect_warnings_to_log(*args):
        '''Redirect the warnings to the Logger.'''
        log.warn(warnings.formatwarning(*args).rstrip())

    warnings.showwarning = _redirect_warnings_to_log


def restore_warnings():
    '''Restore the reporting of warnings::warn as before.'''

    # pylint: disable=global-statement
    global _OLD_WARNINGS_HANDLER

    if _OLD_WARNINGS_HANDLER:
        warnings.showwarning = _OLD_WARNINGS_HANDLER
        _OLD_WARNINGS_HANDLER = None

