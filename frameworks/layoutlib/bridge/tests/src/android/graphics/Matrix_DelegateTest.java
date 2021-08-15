/*
 * Copyright (C) 2008 The Android Open Source Project
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

package android.graphics;

import java.util.Arrays;

import junit.framework.TestCase;

/**
 *
 */
public class Matrix_DelegateTest extends TestCase {

    public void testIdentity() {
        Matrix m1 = new Matrix();

        assertTrue(m1.isIdentity());

        m1.setValues(new float[]{1, 0, 0, 0, 1, 0, 0, 0, 1});
        assertTrue(m1.isIdentity());
    }

    public void testCopyConstructor() {
        Matrix m1 = new Matrix();
        Matrix m2 = new Matrix(m1);

        float[] v1 = new float[9];
        float[] v2 = new float[9];
        m1.getValues(v1);
        m2.getValues(v2);

        for (int i = 0; i < 9; i++) {
            assertEquals(v1[i], v2[i]);
        }
    }

    public void testInvert() {
        Matrix m1 = new Matrix();
        Matrix inverse = new Matrix();
        m1.setValues(new float[]{1, 2, 3, 4, 5, 6, 7, 8, 9});
        assertFalse(m1.invert(inverse));

        m1.setValues(new float[]{3, 5, 6, 2, 5, 7, 4, 8, 2});
        m1.invert(inverse);
        float[] values = new float[9];
        inverse.getValues(values);

        assertTrue(Arrays.equals(values,
                new float[]{1.0952381f, -0.9047619f, -0.11904762f, -0.5714286f, 0.42857143f,
                        0.21428572f, 0.0952381f, 0.0952381f, -0.11904762f}));
    }
}
