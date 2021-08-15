// LINT: LEGACY_NAMES
syntax = "proto2";

package wireless.android.partner.adl.proto;

option java_package = "com.google.wireless.android.partner.adl.proto";
option java_outer_classname = "CameraDeviceInfoProto";

// Content of this file is generated from $(ANDROID_ROOT)/system/media/camera/doc
// Keep internal protocol buffer definition in sync with this one
// Camera related device information
// Next Id: 9
message CameraDeviceInfo {
  // Supported profiles from CamcorderProfile.hasProfile
  optional bool profile_480p = 1;
  optional bool profile_720p = 2;
  optional bool profile_1080p = 3;
  optional bool profile_cif = 4;
  optional bool profile_qcif = 5;
  optional bool profile_qvga = 6;
  optional int32 num_of_camera = 7;

  message PerCameraInfo {
    message Rational {
        optional int32 numerator = 1;
        optional int32 denominator = 2;
    }

    message Size {
        optional int32 width = 1;
        optional int32 height = 2;
    }

    message SizeF {
        optional float width = 1;
        optional float height = 2;
    }

    message Rect {
        optional int32 left = 1;
        optional int32 right = 2;
        optional int32 top = 3;
        optional int32 bottom = 4;
    }

    message StreamConfigurations {
        message StreamConfig {
            optional int32 format = 1;
            optional int32 width = 2;
            optional int32 height = 3;
            optional bool input = 4;
            optional int64 minFrameDuration = 5;
        }
        repeated StreamConfig availableStreamConfigurations = 1;
    }

    message RangeInt {
        optional int32 lower = 1;
        optional int32 upper = 2;
    }

    message RangeLong {
        optional int64 lower = 1;
        optional int64 upper = 2;
    }

    message ColorSpaceTransform {
        repeated Rational elements = 1;
    }

    message BlackLevelPattern {
        repeated int32 black_level_pattern = 1;
    }

    optional string cameraId = 1;

    // Start of codegen fields
<%\
  section_idx = 1
%>\
% for sec in find_all_sections(metadata):
## Reserve 2^16 tag id space for each section
<%\
  idx = section_idx * pow(2,16)
%>\
% for entry in find_unique_entries(sec):
% if entry.kind == 'static' and entry.visibility in ("public", "java_public"):
    ${protobuf_type(entry)} ${protobuf_name(entry)} = ${idx};
<%\
  idx += 1
%>\
% endif
% endfor
<%\
  section_idx += 1
%>\
% endfor
    // End of codegen fields
  }

  // Per camera (front/back) informations
  repeated PerCameraInfo per_camera_info = 8;
} // CameraDeviceInfo

