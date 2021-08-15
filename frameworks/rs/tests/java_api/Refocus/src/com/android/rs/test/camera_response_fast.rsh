#ifndef CAMERA_RESPONSE_FAST_RSH
#define CAMERA_RESPONSE_FAST_RSH

// Adapt //depot/google3/geo/lightfield/refocus/image/camera_response.h|cc to
// Render Script.
//
// Implementation of a three-parameter Camera Response Function as described in:
//
//     Comparametric Equations with Practical Applications
//     in Quantigraphic Image Processing,
//     by Steve Mann, IEEE TOIP 2000.
//
// The difference here is that we use a normalized model such
// that f(0) = 0 and f(1) = 1. The function and its inverse is
// given with respect to parameters a,b,c as:
//
//     f(q) = ((e^b + 1) * q^a / (e^b * q^a + 1))^c
//     f^-1(q) = (x^(1/c) / (e^b - e^b * x^(1/c) + 1))^(1/a)
//
// The typical work flow is to first remove the response from a photograph,
// do some computation in the radiance space, then re-apply the response to
// the processed photo.

// Quantize a float in [0,1] using an integer in [0,kLUTLength-1].
static const int kLUTLength = 256;
// Default parameters of the camera response function.
static const float kDefaultA = 2.0f;
static const float kDefaultB = 4.0f;
static const float kDefaultC = 3.0f;

rs_allocation g_lut_apply_crf_float;
rs_allocation g_lut_remove_crf_float;



static inline float ApplyCRFdefault(float v) {
  const float pow_va = pow(v, kDefaultA);
  const float exp_b = exp(kDefaultB);
  const float x1 = (exp_b + 1.0f) * pow_va;
  const float x2 = exp_b * pow_va + 1.0f;
  return pow(x1 / x2, kDefaultC);
}

static inline float RemoveCRFdefault(float v) {
  const float pow_vc = pow(v, 1.0f / kDefaultC);
  const float x2 = exp(kDefaultB) * (1.0f - pow_vc) + 1.0f;
  return pow(pow_vc / x2, 1.0f / kDefaultA);
}


static inline float ApplyLUT_FloatAlloc(float value) {
  const float scaled_value = (kLUTLength - 1) * value;
  const int pos = (int)(scaled_value);
  const float delta = scaled_value - pos;
  float valPos = rsGetElementAt_float(g_lut_apply_crf_float, pos);
  float valPosPlus = rsGetElementAt_float(g_lut_apply_crf_float, pos+1);
  return valPos + delta * (valPosPlus - valPos);
}

static inline float ApplyLUT_FloatRemoveAlloc(float value) {
  const float scaled_value = (kLUTLength - 1) * value;
  const int pos = (int)(scaled_value);
  const float delta = scaled_value - pos;
  float valPos = rsGetElementAt_float(g_lut_remove_crf_float, pos);
  float valPosPlus = rsGetElementAt_float(g_lut_remove_crf_float, pos+1);
  return valPos + delta * (valPosPlus - valPos);
}

#endif
