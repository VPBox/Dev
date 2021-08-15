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

import adb
import argparse
import os
import unittest
import fastboot
import subprocess
import sys

# Default values for arguments
device_type = "phone"

class ShellTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.fastboot = fastboot.FastbootDevice()

    def exists_validvals(self, varname, varlist, validlist):
        self.assertIn(varname, varlist)
        self.assertIn(varlist[varname], validlist)
        return varlist[varname]

    def exists_yes_no(self, varname, varlist):
        return self.exists_validvals(varname, varlist, ["yes", "no"])

    def exists_nonempty(self, varname, varlist):
        self.assertIn(varname, varlist)
        self.assertGreater(len(varlist[varname]), 0)
        return varlist[varname]

    def exists_integer(self, varname, varlist, base=10):
        val = 0
        self.assertIn(varname, varlist)
        try:
            val = int(varlist[varname], base)
        except ValueError:
            self.fail("%s (%s) is not an integer" % (varname, varlist[varname]))
        return val

    def get_exists(self, varname):
        val = self.fastboot.getvar(varname)
        self.assertIsNotNone(val)
        return val

    def get_exists_validvals(self, varname, validlist):
        val = self.get_exists(varname)
        self.assertIn(val, validlist)
        return val

    def get_exists_yes_no(self, varname):
        return self.get_exists_validvals(varname, ["yes", "no"])

    def get_exists_nonempty(self, varname):
        val = self.get_exists(varname)
        self.assertGreater(len(val), 0)
        return val

    def get_exists_integer(self, varname, base=10):
        val = self.get_exists(varname)
        try:
            num = int(val, base)
        except ValueError:
            self.fail("%s (%s) is not an integer" % (varname, val))
        return num

    def get_slotcount(self):
        slotcount = 0
        try:
            val = self.fastboot.getvar("slot-count")
            if val != None:
                slotcount = int(val)
        except ValueError:
            self.fail("slot-count (%s) is not an integer" % val)
        except subprocess.CalledProcessError:
            print "Does not appear to be an A/B device."
        if not slotcount:
            print "Does not appear to be an A/B device."
        return slotcount

    def test_getvarall(self):
        """Tests that required variables are reported by getvar all"""

        var_all = self.fastboot.getvar_all()
        self.exists_nonempty("version-baseband", var_all)
        self.exists_nonempty("version-bootloader", var_all)
        self.exists_nonempty("product", var_all)
        self.exists_yes_no("secure", var_all)
        self.exists_yes_no("unlocked", var_all)
        self.exists_validvals("off-mode-charge", var_all, ["0", "1"])
        self.assertIn("variant", var_all)
        voltage = self.exists_nonempty("battery-voltage", var_all)
        if voltage[-2:].lower() == "mv":
            voltage = voltage[:-2]
        try:
            voltnum = float(voltage)
        except ValueError:
            self.fail("battery-voltage (%s) is not a number" % (varname, voltage))
        self.exists_yes_no("battery-soc-ok", var_all)
        maxdl = self.exists_integer("max-download-size", var_all, 16)
        self.assertGreater(maxdl, 0)

        if "slot-count" in var_all:
            try:
                slotcount = int(var_all["slot-count"])
            except ValueError:
                self.fail("slot-count (%s) is not an integer" % var_all["slot-count"])
            if slotcount > 1:
                # test for A/B variables
                slots = [chr(slotnum+ord('a')) for slotnum in range(slotcount)]
                self.exists_validvals("current-slot", var_all, slots)

                # test for slot metadata
                for slot in slots:
                    self.exists_yes_no("slot-unbootable:"+slot, var_all)
                    self.exists_yes_no("slot-unbootable:"+slot, var_all)
                    self.exists_integer("slot-retry-count:"+slot, var_all)
            else:
                print "This does not appear to be an A/B device."

    def test_getvar_nonexistent(self):
        """Tests behaviour of nonexistent variables."""

        self.assertIsNone(self.fastboot.getvar("fhqwhgads"))

    def test_getvar(self):
        """Tests all variables separately"""

        self.get_exists_nonempty("version-baseband")
        self.get_exists_nonempty("version-bootloader")
        self.get_exists_nonempty("product")
        self.get_exists_yes_no("secure")
        self.get_exists_yes_no("unlocked")
        self.get_exists_validvals("off-mode-charge", ["0", "1"])
        self.get_exists("variant")
        voltage = self.get_exists_nonempty("battery-voltage")
        if voltage[-2:].lower() == "mv":
            voltage = voltage[:-2]
        try:
            voltnum = float(voltage)
        except ValueError:
            self.fail("battery-voltage (%s) is not a number" % voltage)
        self.get_exists_yes_no("battery-soc-ok")
        maxdl = self.get_exists_integer("max-download-size", 16)
        self.assertGreater(maxdl, 0)

        slotcount = self.get_slotcount()
        if slotcount  > 1:
            # test for A/B variables
            slots = [chr(slotnum+ord('a')) for slotnum in range(slotcount)]
            self.get_exists_validvals("current-slot", slots)

            # test for slot metadata
            for slot in slots:
                self.get_exists_yes_no("slot-unbootable:"+slot)
                self.get_exists_yes_no("slot-successful:"+slot)
                self.get_exists_integer("slot-retry-count:"+slot)

    def test_setactive(self):
        """Tests that A/B devices can switch to each slot, and the change persists over a reboot."""
        # Test invalid if not an A/B device
        slotcount = self.get_slotcount()
        if not slotcount:
            return

        maxtries = 0
        slots = [chr(slotnum+ord('a')) for slotnum in range(slotcount)]
        for slot in slots:
            self.fastboot.set_active(slot)
            self.assertEqual(slot, self.fastboot.getvar("current-slot"))
            self.assertEqual("no", self.fastboot.getvar("slot-unbootable:"+slot))
            self.assertEqual("no", self.fastboot.getvar("slot-successful:"+slot))
            retry = self.get_exists_integer("slot-retry-count:"+slot)
            if maxtries == 0:
                maxtries = retry
            else:
                self.assertEqual(maxtries, retry)
            self.fastboot.reboot(True)
            self.assertEqual(slot, self.fastboot.getvar("current-slot"))
            self.assertEqual("no", self.fastboot.getvar("slot-unbootable:"+slot))
            self.assertEqual("no", self.fastboot.getvar("slot-successful:"+slot))
            retry = self.get_exists_integer("slot-retry-count:"+slot)
            if maxtries == 0:
                maxtries = retry
            else:
                self.assertEqual(maxtries, retry)

    def test_hasslot(self):
        """Tests that A/B devices report partitions that have slots."""
        # Test invalid if not an A/B device
        if not self.get_slotcount():
            return

        self.assertEqual("yes", self.fastboot.getvar("has-slot:system"))
        self.assertEqual("yes", self.fastboot.getvar("has-slot:boot"))

        # Additional partition on AndroidThings (IoT) devices
        if device_type == "iot":
            self.assertEqual("yes", self.fastboot.getvar("has-slot:oem"))

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--device-type", default="phone",
                        help="Type of device ('phone' or 'iot').")
    parser.add_argument("extra_args", nargs="*")
    args = parser.parse_args()

    if args.device_type.lower() not in ("phone", "iot"):
        raise ValueError("Unsupported device type '%s'." % args.device_type)
    device_type = args.device_type.lower()

    sys.argv[1:] = args.extra_args
    unittest.main(verbosity=3)
