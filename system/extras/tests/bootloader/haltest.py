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

import bootctl
import shelltest
import sys
import unittest

# Note: In order to run these tests, the device must be able to boot
# from all slots on the device.
class HalTest(shelltest.ShellTest):
    def __init__(self, *args, **kwargs):
        super(HalTest, self).__init__(*args, **kwargs)
        self.bootctl = bootctl.Bootctl(self.device)

    def test_slots(self):
        """Test that all slots are reported and named uniquely."""

        self.device.root()
        self.device.wait()
        num_slots = self.bootctl.get_number_slots()
        suffixes = dict()
        for slot in range(num_slots):
            suffix = self.bootctl.get_suffix(slot)
            self.assertNotEqual(suffix, "(null)")
            suffixes[suffix] = slot
        self.assertEqual(len(suffixes), num_slots)

    def test_mark_successful(self):
        """Ensure mark successful works, and persists on reboot.

        Ensure that mark_successful will mark the slot as
        successful, and that the HAL sees this. First resets
        slot-successful by setting the active slot to the current one."""

        self.device.root()
        self.device.wait()
        slot = self.bootctl.get_current_slot()
        self.assertTrue(self.bootctl.set_active_boot_slot(slot))
        self.assertFalse(self.bootctl.is_slot_marked_successful(slot))
        self.assertTrue(self.bootctl.mark_boot_successful())
        self.assertTrue(self.bootctl.is_slot_marked_successful(slot))
        self.device.reboot()
        self.device.wait()
        self.device.root()
        self.device.wait()
        self.assertTrue(self.bootctl.is_slot_marked_successful(slot))

    def test_switch_slots(self):
        """Test that setActiveBootSlot works and persists

        Ensure switching slots works, and that setting the slot does not
        change the reported slot until the reboot."""

        # Cycle through all slots once
        num_slots = self.bootctl.get_number_slots()
        for i in range(num_slots):
            self.device.root()
            self.device.wait()
            slot = self.bootctl.get_current_slot()
            new_slot = (slot + 1) % num_slots
            self.assertTrue(self.bootctl.set_active_boot_slot(new_slot))
            slot2 = self.bootctl.get_current_slot()
            self.assertEqual(slot, slot2)
            self.device.reboot()
            self.device.wait()
            self.device.root()
            self.device.wait()
            self.assertEqual(new_slot, self.bootctl.get_current_slot())

    def test_unbootable(self):
        """Test setSlotAsUnbootable

        Test that the device will attempt to roll back to a valid slot if
        the current slot is unbootable."""

        # Cycle through all slots once
        num_slots = self.bootctl.get_number_slots()
        for i in range(num_slots):
            self.device.root()
            self.device.wait()
            slot = self.bootctl.get_current_slot()
            new_slot = (slot + 1) % num_slots
            self.device.root()
            self.device.wait()
            self.assertTrue(self.bootctl.set_active_boot_slot(new_slot))
            self.assertTrue(self.bootctl.is_slot_bootable(new_slot))
            self.assertTrue(self.bootctl.set_slot_as_unbootable_slot(new_slot))
            self.assertFalse(self.bootctl.is_slot_bootable(new_slot))
            self.device.reboot()
            self.device.wait()
            self.device.root()
            self.device.wait()
            self.assertEqual(slot, self.bootctl.get_current_slot())
            self.assertFalse(self.bootctl.is_slot_bootable(new_slot))
            self.assertTrue(self.bootctl.set_active_boot_slot(new_slot))
            self.assertTrue(self.bootctl.is_slot_bootable(new_slot))
            self.device.reboot()
            self.device.wait()
            self.device.root()
            self.device.wait()
            self.assertEqual(new_slot, self.bootctl.get_current_slot());

if __name__ == '__main__':
    unittest.main(verbosity=3)
