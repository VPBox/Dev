#pragma version(1)
#pragma rs java_package_name(com.example.android.rs.nbody_gl)
#pragma rs_fp_relaxed

float espSqr = .2f;                     // will be constant folded
float dt = 0.5f;                        // will be constant folded
float3 half_dt = {0.25f, 0.25f, 0.25f}; // will be constant folded

rs_allocation positions;  // float4
rs_allocation velocities; // float4
rs_allocation triangles1;  // float2
rs_allocation triangles2;  // float2

float CENTER_MASS = 1000;
float OBJECT_MAX_MASS = 800.f;
float OBJECT_MIN_MASS = 300.f;
float INITAL_VEL = 0;
float3 INITAL_DIR = {1, 1, 1};
float2 RADIUS_RANGE = {1300.f, 1900.f};

void setCenterMassPos(float x, float y, float z) {
  float4 ctr_pos = {x, y, z, CENTER_MASS};
  rsSetElementAt_float4(positions, ctr_pos, 0);
}

/* create initial conditions */
float4 __attribute__((kernel)) fill_pos(uint32_t x) {
  if (x == 0) {
    float4 out = {0, 0, 0, CENTER_MASS};
    float4 v = {0, 0, -.000001f, 0};
    rsSetElementAt_float4(velocities, v, x);
    return out;
  }
  float r = rsRand(RADIUS_RANGE.x, RADIUS_RANGE.y);
  float3 point = {rsRand(-1000.f, 1000.f),
                  rsRand(-1000.f, 1000.f),
                  rsRand(-1000.f, 1000.f)};
  point = normalize(point);
  float4 v = {rsRand(-1000.f, 1000.f),
              rsRand(-1000.f, 1000.f),
              rsRand(-1000.f, 1000.f), 0};
  v.xyz *= INITAL_DIR;
  v = normalize(v);
  v.xyz = cross(point.xyz, v.xyz);
  point *= r;
  float4 out = {point.x, point.y, point.z,
                rsRand(OBJECT_MIN_MASS, OBJECT_MAX_MASS)};

  v = INITAL_VEL * v * rsqrt(r); // - normalize(out);
  rsSetElementAt_float4(velocities, v, x);
  return out;
}


static float4  simulate(float4 in, uint32_t x ,rs_allocation triangles) {
  float3 force = {0.f, 0.f, 0.f};
  int dimx = rsAllocationGetDimX(positions);
  for (int i = 0; i < dimx; i++) {
    float4 mass = rsGetElementAt_float4(positions, i);
    float3 dir = mass.xyz - in.xyz;
    float dist = distance(mass.xyz, in.xyz);
    dist = mad(dist * dist, dist, espSqr);

    float invDistCubed = 1 / (dist);
    force += dir * (mass.w * invDistCubed);
  }
  float3 acc = force;

  float TSIZE = 0.04f;
  float4 v = rsGetElementAt_float4(velocities, x);
  float4 out = in;
  out.xyz = mad(acc, half_dt, mad(v.xyz, dt, in.xyz));
  v.xyz += acc;
  rsSetElementAt_float4(velocities, v, x);
  float4 normv;
  float4 up = {0, 0, 1, 0};
  float4 right;
  normv.xyz = normalize(v.xyz) * TSIZE * 2.f;

  right = normalize(cross(normv, up));
  up = normalize(cross(normv, right));
  right *= TSIZE;
  up *= TSIZE * 0.5f;
  float4 p;
  p.xyz = out.xyz;
  p *= 0.001f;
  int colOff = x * 4;

  float4 p1 = p + normv;
  right *= 0.866f; // sqrt(3)/2
  rsSetElementAt_float4(triangles, p1, colOff);
  float4 p2 = p - normv + right + up;
  rsSetElementAt_float4(triangles, p2, colOff + 1);
  float4 p3 = p - normv - right + up;
  rsSetElementAt_float4(triangles, p3, colOff + 2);
  float4 p4 = p - normv - up * 2;
  rsSetElementAt_float4(triangles, p4, colOff + 3);

  return out;
}
/* one pass of simulation */
float4 __attribute__((kernel)) simulate1(float4 in, uint32_t x) {
return simulate(in, x,triangles1);
}
float4 __attribute__((kernel)) simulate2(float4 in, uint32_t x) {
 return simulate(in,x,triangles2);
}
