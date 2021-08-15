/*
 * Copyright (C) 2015 The Android Open Source Project
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

package com.android.server.wifi;

import android.net.wifi.ScanResult;
import android.net.wifi.WifiScanner.ScanData;
import android.net.wifi.WifiSsid;

import com.android.server.wifi.hotspot2.NetworkDetail;

import java.math.BigInteger;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Random;

/**
 * Utility for creating scan results from a scan
 */
public class ScanResults {
    private final ArrayList<ScanDetail> mScanDetails = new ArrayList<>();
    private final ScanData mScanData;
    private final ScanData mRawScanData;
    private final ScanResult[] mScanResults;

    private ScanResults(ArrayList<ScanDetail> scanDetails, ScanData scanData,
            ScanResult[] scanResults) {
        mScanDetails.addAll(scanDetails);
        mScanData = scanData;
        mRawScanData = scanData;
        mScanResults = scanResults;
    }

    /**
     * Merge the results contained in a number of ScanResults into a single ScanResults
     */
    public static ScanResults merge(int bandScanned, ScanResults... others) {
        ArrayList<ScanDetail> scanDetails = new ArrayList<>();
        ArrayList<ScanResult> scanDataResults = new ArrayList<>();
        ArrayList<ScanResult> rawScanResults = new ArrayList<>();
        for (ScanResults other : others) {
            scanDetails.addAll(other.getScanDetailArrayList());
            scanDataResults.addAll(Arrays.asList(other.getScanData().getResults()));
            rawScanResults.addAll(Arrays.asList(other.getRawScanResults()));
        }
        Collections.sort(scanDataResults, SCAN_RESULT_RSSI_COMPARATOR);
        int id = others[0].getScanData().getId();
        return new ScanResults(scanDetails, new ScanData(id, 0, 0, bandScanned, scanDataResults
                        .toArray(new ScanResult[scanDataResults.size()])),
                rawScanResults.toArray(new ScanResult[rawScanResults.size()]));
    }

    private static String generateBssid(Random r) {
        return String.format("%02X:%02X:%02X:%02X:%02X:%02X",
                r.nextInt(256), r.nextInt(256), r.nextInt(256),
                r.nextInt(256), r.nextInt(256), r.nextInt(256));
    }

    public static final Comparator<ScanResult> SCAN_RESULT_RSSI_COMPARATOR =
            new Comparator<ScanResult>() {
        public int compare(ScanResult r1, ScanResult r2) {
            return r2.level - r1.level;
        }
    };

    public static ScanResult.InformationElement generateSsidIe(String ssid) {
        ScanResult.InformationElement ie = new ScanResult.InformationElement();
        ie.id = ScanResult.InformationElement.EID_SSID;
        ie.bytes = ssid.getBytes(Charset.forName("UTF-8"));
        return ie;
    }

    /**
     * Generates an array of random ScanDetails with the given frequencies, seeded by the provided
     * seed value and test method name and class (annotated with @Test). This method will be
     * consistent between calls in the same test across runs.
     *
     * @param seed combined with a hash of the test method this seeds the random number generator
     * @param freqs list of frequencies for the generated scan results, these will map 1 to 1 to
     *              to the returned scan details. Duplicates can be specified to create multiple
     *              ScanDetails with the same frequency.
     */
    private static ScanDetail[] generateNativeResults(boolean needIE, int seed, int... freqs) {
        ScanDetail[] results = new ScanDetail[freqs.length];
        // Seed the results based on the provided seed as well as the test method name
        // This provides more varied scan results between individual tests that are very similar.
        Random r = new Random(seed + WifiTestUtil.getTestMethod().hashCode());
        for (int i = 0; i < freqs.length; ++i) {
            int freq = freqs[i];
            String ssid = new BigInteger(128, r).toString(36);
            String bssid = generateBssid(r);
            int rssi = r.nextInt(40) - 99; // -99 to -60
            ScanResult.InformationElement[] ie;
            if (needIE) {
                ie = new ScanResult.InformationElement[1];
                ie[0] = generateSsidIe(ssid);
            } else {
                ie = new ScanResult.InformationElement[0];
            }
            List<String> anqpLines = new ArrayList<>();
            NetworkDetail nd = new NetworkDetail(bssid, ie, anqpLines, freq);
            ScanDetail detail = new ScanDetail(nd, WifiSsid.createFromAsciiEncoded(ssid),
                    bssid, "", rssi, freq,
                    Long.MAX_VALUE, /* needed so that scan results aren't rejected because
                                        they are older than scan start */
                    ie, anqpLines);
            results[i] = detail;
        }
        return results;
    }

    /**
     * Create scan results with no IE information.
     */
    public static ScanDetail[] generateNativeResults(int seed, int... freqs) {
        return generateNativeResults(true, seed, freqs);
    }

    /**
     * Create a ScanResults with randomly generated results seeded by the id.
     * @see #generateNativeResults for more details on how results are generated
     */
    public static ScanResults create(int id, int bandScanned, int... freqs) {
        return create(id, bandScanned, generateNativeResults(id, freqs));
    }

    public static ScanResults create(int id, int bandScanned,
            ScanDetail... nativeResults) {
        return new ScanResults(id, bandScanned, -1, nativeResults);
    }

    /**
     * Create scan results that contain all results for the native results and
     * full scan results, but limits the number of onResults results after sorting
     * by RSSI
     */
    public static ScanResults createOverflowing(int id, int bandScanned, int maxResults,
            ScanDetail... nativeResults) {
        return new ScanResults(id, bandScanned, maxResults, nativeResults);
    }

    private ScanResults(int id, int bandScanned, int maxResults, ScanDetail... nativeResults) {
        mScanResults = new ScanResult[nativeResults.length];
        for (int i = 0; i < nativeResults.length; ++i) {
            mScanDetails.add(nativeResults[i]);
            mScanResults[i] = nativeResults[i].getScanResult();
        }
        ScanResult[] sortedScanResults = Arrays.copyOf(mScanResults, mScanResults.length);
        Arrays.sort(sortedScanResults, SCAN_RESULT_RSSI_COMPARATOR);
        mRawScanData = new ScanData(id, 0, 0, bandScanned, sortedScanResults);
        if (maxResults == -1) {
            mScanData = mRawScanData;
        } else {
            ScanResult[] reducedScanResults = Arrays.copyOf(sortedScanResults,
                    Math.min(sortedScanResults.length, maxResults));
            mScanData = new ScanData(id, 0, 0, bandScanned, reducedScanResults);
        }
    }

    public ArrayList<ScanDetail> getScanDetailArrayList() {
        return mScanDetails;
    }

    public ScanData getScanData() {
        return mScanData;
    }

    public ScanResult[] getRawScanResults() {
        return mScanResults;
    }

    public ScanData getRawScanData() {
        return mRawScanData;
    }
}
