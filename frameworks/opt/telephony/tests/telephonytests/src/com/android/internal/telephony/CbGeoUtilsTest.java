/*
 * Copyright (C) 2019 The Android Open Source Project
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

package com.android.internal.telephony;

import static com.google.common.truth.Truth.assertThat;

import com.android.internal.telephony.CbGeoUtils.Circle;
import com.android.internal.telephony.CbGeoUtils.Geometry;
import com.android.internal.telephony.CbGeoUtils.LatLng;
import com.android.internal.telephony.CbGeoUtils.Polygon;

import org.junit.Test;

import java.util.Arrays;
import java.util.List;

public class CbGeoUtilsTest {
    @Test
    public void testDistanceBetweenTwoLatLng() {
        LatLng latlng1 = new LatLng(37.428402, -122.084238);
        LatLng latlng2 = new LatLng(37.399525, -121.946445);
        double distInMeter = latlng1.distance(latlng2);
        assertThat(distInMeter).isWithin(0.1).of(12586.1);
    }

    @Test
    public void testParseLatLngFromString() {
        LatLng latlng = CbGeoUtils.parseLatLngFromString("-80.1234, 179.5678");
        assertThat(latlng.lat).isWithin(CbGeoUtils.EPS).of(-80.1234);
        assertThat(latlng.lng).isWithin(CbGeoUtils.EPS).of(179.5678);
    }

    @Test
    public void testEncodeGeometries() {
        List<Geometry> geo = Arrays.asList(
                new Polygon(Arrays.asList(
                        new LatLng(11.11, 22.22),
                        new LatLng(33.33, 44.44),
                        new LatLng(55.55, -56.0))),
                new Circle(new LatLng(15.123, 123.456), 314));

        // Encode a list of geometry objects.
        String encodedStr = CbGeoUtils.encodeGeometriesToString(geo);

        // Decode and verify the geometry objects.
        List<Geometry> decodedGeo = CbGeoUtils.parseGeometriesFromString(encodedStr);
        assertThat(decodedGeo.size()).isEqualTo(2);

        // The first one is polygon with three vertexes
        assertThat(geo.get(0)).isInstanceOf(CbGeoUtils.Polygon.class);
        CbGeoUtils.Polygon polygon = (Polygon) geo.get(0);
        List<LatLng> vertexes = polygon.getVertices();
        assertThat(vertexes.size()).isEqualTo(3);
        LatLng p = vertexes.get(0);
        assertThat(p.lat).isWithin(CbGeoUtils.EPS).of(11.11);
        assertThat(p.lng).isWithin(CbGeoUtils.EPS).of(22.22);
        p = vertexes.get(1);
        assertThat(p.lat).isWithin(CbGeoUtils.EPS).of(33.33);
        assertThat(p.lng).isWithin(CbGeoUtils.EPS).of(44.44);
        p = vertexes.get(2);
        assertThat(p.lat).isWithin(CbGeoUtils.EPS).of(55.55);
        assertThat(p.lng).isWithin(CbGeoUtils.EPS).of(-56.0);

        // THe second one is circle.
        assertThat(geo.get(1)).isInstanceOf(CbGeoUtils.Circle.class);
        Circle circle = (Circle) geo.get(1);
        p = circle.getCenter();
        assertThat(p.lat).isWithin(CbGeoUtils.EPS).of(15.123);
        assertThat(p.lng).isWithin(CbGeoUtils.EPS).of(123.456);
        assertThat(circle.getRadius()).isWithin(CbGeoUtils.EPS).of(314);
    }

    @Test
    public void testParseGeometriesFromString() {
        String geometriesStr =
                "polygon|11.11,22.22|33.33, 44.44| 55.55, -56.0; circle|15.123, 123.456|314";
        List<Geometry> geo = CbGeoUtils.parseGeometriesFromString(geometriesStr);

        assertThat(geo.size()).isEqualTo(2);

        // The first one is polygon with three vertexes
        assertThat(geo.get(0)).isInstanceOf(CbGeoUtils.Polygon.class);
        CbGeoUtils.Polygon polygon = (Polygon) geo.get(0);
        List<LatLng> vertexes = polygon.getVertices();
        assertThat(vertexes.size()).isEqualTo(3);
        LatLng p = vertexes.get(0);
        assertThat(p.lat).isWithin(CbGeoUtils.EPS).of(11.11);
        assertThat(p.lng).isWithin(CbGeoUtils.EPS).of(22.22);
        p = vertexes.get(1);
        assertThat(p.lat).isWithin(CbGeoUtils.EPS).of(33.33);
        assertThat(p.lng).isWithin(CbGeoUtils.EPS).of(44.44);
        p = vertexes.get(2);
        assertThat(p.lat).isWithin(CbGeoUtils.EPS).of(55.55);
        assertThat(p.lng).isWithin(CbGeoUtils.EPS).of(-56.0);

        // THe second one is circle.
        assertThat(geo.get(1)).isInstanceOf(CbGeoUtils.Circle.class);
        Circle circle = (Circle) geo.get(1);
        p = circle.getCenter();
        assertThat(p.lat).isWithin(CbGeoUtils.EPS).of(15.123);
        assertThat(p.lng).isWithin(CbGeoUtils.EPS).of(123.456);
        assertThat(circle.getRadius()).isWithin(CbGeoUtils.EPS).of(314);
    }

    @Test
    public void testPointInPolygon() {
        List<LatLng> vertex = Arrays.asList(
                new LatLng(-1, 0),
                new LatLng(0, 1),
                new LatLng(1, 0),
                new LatLng(0, -1));
        Polygon polygon = new Polygon(vertex);

        assertThat(polygon.contains(new LatLng(0, 0))).isTrue();

        assertThat(polygon.contains(new LatLng(0.5, 0.5))).isTrue();

        assertThat(polygon.contains(new LatLng(-2, -1))).isFalse();

        assertThat(polygon.contains(new LatLng(1.0001, 1.0001))).isFalse();
    }

    @Test
    public void testPointInPolygon_crossing180thMeridian() {
        List<LatLng> vertices = Arrays.asList(
                new LatLng(68.7153, 176.76038),
                new LatLng(68.69982, -179.61491),
                new LatLng(68.09107, -177.87357),
                new LatLng(67.51155, -179.73498),
                new LatLng(66.69957, -178.76818),
                new LatLng(66.7126, 177.43054),
                new LatLng(67.95902, 178.33927));

        Polygon polygon = new Polygon(vertices);

        // Verify the points are inside the polygon(manually check in google map).
        assertThat(polygon.contains(new LatLng(68.65294, 177.16205))).isTrue();
        assertThat(polygon.contains(new LatLng(68.60522, 178.83294))).isTrue();
        assertThat(polygon.contains(new LatLng(68.63098, -179.90943))).isTrue();
        assertThat(polygon.contains(new LatLng(67.51219, -179.74427))).isTrue();
        assertThat(polygon.contains(new LatLng(67.91933, 179.46802))).isTrue();

        // Verify the points are outside the polygon(manually check in google map).
        assertThat(polygon.contains(new LatLng(67.50498, -179.48277))).isFalse();
        assertThat(polygon.contains(new LatLng(67.95463, 178.23206))).isFalse();
    }

    @Test
    public void testPointInPolygon_crossing0thMeridian() {
        List<LatLng> vertices = Arrays.asList(
                new LatLng(51.79327, -1.00339),
                new LatLng(51.79327, 1.00339),
                new LatLng(49.79327, 1.00339),
                new LatLng(49.79327, -2.1234));

        Polygon polygon = new Polygon(vertices);

        // Verify the points are inside the polygon(manually check on google map).
        assertThat(polygon.contains(new LatLng(51.78091, 0.97431))).isTrue();
        assertThat(polygon.contains(new LatLng(49.97102, 0.72206))).isTrue();
        assertThat(polygon.contains(new LatLng(50.82538, -0.17881))).isTrue();
        assertThat(polygon.contains(new LatLng(51.50735, -0.12775))).isTrue();

        // Verify the points are outside the polygon(manually check on google map).
        assertThat(polygon.contains(new LatLng(51.28268, 1.06951))).isFalse();
        assertThat(polygon.contains(new LatLng(50.30352, -1.94073))).isFalse();
        assertThat(polygon.contains(new LatLng(51.74758, -1.27057))).isFalse();
    }

    @Test
    public void testPointInCircle() {
        Circle circle = new Circle(new LatLng(37.42331, -122.08636), 500);

        // ~ 307 meters
        assertThat(circle.contains(new LatLng(37.42124, -122.08405))).isTrue();

        // ~ 451 meters
        assertThat(circle.contains(new LatLng(37.42093, -122.08222))).isTrue();

        // ~ 622 meters
        assertThat(circle.contains(new LatLng(37.41807, -122.08389))).isFalse();
    }
}
