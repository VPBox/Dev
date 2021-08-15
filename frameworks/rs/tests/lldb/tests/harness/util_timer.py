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

'''Timer utility'''

from __future__ import absolute_import

import threading


class Timer(object):
    '''A Timer utility to execute a callback after a certain interval.'''

    def __init__(self, interval, callback):
        '''Initialise the Timer without starting it.

        Args:
            interval: int or float, interval in seconds to count, before
                invoking the callback
            callback: function, it handles the function to call once
                the timeout expires.
        '''

        # validate input parameters
        if not isinstance(interval, (int, float)):
            raise TypeError('Argument "interval" is not a number: '
                             '{0}'.format(type(interval)))
        if not callable(callback):
            raise TypeError('Argument "callback" is not a function: '
                             '{0}'.format(type(callback)))

        self._timer = None
        self._callback = callback
        self._interval = interval

    def _is_running(self):
        '''Checks whether the timer is executing.

        Returns:
            boolean, true if the timer is currently running, false otherwise
        '''
        return self._timer is not None

    def start(self):
        '''Starts the timer.

        Returns:
            self, the Timer instance

        Throws:
            RuntimeError: if the timer is already running
        '''
        if self._is_running():
            raise RuntimeError('Timer already running')

        self._timer = threading.Timer(self._interval, self._callback)
        self._timer.start()
        return self # so that we can perform Timer(...).start()

    def stop(self):
        '''Stops the timer if it's executing.

        Returns:
            self, the Timer instance
        '''

        if self._is_running():
            self._timer.cancel()
            self._timer = None
        return self

    def reset(self):
        '''Restart the timer.

        Returns:
            self, the Timer instance
        '''

        self.stop()
        self.start()
        return self
