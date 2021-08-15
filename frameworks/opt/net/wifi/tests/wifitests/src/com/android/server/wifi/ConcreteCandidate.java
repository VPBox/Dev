/*
 * Copyright 2019 The Android Open Source Project
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

import android.util.ArrayMap;

import java.util.Map;

public final class ConcreteCandidate implements WifiCandidates.Candidate {
    private WifiCandidates.Key mKey;
    private ScanDetail mScanDetail;
    private int mNetworkConfigId = -1;
    private boolean mIsOpenNetwork;
    private boolean mIsCurrentNetwork;
    private boolean mIsCurrentBssid;
    private boolean mIsPasspoint;
    private boolean mIsEphemeral;
    private boolean mIsTrusted;
    private int mEvaluatorId = -1;
    private int mEvaluatorScore = Integer.MIN_VALUE;
    private double mLastSelectionWeight;
    private int mScanRssi = -127;
    private int mFrequency = -1;
    private final Map<WifiScoreCardProto.Event, WifiScoreCardProto.Signal>
            mEventStatisticsMap = new ArrayMap<>();

    ConcreteCandidate() {
    }

    public ConcreteCandidate(WifiCandidates.Candidate candidate) {
        mKey = candidate.getKey();
        mScanDetail = candidate.getScanDetail();
        mNetworkConfigId = candidate.getNetworkConfigId();
        mIsOpenNetwork = candidate.isOpenNetwork();
        mIsCurrentNetwork = candidate.isCurrentNetwork();
        mIsCurrentBssid = candidate.isCurrentBssid();
        mIsPasspoint = candidate.isPasspoint();
        mIsEphemeral = candidate.isEphemeral();
        mIsTrusted = candidate.isTrusted();
        mEvaluatorId = candidate.getEvaluatorId();
        mEvaluatorScore = candidate.getEvaluatorScore();
        mLastSelectionWeight = candidate.getLastSelectionWeight();
        mScanRssi = candidate.getScanRssi();
        mFrequency = candidate.getFrequency();
        for (WifiScoreCardProto.Event event : WifiScoreCardProto.Event.values()) {
            WifiScoreCardProto.Signal signal = candidate.getEventStatistics(event);
            if (signal != null) {
                mEventStatisticsMap.put(event, signal);
            }
        }
    }

    public ConcreteCandidate setKey(WifiCandidates.Key key) {
        mKey = key;
        return this;
    }

    @Override
    public WifiCandidates.Key getKey() {
        return mKey;
    }

    public ConcreteCandidate setScanDetail(ScanDetail scanDetail) {
        mScanDetail = scanDetail;
        return this;
    }

    @Override
    public ScanDetail getScanDetail() {
        return mScanDetail;
    }

    public ConcreteCandidate setNetworkConfigId(int networkConfigId) {
        mNetworkConfigId = networkConfigId;
        return this;
    }

    @Override
    public int getNetworkConfigId() {
        return mNetworkConfigId;
    }

    public ConcreteCandidate setOpenNetwork(boolean isOpenNetwork) {
        mIsOpenNetwork = isOpenNetwork;
        return this;
    }

    @Override
    public boolean isOpenNetwork() {
        return mIsOpenNetwork;
    }

    public ConcreteCandidate setPasspoint(boolean isPasspoint) {
        mIsPasspoint = isPasspoint;
        return this;
    }

    @Override
    public boolean isPasspoint() {
        return mIsPasspoint;
    }

    public ConcreteCandidate setEphemeral(boolean isEphemeral) {
        mIsEphemeral = isEphemeral;
        return this;
    }

    @Override
    public boolean isEphemeral() {
        return mIsEphemeral;
    }

    public ConcreteCandidate setTrusted(boolean isTrusted) {
        mIsTrusted = isTrusted;
        return this;
    }

    @Override
    public boolean isTrusted() {
        return mIsTrusted;
    }

    public ConcreteCandidate setEvaluatorId(int evaluatorId) {
        mEvaluatorId = evaluatorId;
        return this;
    }

    @Override
    public int getEvaluatorId() {
        return mEvaluatorId;
    }

    public ConcreteCandidate setEvaluatorScore(int evaluatorScore) {
        mEvaluatorScore = evaluatorScore;
        return this;
    }

    @Override
    public int getEvaluatorScore() {
        return mEvaluatorScore;
    }

    public ConcreteCandidate setLastSelectionWeight(double lastSelectionWeight) {
        mLastSelectionWeight = lastSelectionWeight;
        return this;
    }

    @Override
    public double getLastSelectionWeight() {
        return mLastSelectionWeight;
    }

    public ConcreteCandidate setCurrentNetwork(boolean isCurrentNetwork) {
        mIsCurrentNetwork = isCurrentNetwork;
        return this;
    }

    @Override
    public boolean isCurrentNetwork() {
        return mIsCurrentNetwork;
    }

    public ConcreteCandidate setCurrentBssid(boolean isCurrentBssid) {
        mIsCurrentBssid = isCurrentBssid;
        return this;
    }

    @Override
    public boolean isCurrentBssid() {
        return mIsCurrentBssid;
    }

    public ConcreteCandidate setScanRssi(int scanRssi) {
        mScanRssi = scanRssi;
        return this;
    }

    @Override
    public int getScanRssi() {
        return mScanRssi;
    }

    public ConcreteCandidate setFrequency(int frequency) {
        mFrequency = frequency;
        return this;
    }

    @Override
    public int getFrequency() {
        return mFrequency;
    }

    public ConcreteCandidate setEventStatistics(
            WifiScoreCardProto.Event event,
            WifiScoreCardProto.Signal signal) {
        mEventStatisticsMap.put(event, signal);
        return this;
    }

    @Override
    public WifiScoreCardProto.Signal getEventStatistics(WifiScoreCardProto.Event event) {
        return mEventStatisticsMap.get(event);
    }

}
