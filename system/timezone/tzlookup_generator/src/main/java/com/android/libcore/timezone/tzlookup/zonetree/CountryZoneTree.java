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
package com.android.libcore.timezone.tzlookup.zonetree;

import com.android.libcore.timezone.tzlookup.proto.CountryZonesFile;
import com.android.libcore.timezone.tzlookup.proto.CountryZonesFile.Country;
import com.android.libcore.timezone.tzlookup.zonetree.ZoneOffsetPeriod.ZonePeriodsKey;
import com.ibm.icu.text.TimeZoneNames;
import com.ibm.icu.util.BasicTimeZone;
import com.ibm.icu.util.TimeZone;
import com.ibm.icu.util.ULocale;

import java.io.FileWriter;
import java.io.IOException;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static java.util.stream.Collectors.toList;

/**
 * A tree that holds all the zones for a country and records how they relate to each other over
 * time.
 */
public final class CountryZoneTree {

    /** A Visitor for visiting {@link ZoneNode} trees. */
    private interface ZoneNodeVisitor extends TreeNode.Visitor<ZoneNode> {}

    /** A specialist node for zone trees. */
    private static class ZoneNode extends TreeNode<ZoneNode> {

        private final int periodOffset;
        private final List<ZoneInfo> zoneInfos;

        private int periodCount;
        private ZoneInfo primaryZoneInfo;
        private boolean priorityClash;

        ZoneNode(String id, List<ZoneInfo> zoneInfos, int periodOffset, int periodCount) {
            super(id);
            this.periodOffset = periodOffset;
            this.zoneInfos = zoneInfos;
            this.periodCount = periodCount;
            initNodePriority();
        }

        private void initNodePriority() {
            ZoneInfo priorityCandidate = null;
            int priorityCount = 0;
            for (ZoneInfo zoneInfo : zoneInfos) {
                if (priorityCandidate == null
                        || priorityCandidate.getPriority() < zoneInfo.getPriority()) {
                    priorityCandidate = zoneInfo;
                    priorityCount = 1;
                } else if (priorityCandidate.getPriority() == zoneInfo.getPriority()) {
                    priorityCount++;
                }
            }
            primaryZoneInfo = priorityCandidate;
            // If more than one ZoneInfo has the same priority as the primaryZoneInfo then we
            // can't know which one is actually the primary.
            priorityClash = priorityCount > 1;
        }

        ZoneInfo getPrimaryZoneInfo() {
            if (priorityClash) {
                throw new IllegalStateException("No primary zone for " + getId()
                        + ": priority clash (between" + getZoneInfosString() + ")");
            }
            return primaryZoneInfo;
        }

        /** {@code true} if multiple zones have the same priority. */
        boolean hasPriorityClash() {
            return priorityClash;
        }

        List<ZoneInfo> getZoneInfos() {
            return zoneInfos;
        }

        String getZoneInfosString() {
            return zoneInfos.stream()
                    .map(z -> z.getZoneId() + "(" + z.getPriority() + ")")
                    .collect(toList()).toString();
        }

        Instant getStartInstant() {
            int offset = periodOffset + periodCount - 1;
            int index = primaryZoneInfo.getZoneOffsetPeriodCount() - offset;
            return primaryZoneInfo.getZoneOffsetPeriod(index).getStartInstant();
        }

        Instant getEndInstant() {
            int index = primaryZoneInfo.getZoneOffsetPeriodCount() - periodOffset;
            return primaryZoneInfo.getZoneOffsetPeriod(index).getEndInstant();
        }

        void adjustPeriodCount(int adjustment) {
            periodCount += adjustment;
        }

        int getPeriodOffset() {
            return periodOffset;
        }

        int getPeriodCount() {
            return periodCount;
        }
    }

    private final String countryIso;

    private final ZoneNode root;
    private final Instant startInclusive;
    private final Instant endExclusive;

    private CountryZoneTree(
            String countryIso, ZoneNode root, Instant startInclusive, Instant endExclusive) {
        this.countryIso = countryIso;
        this.root = root;
        this.startInclusive = startInclusive;
        this.endExclusive = endExclusive;
    }

    /**
     * Creates a tree for the time zones for a country.
     */
    public static CountryZoneTree create(
            Country country, Instant startInclusive, Instant endExclusive) {
        return create(country, startInclusive, endExclusive, true /* compress */);
    }

    /**
     * Creates a tree for the time zones for a country which can optionally be compressed to remove
     * uninteresting nodes (which makes it easier for visualization).
     */
    public static CountryZoneTree create(
            Country country, Instant startInclusive, Instant endExclusive, boolean compress) {

        // We use the US English names for detecting time zone name clashes.
        TimeZoneNames timeZoneNames = TimeZoneNames.getInstance(ULocale.US);
        List<CountryZonesFile.TimeZoneMapping> timeZoneMappings = country.getTimeZoneMappingsList();

        // Create ZoneInfo objects for every time zone for the time range
        // startInclusive -> endExclusive.
        List<ZoneInfo> zoneInfos = new ArrayList<>();
        for (CountryZonesFile.TimeZoneMapping timeZoneMapping : timeZoneMappings) {
            int priority = timeZoneMapping.getPriority();
            BasicTimeZone basicTimeZone = getBasicTimeZone(timeZoneMapping.getId());
            ZoneInfo zoneInfo = ZoneInfo.create(
                    timeZoneNames, basicTimeZone, priority, startInclusive, endExclusive);
            zoneInfos.add(zoneInfo);
        }

        // Add additional periods to the zone infos to help merging.
        addExtraPeriodSplits(timeZoneNames, zoneInfos);

        // The algorithm constructs a tree. The root of the tree contains all ZoneInfos, and at each
        // node the ZoneInfos can be split into subsets.
        return create(country.getIsoCode(), zoneInfos, startInclusive, endExclusive, compress);
    }

    /**
     * Inserts artificial periods to the supplied {@link ZoneInfo}s to enable them to be merged
     * more easily. The artificial periods are created by splitting existing periods into multiple
     * sub-periods.
     *
     * <p>For example, some time zones do not use DST but will have changed offset or decided to
     * stop observing DST at different points in time. To find them we introduce artificial periods
     * as needed to make it easy to align and merge those zones.
     *
     * <ul>
     *     <li>Zone A moved to X hours offset from UTC in 1985 and has stayed there</li>
     *     <li>Zone B moved to X hours offset from UTC in 1984 and has stayed there</li>
     * </ul>
     *
     * <p>The simple period matching algorithm will not detect that they can be merged after 1985
     * because their respective periods using X hours offset from UTC have different start times.
     * The solution is to split Zone A into two periods: 1984 -> 1985, and 1985+. Then the
     * algorithm will find the match because both Zone A and Zone B have a period from 1985+ with
     * the same zone properties.
     */
    private static void addExtraPeriodSplits(
            TimeZoneNames timeZoneNames, List<ZoneInfo> zoneInfos) {

        // This algorithm works backwards through all the zone info periods by incrementing an
        // "offset from the end". It looks for periods that "match". In this case "match" means
        // "have the same zone properties, or *could* have the same zone properties if it were not
        // for different start times". If two or more matching zone offset periods are found with
        // different start times it splits the longer ones in two so that periods then exist with
        // the same offset with the same start and end times. It keeps going backwards through the
        // periods for all the zoneinfos until no two periods with the same offset match.

        // True if any of the zones have the matching properties. When false this is the stopping
        // condition for the loop that steps backwards through the zone offset periods.
        boolean anyZonesMatch;

        // The offset into the zone offset periods. Since we work in reverse (from the end of time),
        // offset = 1 means "the last period" offset = 2 means "the last but one period", etc.
        // We initialize at 0; it is incremented at the start of the do-while loop.
        int offsetFromEnd = 0;

        // The offsetFromEnd loop: increments offsetFromEnd and processes zone offset periods found
        // at that offset.
        do {
            offsetFromEnd += 1;

            // Reset our stopping condition. The offsetFromEnd loop will stop if this stays false.
            anyZonesMatch = false;

            // Keep track of which zoneinfos have been processed for the current offsetFromEnd so
            // we don't process them again.
            boolean[] processed = new boolean[zoneInfos.size()];

            // The splitting loop: processes groups of zoneinfos with matching periods at offset and
            // splits the matching periods as needed.
            for (int i = 0; i < zoneInfos.size(); i++) {
                if (processed[i]) {
                    // zoneinfo[i] has already been processed by a prior round of the splitting
                    // loop. Skip.
                    continue;
                }
                processed[i] = true;

                // Get zoneinfo[i] so we can use its zone properties to find all matching periods.
                ZoneInfo iZoneInfo = zoneInfos.get(i);
                ZoneOffsetPeriod iPeriod =
                        getOffsetPeriodAtOffsetFromEndOrNull(iZoneInfo, offsetFromEnd);
                if (iPeriod == null) {
                    // We've run out of periods for this zoneinfo. Skip.
                    continue;
                }

                // Pass 1: Find all zoneinfos that have a period at offsetFromEnd that matches the
                // same period at offsetFromEnd from zoneinfo[i]. Also work out the instant that
                // they would need to be split at.
                boolean[] matchAtOffsetFromEnd = new boolean[zoneInfos.size()];
                // zoneinfo[i] matches itself.
                matchAtOffsetFromEnd[i] = true;

                Instant toSplitInstant = iPeriod.getStartInstant();
                for (int j = i + 1; j < zoneInfos.size(); j++) {
                    if (processed[j]) {
                        // Don't bother to look any other zoneinfos that have previously been
                        // processed.
                        continue;
                    }

                    ZoneInfo jZoneInfo = zoneInfos.get(j);
                    ZoneOffsetPeriod jPeriod =
                            getOffsetPeriodAtOffsetFromEndOrNull(jZoneInfo, offsetFromEnd);
                    if (jPeriod == null) {
                        // We've run out of periods for this zoneinfo. Skip and make sure we don't
                        // look at it again.
                        processed[j] = true;
                        continue;
                    }
                    if (isMatch(iPeriod, jPeriod)) {
                        // At least one pair of zones have similar properties so we get to loop
                        // around the outer loop at least once more.
                        anyZonesMatch = true;

                        // Mark zoneinfo[j] as being a match for zoneinfo[i] at offset.
                        matchAtOffsetFromEnd[j] = true;
                        if (jPeriod.getStartInstant().isAfter(toSplitInstant)) {
                            // Keep track of the max start instant for pass 2.
                            toSplitInstant = jPeriod.getStartInstant();
                        }
                    }
                }

                // Pass 2: Split all the periods for the matching zonesinfos at toSplitInstant
                // (if needed).
                for (int j = i; j < zoneInfos.size(); j++) {
                    if (!matchAtOffsetFromEnd[j]) {
                        continue;
                    }
                    ZoneInfo jZoneInfo = zoneInfos.get(j);
                    int jIndex = jZoneInfo.getZoneOffsetPeriodCount() - offsetFromEnd;
                    ZoneOffsetPeriod jPeriod = jZoneInfo.getZoneOffsetPeriod(jIndex);
                    if (!jPeriod.getStartInstant().equals(toSplitInstant)) {
                        ZoneInfo.splitZoneOffsetPeriodAtTime(
                                timeZoneNames, jZoneInfo, jIndex, toSplitInstant);
                    }
                    processed[j] = true;
                }
            }
        } while (anyZonesMatch);
    }

    /**
     * Returns the {@link ZoneOffsetPeriod} with the specified offset from the end, or null if there
     * isn't one.
     */
    private static ZoneOffsetPeriod getOffsetPeriodAtOffsetFromEndOrNull(
            ZoneInfo zoneInfo, int offsetFromEnd) {
        int index = zoneInfo.getZoneOffsetPeriodCount() - offsetFromEnd;
        if (index < 0) {
            return null;
        }
        return zoneInfo.getZoneOffsetPeriod(index);
    }

    /**
     * Returns true if the one of the two {@link ZoneOffsetPeriod}s could be split and may be
     * identical to the other. The name is ignored since to know the name requires a time. If we
     * split and the name turns out not to be the same then it's ok: the different name will ensure
     * the periods will not actually be merged.
     */
    private static boolean isMatch(ZoneOffsetPeriod a, ZoneOffsetPeriod b) {
        return a.getEndInstant().equals(b.getEndInstant())
                && a.getDstOffsetMillis() == b.getDstOffsetMillis()
                && a.getRawOffsetMillis() == b.getRawOffsetMillis();
    }

    private static CountryZoneTree create(String countryIso, List<ZoneInfo> zoneInfos,
            Instant startInclusive, Instant endExclusive, boolean compress) {
        // Create a root node with all the information needed to grow the whole tree.
        ZoneNode root = new ZoneNode("0", zoneInfos, 0, 0);

        // We call growTree() to build all the branches and leaves from the root.
        growTree(root);

        // Now we compress the tree to remove unnecessary nodes if we have been asked to do so.
        if (compress) {
            compressTree(root);
        }

        // Wrap the root and return.
        return new CountryZoneTree(countryIso, root, startInclusive, endExclusive);
    }

    /**
     * Grows the zone tree from the root.
     *
     * <p>After this step, we have a tree represents a forest. The root node is just a convenience
     * for constructing and anchoring the trees in the forest. Below the root, each node
     * represents a single period of time where all the zones associated with the node agreed on
     * what the local time was and what it was called. The tree grows from the future (the root)
     * into the past (the leaves). If a node has a single child it means that the previous
     * period (the child) also had every zone in agreement. If a node has zero children it means
     * there are no more periods in the past to investigate. If a node has multiple children it
     * means that the zones disagreed in the past. Looking at a node with multiple children in
     * reverse, from the children to a parent (i.e. going forward in time), it means that
     * several zones that previously disagreed were standardized to be the same. The tzdb ID
     * exists forever, but if zones have standardized it means that fewer zones are needed to
     * represent all possible local times in a given country.
     */
    private static void growTree(ZoneNode root) {
        root.visitSelfThenChildrenRecursive((ZoneNodeVisitor) currentNode -> {
            // Increase the period offset by one so that the child will be for one period further
            // back.
            int newPeriodOffset = currentNode.getPeriodOffset() + 1;

            // Split the zoneinfo set into new sets for the new depth.
            List<ZoneInfo> zoneInfosToSplit = currentNode.getZoneInfos();

            // Generate all the child sets.
            Map<ZonePeriodsKey, List<ZoneInfo>> newSetsMap = new HashMap<>();
            for (ZoneInfo zoneInfo : zoneInfosToSplit) {
                int periodStartIndex = zoneInfo.getZoneOffsetPeriodCount() - newPeriodOffset;
                if (periodStartIndex < 0) {
                    // We've run out of ZoneOffsetPeriods. We could declare this a leaf node at this
                    // point but we continue for safety to allow the childZoneInfoCount check below.
                    continue;
                }
                // Create a zone key for the zoneInfo. We only need to look at one period each time
                // as we know all periods after this point have to agree (otherwise they wouldn't
                // have been lumped together in a single node).
                ZonePeriodsKey zoneKey =
                        zoneInfo.createZonePeriodsKey(periodStartIndex, periodStartIndex + 1);
                List<ZoneInfo> identicalTimeZones =
                        newSetsMap.computeIfAbsent(zoneKey, k -> new ArrayList<>());
                identicalTimeZones.add(zoneInfo);
            }

            // Construct any child nodes.
            int childZoneInfoCount = 0;
            int i = 1;
            for (Map.Entry<ZonePeriodsKey, List<ZoneInfo>> newSetEntry : newSetsMap.entrySet()) {
                List<ZoneInfo> newSet = newSetEntry.getValue();
                childZoneInfoCount += newSet.size();
                // The child ID is just the {parent ID}.{child number} so we create an easy-to-debug
                // address.
                String childId = currentNode.getId() + "." + i;
                ZoneNode e = new ZoneNode(childId, newSet, newPeriodOffset, 1 /* periodCount */);
                currentNode.addChild(e);
                i++;
            }

            // Assertion: a node should either have no nodes (be a leaf) or all zones should have
            // been split between the children.
            if (childZoneInfoCount != 0 && childZoneInfoCount != zoneInfosToSplit.size()) {
                // This implies some kind of data issue.
                throw new IllegalStateException();
            }
        });
    }

    /**
     * Removes uninteresting nodes from the tree by merging them with their children where possible.
     * Uninteresting nodes are those that have a single child; having a single child implies the
     * node and its child have the same offsets and other information (they're just for an earlier
     * period). The resulting merged node has the same zones and depthInTree but a larger period
     * count.
     */
    private static void compressTree(ZoneNode root) {
        class CompressionVisitor implements ZoneNodeVisitor {

            @Override
            public void visit(ZoneNode node) {
                if (node.isRoot()) {
                    // Ignore the root.
                    return;
                }

                // If there's one child then we can compress it into node.
                if (node.getChildrenCount() == 1) {
                    ZoneNode child = node.getChildren().get(0);

                    // Remove the child from node.
                    node.removeChild(child);

                    // The child may also have descendants with a single child, so handle those too.
                    int periodCountAdjustment = child.getPeriodCount();
                    ZoneNode descendant = child;
                    while (descendant.getChildrenCount() == 1) {
                        descendant = descendant.getChildren().get(0);
                        periodCountAdjustment += descendant.getPeriodCount();
                    }

                    // Remove the children from descendant and add them to node.
                    for (ZoneNode newChild : descendant.getChildren()) {
                        descendant.removeChild(newChild);
                        node.addChild(newChild);
                    }

                    // Add the removed descendant's period count to node.
                    node.adjustPeriodCount(periodCountAdjustment);
                }
            }
        }
        root.visitSelfThenChildrenRecursive(new CompressionVisitor());
    }

    /** Validates the tree has no nodes with priority clashes, returns a list of issues. */
    public List<String> validateNoPriorityClashes() {
        class ValidationVisitor implements ZoneNodeVisitor {
            private final List<String> issues = new ArrayList<>();

            @Override
            public void visit(ZoneNode node) {
                if (node.isRoot()) {
                    // Ignore the root, it's not a "real" node and will usually clash in countries
                    // where there's more than one zone.
                    return;
                }

                if (node.hasPriorityClash()) {
                    String issue = node.getZoneInfosString();
                    issues.add(issue);
                }
            }

            public List<String> getIssues() {
                return issues;
            }
        }

        ValidationVisitor visitor = new ValidationVisitor();
        root.visitSelfThenChildrenRecursive(visitor);
        return visitor.getIssues();
    }

    /**
     * Creates a {@link CountryZoneUsage} object from the tree.
     */
    public CountryZoneUsage calculateCountryZoneUsage(Instant notAfterCutOff) {
        class CountryZoneVisibilityVisitor implements ZoneNodeVisitor {
            private final CountryZoneUsage zoneUsage = new CountryZoneUsage(countryIso);

            @Override
            public void visit(ZoneNode node) {
                // We ignore the root.
                if (node.isRoot()) {
                    return;
                }

                if (node.hasPriorityClash()) {
                    throw new IllegalStateException(
                            "Cannot calculate zone usage with priority clashes present");
                }

                Instant endInstant = node.getEndInstant();
                if (!node.isLeaf()) {
                    ZoneInfo primaryZone = node.getPrimaryZoneInfo();
                    addZoneEntryIfMissing(endInstant, primaryZone);
                } else {
                    // In some rare cases (e.g. Canada: Swift_Current and Creston) zones have agreed
                    // completely since 1970 so some leaves may have multiple zones. So, attempt to
                    // add all zones for leaves, not just the primary.
                    for (ZoneInfo zoneInfo : node.getZoneInfos()) {
                        addZoneEntryIfMissing(endInstant, zoneInfo);
                    }
                }
            }

            private void addZoneEntryIfMissing(Instant endInstant, ZoneInfo zoneInfo) {
                String zoneId = zoneInfo.getZoneId();
                if (!notAfterCutOff.isAfter(endInstant)) {
                    // notAfterCutOff <= endInstant
                    endInstant = null;
                }
                if (!zoneUsage.hasEntry(zoneId)) {
                    zoneUsage.addEntry(zoneId, endInstant);
                }
            }

            private CountryZoneUsage getCountryZoneUsage() {
                return zoneUsage;
            }
        }

        CountryZoneVisibilityVisitor visitor = new CountryZoneVisibilityVisitor();
        root.visitSelfThenChildrenRecursive(visitor);
        return visitor.getCountryZoneUsage();
    }

    /**
     * Creates a Graphviz file for visualizing the tree.
     */
    public void createGraphvizFile(String outputFile) throws IOException {
        class DotFileVisitor implements ZoneNodeVisitor {
            private StringBuilder graphStringBuilder = new StringBuilder();

            @Override
            public void visit(ZoneNode node) {
                if (node.isRoot()) {
                    // Don't draw the root - make the tree look like a forest.
                    return;
                }

                String nodeName = enquote(node.getId());

                // Draw the node.
                Instant startInstant = node.getStartInstant();
                Instant endInstant = node.getEndInstant();
                boolean priorityClash = node.hasPriorityClash();

                String fromTimestamp = startInstant.toString();
                String toTimestamp = endInstant.toString();
                String optionalColor = priorityClash ? ",color=\"red\"" : "";
                String label = node.getZoneInfosString()
                        + "\nFrom=" + fromTimestamp + " to " + toTimestamp
                        + "\nPeriod count=" + node.getPeriodCount();
                if (node.getPeriodCount() == 1) {
                    ZoneInfo arbitraryZoneInfo = node.getZoneInfos().get(0);
                    int periodIndex =
                            arbitraryZoneInfo.getZoneOffsetPeriodCount() - node.getPeriodOffset();
                    ZoneOffsetPeriod zoneOffsetPeriod =
                            arbitraryZoneInfo.getZoneOffsetPeriod(periodIndex);
                    label += "\nrawOffset=" + durationString(zoneOffsetPeriod.getRawOffsetMillis());
                    label += "\ndstOffset=" + durationString(zoneOffsetPeriod.getDstOffsetMillis());
                    label += "\nname=" + zoneOffsetPeriod.getName();
                }
                writeLine(nodeName + "[label=" + enquote(label) + optionalColor + "];");

                // Link the node to its children.
                for (ZoneNode child : node.getChildren()) {
                    writeLine(nodeName + " -> " + enquote(child.getId()) + ";");
                }
            }

            private String durationString(int durationMillis) {
                return Duration.ofMillis(durationMillis).toString();
            }

            private String enquote(String toQuote) {
                return "\"" + toQuote + "\"";
            }

            private void writeLine(String s) {
                graphStringBuilder.append(s);
                graphStringBuilder.append('\n');
            }
        }

        DotFileVisitor dotFileVisitor = new DotFileVisitor();
        root.visitSelfThenChildrenRecursive(dotFileVisitor);

        try (FileWriter fileWriter = new FileWriter(outputFile)) {
            writeLine(fileWriter, "strict digraph " + countryIso + " {");
            writeLine(fileWriter, dotFileVisitor.graphStringBuilder.toString());
            writeLine(fileWriter, "}");
        }
    }

    private static void writeLine(Appendable appendable, String s) throws IOException {
        appendable.append(s);
        appendable.append('\n');
    }

    /**
     * Returns an ICU {@link BasicTimeZone} with the specified ID or throws an exception if there
     * isn't one.
     */
    private static BasicTimeZone getBasicTimeZone(String zoneId) {
        TimeZone timeZone = TimeZone.getTimeZone(zoneId);
        if (isInvalidZone(timeZone)) {
            throw new IllegalArgumentException(
                    "Unknown or unexpected type for zone id: " + timeZone.getID());
        }
        return (BasicTimeZone) timeZone;
    }

    private static boolean isInvalidZone(TimeZone timeZone) {
        return !(timeZone instanceof BasicTimeZone)
                || timeZone.getID().equals(TimeZone.UNKNOWN_ZONE_ID);
    }
}
