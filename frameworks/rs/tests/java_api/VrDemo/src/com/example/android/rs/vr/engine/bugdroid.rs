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

#pragma version(1)
#pragma rs java_package_name(com.example.android.rs.vr.engine)
#pragma rs_fp_relaxed

int size;
int z;
rs_allocation volume;

static float sigmoid(float f) {
    return (float) (1 / (1 + exp(f / 2)));
}

static float pillDistance(float3 p1, float3 p2, float3 img) {

    if (dot(p2 - p1, img - p1) > 0 && dot(p1 - p2, img - p2) > 0) {
        return length(cross(img - p1, img - p2)) / length(p2 - p1);
    }
    return min(distance(p1, img), distance(p2, img));
}

static short pill(float3 p1, float3 p2, float rad, short max, float3 img) {
    return (short) (max * sigmoid(pillDistance(p1, p2, img) - rad));
}

static short cogPill(float3 p1, float3 p2, float rad, short max, float3 img) {
    float3 vec = (p1 + p2) / 2 - img;
    float angle = fabs(2 * fract(atan2pi(vec.z, vec.y) * 5) - 1);
    return (short) (max
            * sigmoid(pillDistance(p1, p2, img) - rad * (1 + angle / 2)));
}

static float cylinderDistance(float3 p1, float3 p2, float3 img) {
    float dot1 = dot(p2 - p1, img - p1);
    float dot2 = dot(p1 - p2, img - p2);
    if (dot1 > 0 && dot2 > 0) {
        return length(cross(img - p1, img - p2)) / length(p2 - p1);
    }
    return -dot1 * dot2;
}

static short cylinder(float3 p1, float3 p2, float rad, short max, float3 img) {
    return (short) (max * sigmoid(cylinderDistance(p1, p2, img) - rad));
}

static short cogCylinder(float3 p1, float3 p2, float rad, short max, float3 img) {
    float3 vec = (p1 + p2) / 2 - img;
    float angle = fabs(2 * fract(atan2pi(vec.y, vec.x) * 6) - 1);
    return (short) (max
            * sigmoid(cylinderDistance(p1, p2, img) - rad * (1 + angle / 5)));
}

static float distanceCircle(float3 center, float radius, float3 normal,
        float3 img) {
    float3 pc = img - center;
    float tmp1 = dot(normal, pc);
    tmp1 = tmp1 * tmp1;
    float tmp2 = (sqrt(length(pc) * length(pc) - tmp1) - radius);
    return tmp1 + tmp2 * tmp2;
}

static short circle(float3 center, float circleRadius, float3 normal, float rad,
        short max, float3 img) {
    return (short) (max
            * sigmoid(distanceCircle(center, circleRadius, normal, img) - rad));
}

static float distanceDisk(float3 center, float radius, float3 normal,
        float3 img) {
    float3 pc = img - center;
    float tmp1 = dot(normal, pc);
    tmp1 = tmp1 * tmp1;
    float tmp2 = (sqrt(length(pc) * length(pc) - tmp1) - radius);
    if (length(pc - dot(normal, pc) * normal) > radius) {
        return (tmp1 + tmp2 * tmp2);
    }
    return tmp1;
}

/* Unused function:
static short disk(float3 center, float circleRadius, float3 normal, float rad,
        short max, float3 img) {
    return (short) (max
            * sigmoid(distanceDisk(center, circleRadius, normal, img) - rad));
}
*/

static short cogDisk(float3 center, float circleRadius, float3 normal,
        float rad, short max, float3 img) {
    float3 vec = center - img;
    float angle = fabs(2 * fract(atan2pi(vec.y, vec.x) * 20) - 1);
    return (short) (max
            * sigmoid(
                    distanceDisk(center, circleRadius * (1 + angle / 10),
                            normal, img) - rad));
}

static float andyBody(float3 img) {
    short v = 0;
    { // body
        float3 p1 = { size * 0.5f, size * 0.5f, size * 0.3f };
        float3 p2 = { size * 0.5f, size * 0.5f, size * 0.65f };
        float radius = size * 0.22f;
        v = max(v, cylinder(p1, p2, radius, 144, img));
        p2.z = size * 0.4f;
        v = max(v, pill(p1, p2, radius, 144, img));
        float3 normal = { 0.0f, 0.0f, 1.0f };
        v -= circle(p1, radius*0.9f, normal, size * 0.05f, 144, img);
    }
    float armOffset = 0.27f;
    { // arm 1
        float3 p1 = { size * (0.5f - armOffset), size * 0.5f, size * 0.4f };
        float3 p2 = { size * (0.5f - armOffset), size * 0.5f, size * 0.57f };
        v = max(v, pill(p1, p2, size * 0.09f, 144, img));
    }
    { // arm 2
        float3 p1 = { size * (0.5f + armOffset), size * 0.5f, size * 0.4f };
        float3 p2 = { size * (0.5f + armOffset), size * 0.5f, size * 0.57f };
        v = max(v, pill(p1, p2, size * 0.09f, 144, img));
    }
    { // leg 1
        float3 p1 = { size * 0.6f, size * 0.5f, size * 0.6f };
        float3 p2 = { size * 0.6f, size * 0.5f, size * 0.8f };
        v = max(v, pill(p1, p2, size * 0.08f, 144, img));
    }
    { // leg 2
        float3 p1 = { size * 0.4f, size * 0.5f, size * 0.6f };
        float3 p2 = { size * 0.4f, size * 0.5f, size * 0.8f };
        v = max(v, pill(p1, p2, size * 0.08f, 144, img));
    }
    float3 p1 = { size * 0.5f, size * 0.5f, size * 0.3f };
    ;
    { // antenna
        float spacex = .1f;

        float3 p2 = { size * (0.5f - spacex), size * 0.5f, size * 0.07f };
        v = max(v, pill(p1, p2, size * 0.017f, 400, img));
        float3 p3 = { size * (0.5f + spacex), size * 0.5f, size * 0.07f };
        v = max(v, pill(p1, p3, size * 0.017f, 400, img));
    }
    { // eyes
        float spacex = .105f;
        float3 p2 = { size * (0.5f - spacex), size * 0.4f, size * 0.2f };
        float3 p3 = { size * (0.5f + spacex), size * 0.4f, size * 0.2f };
        v -= pill(p2, p2, size * 0.018f, 144, img);
        v -= pill(p3, p3, size * 0.018f, 144, img);
        v = max(v, pill(p1, p2, size * 0.032f, 400, img));
        v = max(v, pill(p1, p3, size * 0.032f, 400, img));
    }
    return v;

}

static float andySkeleton(float3 img) {
    short v = 0;
    { // body
        float3 p1 = { size * 0.5f, size * 0.5f, size * 0.3f };
        float radius = size * 0.15f;

        float3 normal = { 0.0f, 0.0f, 1.0f };
        for (int i = 0; i < 5; i++) {
            p1.z += size * 0.04f;
            v += circle(p1, radius, normal, size * 0.07f, 400, img);
        }
        p1.z = size * 0.3f;
        float3 p2 = { size * 0.5f, size * 0.5f, size * 0.6f };
        v = max(v, cogDisk(p2, radius * 0.7f, normal, size * 0.07f, 400, img));
        v = max(v, cogCylinder(p1, p2, size * 0.04f, 400, img));
    }

    float armOffset = 0.27f;
    {
        float3 p1 = { size * (0.5f - armOffset), size * 0.5f, size * 0.4f };
        float3 p2 = { size * (0.5f + armOffset), size * 0.5f, size * 0.4f };
        v = max(v, cogPill(p1, p2, size * 0.02f, 400, img));
    }

    { // arm 1
        float3 p1 = { size * (0.5f - armOffset), size * 0.5f, size * 0.4f };
        float3 p2 = { size * (0.5f - armOffset), size * 0.5f, size * 0.57f };
        v = max(v, pill(p1, p2, size * 0.02f, 400, img));
    }
    { // arm 2
        float3 p1 = { size * (0.5f + armOffset), size * 0.5f, size * 0.4f };
        float3 p2 = { size * (0.5f + armOffset), size * 0.5f, size * 0.57f };
        v = max(v, pill(p1, p2, size * 0.02f, 400, img));
    }
    { // leg 1
        float3 p1 = { size * 0.6f, size * 0.5f, size * 0.6f };
        float3 p2 = { size * 0.6f, size * 0.5f, size * 0.8f };
        v = max(v, pill(p1, p2, size * 0.02f, 400, img));
    }
    { // leg 2
        float3 p1 = { size * 0.4f, size * 0.5f, size * 0.6f };
        float3 p2 = { size * 0.4f, size * 0.5f, size * 0.8f };
        v = max(v, pill(p1, p2, size * 0.02f, 400, img));
    }
    return v;
}

short __attribute__((kernel)) andy(uint32_t x, uint32_t y) {
    float3 img = { x, y, z };
    float v = andyBody(img);
    v = max(v, andySkeleton(img));
    return v;

}

void __attribute__((kernel)) copy(short in, uint32_t x, uint32_t y) {
    rsSetElementAt_short(volume, in, x, y, z);
}
