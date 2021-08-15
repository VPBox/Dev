#ifndef CAMERA_RESPONSE_RSH
#define CAMERA_RESPONSE_RSH

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

typedef struct CameraResponse {
  float a;
  float b;
  float c;
  float lut_apply_crf_float[kLUTLength + 1];
  float lut_remove_crf_float[kLUTLength + 1];
  int lut_apply_crf_int[kLUTLength];
  int lut_remove_crf_int[kLUTLength];
} CameraResponse_t;

static inline float ApplyCRF(float v, const CameraResponse_t* camera_response) {
  const float pow_va = pow(v, camera_response->a);
  const float exp_b = exp(camera_response->b);
  const float x1 = (exp_b + 1.0f) * pow_va;
  const float x2 = exp_b * pow_va + 1.0f;
  return pow(x1 / x2, camera_response->c);
}

static inline float RemoveCRF(float v,
                              const CameraResponse_t* camera_response) {
  const float pow_vc = pow(v, 1.0f / camera_response->c);
  const float x2 = exp(camera_response->b) * (1.0f - pow_vc) + 1.0f;
  return pow(pow_vc / x2, 1.0f / camera_response->a);
}

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

static inline void ComputeLUTs(CameraResponse_t* camera_response) {
  for (int i = 0; i < kLUTLength; ++i) {
    const float value = i / (float)(kLUTLength - 1);

    const float y = ApplyCRF(value, camera_response);
    camera_response->lut_apply_crf_float[i] = y;
    camera_response->lut_apply_crf_int[i] = round((kLUTLength - 1) * y);

    const float x = RemoveCRF(value, camera_response);
    camera_response->lut_remove_crf_float[i] = x;
    camera_response->lut_remove_crf_int[i] = round((kLUTLength - 1) * x);
  }

  // Add a boundary to correctly deal with the max value in ApplyLUT.
  camera_response->lut_apply_crf_float[kLUTLength] =
      camera_response->lut_apply_crf_float[kLUTLength - 1];
  camera_response->lut_remove_crf_float[kLUTLength] =
      camera_response->lut_remove_crf_float[kLUTLength - 1];
}

static inline float ApplyLUT_Float(float value, float lut[kLUTLength + 1]) {
  const float scaled_value = (kLUTLength - 1) * value;
  const int pos = (int)(scaled_value);
  const float delta = scaled_value - pos;
  return lut[pos] + delta * (lut[pos + 1] - lut[pos]);
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

static inline int ApplyLUT_Int(int value, int lut[kLUTLength]) {
  return lut[value];
}

static inline void InitializeDefaultCameraResponse(
    CameraResponse_t* camera_response) {
  camera_response->a = kDefaultA;
  camera_response->b = kDefaultB;
  camera_response->c = kDefaultC;
  ComputeLUTs(camera_response);
}

#endif
