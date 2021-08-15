/*
 * Copyright (C) 2018 The Android Open Source Project
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

package android.util.imagepool;

import org.junit.Test;

import android.util.imagepool.Bucket.BucketCreationMetaData;
import android.util.imagepool.ImagePool.Image.Orientation;

import java.awt.image.BufferedImage;
import java.lang.ref.SoftReference;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

public class ImagePoolHelperTest {

    @Test
    public void testGetBufferedImage() {
        int width = 10;
        int height = 10;
        int numberOfCopiesInBucket = 10;
        int maxCacheSize = width * height * 4 * 5; // can fit 5 width | height buffer
        Bucket bucket = new Bucket();
        BucketCreationMetaData metaData = new BucketCreationMetaData(
                width, height, BufferedImage.TYPE_INT_ARGB, numberOfCopiesInBucket, Orientation
                .NONE, maxCacheSize);
        ImagePoolStats stats = new ImagePoolStatsProdImpl();

        assertNotNull(ImagePoolHelper.getBufferedImage(bucket, metaData, stats));
    }

    @Test
    public void testGetBufferedImageRecurse() {
        int width = 10;
        int height = 10;
        int numberOfCopiesToRequestInBucket = 1;
        int numberOfCopiesInBucket = 10;
        int maxCacheSize = width * height * 4 * numberOfCopiesToRequestInBucket;

        Bucket bucket = new Bucket();
        for (int i = 0; i < numberOfCopiesInBucket; i++) {
            bucket.mBufferedImageRef.add(new SoftReference<>(null));
        }
        BucketCreationMetaData metaData = new BucketCreationMetaData(
                width, height, BufferedImage.TYPE_INT_ARGB, numberOfCopiesToRequestInBucket, Orientation
                .NONE, maxCacheSize);
        ImagePoolStats stats = new ImagePoolStatsProdImpl();

        assertNotNull(ImagePoolHelper.getBufferedImage(bucket, metaData, stats));
    }

    @Test
    public void testRecurseThenHitCacheLimit() {
        int width = 10;
        int height = 10;
        int numberOfCopiesToRequestInBucket = 1;
        int numberOfCopiesInBucket = 10;
        int maxCacheSize = width * height * 4 * numberOfCopiesToRequestInBucket / 2;

        Bucket bucket = new Bucket();
        for (int i = 0; i < numberOfCopiesInBucket; i++) {
            bucket.mBufferedImageRef.add(new SoftReference<>(null));
        }
        BucketCreationMetaData metaData = new BucketCreationMetaData(
                width, height, BufferedImage.TYPE_INT_ARGB, numberOfCopiesToRequestInBucket, Orientation
                .NONE, maxCacheSize);
        ImagePoolStats stats = new ImagePoolStatsProdImpl();

        assertNull(ImagePoolHelper.getBufferedImage(bucket, metaData, stats));
    }

    @Test
    public void testBucketHasImageToReturn() {
        int width = 10;
        int height = 10;
        int numberOfCopiesToRequestInBucket = 1;
        int numberOfCopiesInBucket = 10;
        int maxCacheSize = width * height * 4 * numberOfCopiesToRequestInBucket / 2;
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_4BYTE_ABGR);

        Bucket bucket = new Bucket();
        for (int i = 0; i < numberOfCopiesInBucket; i++) {
            bucket.mBufferedImageRef.add(new SoftReference<>(null));
        }
        bucket.mBufferedImageRef.add(new SoftReference<>(image));
        BucketCreationMetaData metaData = new BucketCreationMetaData(
                width, height, BufferedImage.TYPE_INT_ARGB, numberOfCopiesToRequestInBucket, Orientation
                .NONE, maxCacheSize);
        ImagePoolStats stats = new ImagePoolStatsProdImpl();

        assertNotNull(ImagePoolHelper.getBufferedImage(bucket, metaData, stats));
    }
}