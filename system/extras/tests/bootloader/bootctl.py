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

class Bootctl(object):
    def __init__(self, device):
        self.device = device
        self.base = ["bootctl"]

    def _exec(self, cmd):
        return self.device.shell_nocheck(self.base + [cmd])

    def get_number_slots(self):
        """returns number of slots"""

        return int(self._exec("get-number-slots")[1])

    def get_current_slot(self):
        """returns current slot number"""

        return int(self._exec("get-current-slot")[1])

    def mark_boot_successful(self):
        """returns true on success, false on failure"""

        return self._exec("mark-boot-successful")[0] == 0

    def set_active_boot_slot(self, slot):
        """returns true on success, false on failure"""

        return self._exec("set-active-boot-slot " + str(slot))[0] == 0

    def set_slot_as_unbootable_slot(self, slot):
        """returns true on success, false on failure"""

        return self._exec("set-slot-as-unbootable " + str(slot))[0] == 0

    def is_slot_bootable(self, slot):
        """Returns true if slot is bootable"""

        return self._exec("is-slot-bootable " + str(slot))[0] == 0

    def is_slot_marked_successful(self, slot):
        """returns true on success, false on failure"""

        return self._exec("is-slot-marked-successful " + str(slot))[0] == 0

    def get_suffix(self, slot):
        """returns suffix string for specified slot number"""

        return self._exec("get-suffix " + str(slot))[1].strip()
