/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.libcore.timezone.tzlookup;

import org.junit.Test;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class ErrorsTest {

    @Test
    public void warnings() {
        Errors errors = new Errors();
        assertTrue(errors.isEmpty());
        assertFalse(errors.hasError());
        assertFalse(errors.hasFatal());

        errors.addWarning("Hello");
        assertFalse(errors.isEmpty());
        assertFalse(errors.hasError());
        assertFalse(errors.hasFatal());

        TestUtils.assertContains(errors.asString(), "Hello");
    }

    @Test
    public void error() {
        Errors errors = new Errors();
        assertTrue(errors.isEmpty());
        assertFalse(errors.hasError());
        assertFalse(errors.hasFatal());

        errors.addError("Hello");
        assertFalse(errors.isEmpty());
        assertTrue(errors.hasError());
        assertFalse(errors.hasFatal());

        TestUtils.assertContains(errors.asString(), "Hello");
    }

    @Test
    public void fatal() {
        Errors errors = new Errors();
        assertTrue(errors.isEmpty());
        assertFalse(errors.hasError());
        assertFalse(errors.hasFatal());

        errors.addFatal("Hello");
        assertFalse(errors.isEmpty());
        assertTrue(errors.hasError());
        assertTrue(errors.hasFatal());

        TestUtils.assertContains(errors.asString(), "Hello");
    }

    @Test
    public void scope() {
        Errors errors = new Errors();

        errors.addWarning("Hello");

        errors.pushScope("Monty Python");
        errors.addError("John Cleese");

        errors.pushScope("Holy grail");
        errors.addFatal("Silly place");
        errors.popScope();

        errors.addError("Michael Palin");

        errors.pushScope("Parrot sketch");
        errors.addFatal("Fjords");
        errors.popScope();

        String[] lines = errors.asString().split("\n");

        String line0 = lines[0];
        TestUtils.assertContains(line0, "Hello");
        TestUtils.assertAbsent(line0, "Monty Python");

        String line1 = lines[1];
        TestUtils.assertContains(line1, "Monty Python");
        TestUtils.assertAbsent(line1, "Holy grail");
        TestUtils.assertContains(line1, "John Cleese");

        String line2 = lines[2];
        TestUtils.assertContains(line2, "Monty Python", "Holy grail");
        TestUtils.assertAbsent(line2, "Parrot sketch");
        TestUtils.assertContains(line2, "Silly place");

        String line3 = lines[3];
        TestUtils.assertContains(line3, "Monty Python");
        TestUtils.assertAbsent(line3, "Holy grail");
        TestUtils.assertContains(line3, "Michael Palin");

        String line4 = lines[4];
        TestUtils.assertContains(line4, "Monty Python", "Parrot sketch");
        TestUtils.assertAbsent(line4, "Holy grail");
        TestUtils.assertContains(line4, "Fjords");
    }

}
