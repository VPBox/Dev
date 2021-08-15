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

import android.util.imagepool.ImagePool.Image;
import android.util.imagepool.ImagePool.ImagePoolPolicy;

import java.awt.image.BufferedImage;
import java.lang.ref.SoftReference;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

public class ImagePoolImplTest {

    private static final long TIMEOUT_SEC = 3;

    @Test
    public void testImagePoolInstance() {
        ImagePool pool1 = ImagePoolProvider.get();
        ImagePool pool2 = ImagePoolProvider.get();
        assertNotNull(pool1);
        assertNotNull(pool2);
        assertEquals(pool1, pool2);
    }


    @Test
    public void testImageDispose() throws InterruptedException {
        int width = 700;
        int height = 800;
        int type = BufferedImage.TYPE_INT_ARGB;
        CountDownLatch countDownLatch = new CountDownLatch(1);
        ImagePoolImpl pool = getSimpleSingleBucketPool(width, height);
        Image img1 = pool.acquire(width, height, type,
                bufferedImage -> countDownLatch.countDown());
        BufferedImage img = getImg(img1);
        assertNotNull(img);
        img1 = null;

        // ensure dispose actually loses buffered image link so it can be gc'd
        gc();
        assertTrue(countDownLatch.await(TIMEOUT_SEC, TimeUnit.SECONDS));
    }
    @Test
    public void testImageDisposeFromFunction() throws InterruptedException {
        int width = 700;
        int height = 800;
        int type = BufferedImage.TYPE_INT_ARGB;
        CountDownLatch cd = new CountDownLatch(1);
        ImagePoolImpl pool = getSimpleSingleBucketPool(width, height);

        BufferedImage img = createImageAndReturnBufferedImage(pool, width, height, type, cd);
        assertNotNull(img);

        // ensure dispose actually loses buffered image link so it can be gc'd
        gc();
        assertTrue(cd.await(TIMEOUT_SEC, TimeUnit.SECONDS));
    }

    @Test
    public void testImageDisposedAndRecycled() throws InterruptedException {
        int width = 700;
        int height = 800;
        int bucketWidth = 800;
        int bucketHeight = 800;
        int variant = 1;
        int type = BufferedImage.TYPE_INT_ARGB;
        ImagePoolImpl pool = new ImagePoolImpl(new ImagePoolPolicy(
                new int[]{bucketWidth, bucketHeight},
                new int[]{1, 1},
                bucketHeight * bucketWidth * 4 * 3));

        // acquire first image and draw something.
        BufferedImage bufferedImageForImg1;
        final CountDownLatch countDownLatch1 = new CountDownLatch(1);
        {
            Image img1 = pool.acquire(width, height, type,
                    bufferedImage -> countDownLatch1.countDown());
            bufferedImageForImg1 = getImg(img1);
            img1 = null; // this is still needed.
        }
        // dispose
        gc();
        assertTrue(countDownLatch1.await(TIMEOUT_SEC, TimeUnit.SECONDS));

        // ensure dispose actually loses buffered image link so it can be gc'd
        assertNotNull(bufferedImageForImg1);
        assertEquals(bufferedImageForImg1.getWidth(), bucketWidth);
        assertEquals(bufferedImageForImg1.getHeight(), bucketHeight);

        // get 2nd image with the same spec
        final CountDownLatch countDownLatch2 = new CountDownLatch(1);
        BufferedImage bufferedImageForImg2;
        {
            Image img2 = pool.acquire(width - variant, height - variant, type,
                    bufferedImage -> countDownLatch2.countDown());
            bufferedImageForImg2 = getImg(img2);
            assertEquals(bufferedImageForImg1, bufferedImageForImg2);
            img2 = null;
        }
        // dispose
        gc();
        assertTrue(countDownLatch2.await(TIMEOUT_SEC, TimeUnit.SECONDS));

        // ensure that we're recycling previously created buffered image.
        assertNotNull(bufferedImageForImg1);
        assertNotNull(bufferedImageForImg2);
    }


    @Test
    public void testBufferedImageReleased() throws InterruptedException {
        int width = 700;
        int height = 800;
        int bucketWidth = 800;
        int bucketHeight = 800;
        ImagePoolImpl pool = new ImagePoolImpl(new ImagePoolPolicy(
                new int[]{bucketWidth, bucketHeight},
                new int[]{1, 1},
                bucketWidth * bucketWidth * 4 * 2));
        CountDownLatch countDownLatch = new CountDownLatch(1);
        Image image1 = pool.acquire(width, height, BufferedImage.TYPE_INT_ARGB,
                bufferedImage -> countDownLatch.countDown());
        BufferedImage internalPtr = getImg(image1);
        // dispose
        image1 = null;
        gc();
        assertTrue(countDownLatch.await(TIMEOUT_SEC, TimeUnit.SECONDS));

        // Simulate BufferedBitmaps being gc'd. Bucket filled with null soft refs.
        for (Bucket bucket : ((ImagePoolImpl) pool).mPool.values()) {
            bucket.mBufferedImageRef.clear();
            bucket.mBufferedImageRef.add(new SoftReference<>(null));
            bucket.mBufferedImageRef.add(new SoftReference<>(null));
        }

        assertNotEquals(internalPtr,
                getImg(pool.acquire(width, height, BufferedImage.TYPE_INT_ARGB)));
    }

    @Test
    public void testPoolWidthHeightNotBigEnough() {
        int width = 1000;
        int height = 1000;
        int bucketWidth = 999;
        int bucketHeight = 800;
        ImagePool pool = new ImagePoolImpl(
                new ImagePoolPolicy(new int[]{bucketWidth, bucketHeight}, new int[]{1, 1},
                        bucketWidth * bucketWidth * 4 * 2));
        ImageImpl image = (ImageImpl) pool.acquire(width, height, BufferedImage.TYPE_INT_ARGB);

        assertEquals(getTooBigForPoolCount(pool), 1);
    }

    @Test
    public void testSizeNotBigEnough() {
        int width = 500;
        int height = 500;
        int bucketWidth = 800;
        int bucketHeight = 800;
        ImagePoolImpl pool = new ImagePoolImpl(
                new ImagePoolPolicy(new int[]{bucketWidth, bucketHeight}, new int[]{1, 1},
                        bucketWidth * bucketWidth)); // cache not big enough.
        ImageImpl image = (ImageImpl) pool.acquire(width, height, BufferedImage.TYPE_INT_ARGB);

        assertEquals(getTooBigForPoolCount(pool), 1);
        assertEquals(image.getWidth(), width);
        assertEquals(image.getHeight(), height);
    }

    @Test
    public void testImageMultipleCopies() throws InterruptedException {
        int width = 700;
        int height = 800;
        int bucketWidth = 800;
        int bucketHeight = 800;
        int type = BufferedImage.TYPE_INT_ARGB;
        ImagePoolImpl pool = new ImagePoolImpl(new ImagePoolPolicy(
                new int[]{bucketWidth, bucketHeight},
                new int[]{2, 2},
                bucketHeight * bucketWidth * 4 * 4));

        // create 1, and 2 different instances.
        final CountDownLatch cd1 = new CountDownLatch(1);
        Image img1 = pool.acquire(width, height, type, bufferedImage -> cd1.countDown());
        BufferedImage bufferedImg1 = getImg(img1);

        Image img2 = pool.acquire(width, height, type);
        BufferedImage bufferedImg2 = getImg(img2);

        assertNotEquals(bufferedImg1, bufferedImg2);

        // disposing img1. Since # of copies == 2, this buffer should be recycled.
        img1 = null;
        gc();
        cd1.await(TIMEOUT_SEC, TimeUnit.SECONDS);

        // Ensure bufferedImg1 is recycled in newly acquired img3.
        Image img3 = pool.acquire(width, height, type);
        BufferedImage bufferedImage3 = getImg(img3);
        assertNotEquals(bufferedImg2, bufferedImage3);
        assertEquals(bufferedImg1, bufferedImage3);
    }

    @Test
    public void testPoolDispose() throws InterruptedException {
        int width = 700;
        int height = 800;
        int bucketWidth = 800;
        int bucketHeight = 800;
        int type = BufferedImage.TYPE_INT_ARGB;

        // Pool barely enough for 1 image.
        ImagePoolImpl pool = new ImagePoolImpl(new ImagePoolPolicy(
                new int[]{bucketWidth, bucketHeight},
                new int[]{2, 2},
                bucketHeight * bucketWidth * 4));

        // create 1, and 2 different instances.
        final CountDownLatch cd1 = new CountDownLatch(1);
        Image img1 = pool.acquire(width, height, type, bufferedImage -> cd1.countDown());
        BufferedImage bufferedImg1 = getImg(img1);
        assertEquals(getAllocatedTotalBytes(pool), bucketWidth * bucketHeight * 4);
        assertEquals(getTooBigForPoolCount(pool), 0);

        // Release the img1.
        img1 = null;
        gc();
        cd1.await(TIMEOUT_SEC, TimeUnit.SECONDS);

        // Dispose pool.
        pool.dispose();
        assertEquals(getAllocatedTotalBytes(pool), 0);

        // Request the same sized image as previous.
        // If the pool was not disposed, this would return the image with bufferedImg1.
        Image img2 = pool.acquire(width, height, type);
        BufferedImage bufferedImg2 = getImg(img2);
        assertEquals(getAllocatedTotalBytes(pool), bucketWidth * bucketHeight * 4);
        assertEquals(getTooBigForPoolCount(pool), 0);

        // Pool disposed before. No buffered image should be recycled.
        assertNotEquals(img1, img2);
        assertNotEquals(bufferedImg1, bufferedImg2);
    }

    private static BufferedImage createImageAndReturnBufferedImage(ImagePoolImpl pool, int width,
            int height
            , int type, CountDownLatch cd) {
        Image img1 = pool.acquire(width, height, type, bufferedImage -> cd.countDown());
        return getImg(img1);
        // At this point img1 should have no reference, causing finalizable to trigger
    }

    private static ImagePoolImpl getSimpleSingleBucketPool(int width, int height) {

        int bucketWidth = Math.max(width, height);
        int bucketHeight = Math.max(width, height);
        return new ImagePoolImpl(new ImagePoolPolicy(
                new int[]{bucketWidth, bucketHeight},
                new int[]{1, 1},
                bucketHeight * bucketWidth * 4 * 3));
    }

    // Try to force a gc round
    private static void gc() {
        System.gc();
        System.gc();
        System.gc();
    }

    private static int getTooBigForPoolCount(ImagePool pool) {
        return ((ImagePoolStatsProdImpl) ((ImagePoolImpl) pool).mImagePoolStats).mTooBigForPoolCount;
    }

    private static long getAllocatedTotalBytes(ImagePool pool) {
        return ((ImagePoolStatsProdImpl) ((ImagePoolImpl) pool).mImagePoolStats).mAllocateTotalBytes;
    }

    private static BufferedImage getImg(Image image) {
        return ((ImageImpl) image).mImg;
    }
}
