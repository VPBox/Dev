## -*- coding: utf-8 -*-
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

/**
 * @addtogroup Camera
 * @{
 */

/**
 * @file NdkCameraMetadataTags.h
 */

/*
 * This file defines an NDK API.
 * Do not remove methods.
 * Do not change method signatures.
 * Do not change the value of constants.
 * Do not change the size of any of the classes defined in here.
 * Do not reference types that are not part of the NDK.
 * Do not #include files that aren't part of the NDK.
 */

#ifndef _NDK_CAMERA_METADATA_TAGS_H
#define _NDK_CAMERA_METADATA_TAGS_H

#include <sys/cdefs.h>

__BEGIN_DECLS

#if __ANDROID_API__ >= 24
<%!
  from metadata_helpers import csym
  def annotated_type(entry):
    type = entry.type
    if entry.container == 'array':
       type += '[' + '*'.join(entry.container_sizes) + ']'
    if entry.enum:
       type += ' (acamera_metadata_enum_%s_t)'%(csym(entry.name).lower())

    return type
%>\
\

typedef enum acamera_metadata_section {
  % for i in find_all_sections(metadata):
    ${ndk(path_name(i)) | csym},
  % endfor
    ACAMERA_SECTION_COUNT,

    ACAMERA_VENDOR = 0x8000
} acamera_metadata_section_t;

/**
 * Hierarchy positions in enum space.
 */
typedef enum acamera_metadata_section_start {
  % for i in find_all_sections(metadata):
    ${ndk(path_name(i)) + '.start' | csym,ljust(30)} = ${ndk(path_name(i)) | csym,pad(64)} << 16,
  % endfor
    ACAMERA_VENDOR_START           = ACAMERA_VENDOR            << 16
} acamera_metadata_section_start_t;

/**
 * Main enum for camera metadata tags.
 */
typedef enum acamera_metadata_tag {
    % for sec in find_all_sections(metadata):
<%
      entries = remove_synthetic(find_unique_entries(sec))
      skip_sec = all(e.applied_ndk_visible == "false" for e in entries)
      if skip_sec:
        continue
%>\
      % for idx,entry in enumerate(remove_synthetic(find_unique_entries(sec))):
        % if entry.applied_ndk_visible == "true":
          % if entry.deprecated:
    ${ndk(entry.name) + " = " | csym,ljust(60)}// Deprecated! DO NOT USE
          % else:
            % if entry.description or entry.details:
    /**
              % if entry.description:
${entry.description | ndkdoc(metadata)}\
              % endif
     *
     * <p>Type: ${annotated_type(entry)}</p>
     *
     * <p>This tag may appear in:
     * <ul>
              % if metadata.is_entry_this_kind(entry, 'static'):
     *   <li>ACameraMetadata from ACameraManager_getCameraCharacteristics</li>
              % endif
              % if metadata.is_entry_this_kind(entry, 'dynamic'):
     *   <li>ACameraMetadata from ACameraCaptureSession_captureCallback_result callbacks</li>
              % endif
              % if metadata.is_entry_this_kind(entry, 'controls'):
     *   <li>ACaptureRequest</li>
              % endif
     * </ul></p>
     *
              % if entry.details:
${entry.applied_ndk_details | ndkdoc(metadata)}\
              % endif
     */
            % endif
    ${ndk(entry.name) + " = " | csym,ljust(60)}// ${annotated_type(entry)}
          % endif
          % if idx == 0:
            ${ndk(path_name(find_parent_section(entry))) | csym}_START,
          % else:
            ${ndk(path_name(find_parent_section(entry))) | csym}_START + ${idx},
          % endif
        % endif
      % endfor
    ${ndk(path_name(sec)) | csym}_END,

    %endfor
} acamera_metadata_tag_t;

/**
 * Enumeration definitions for the various entries that need them
 */

% for sec in find_all_sections(metadata):
  % for entry in filter_ndk_visible(remove_synthetic(find_unique_entries(sec))):
    % if entry.enum:
// ${ndk(entry.name) | csym}
typedef enum acamera_metadata_enum_${csym(ndk(entry.name)).lower()} {
<%
      i = 0
%>\
      % for val in entry.enum.values:
        % if val.ndk_hidden:
<%
          print "  WARNING: {}_{} is marked as hidden".format(csym(ndk(entry.name)), val.name) + \
                " enum in NDK. Please double check this value is properly hidden" +  \
                " in NDK API implementation"
%>\
        % endif
        % if val.hidden or val.ndk_hidden:
          % if val.id:
<%
            i = int(val.id, 0) + 1
            continue
%>\
          % else:
<%
            i += 1
            continue
%>\
          % endif
        % endif
        % if (val.notes or val.deprecated):
    /**
          % if val.notes:
${val.notes | ndkdoc(metadata)}\
          % endif
          % if val.ndk_notes:
${val.ndk_notes | ndkdoc(metadata)}\
          % endif
          % if val.deprecated:
     *
     * <b>Deprecated</b>: please refer to this API documentation to find the alternatives
          % endif
     */
        % endif
        % if val.id:
    ${'%s_%s'%(csym(ndk(entry.name)), val.name) | pad(70)} = ${val.id},
<%
          i = int(val.id, 0)
%>\
        % else:
    ${'%s_%s'%(csym(ndk(entry.name)), val.name) | pad(70)} = ${i},
        % endif
<%
        i += 1
%>
      % endfor
} acamera_metadata_enum_${csym(entry.name).lower()}_t;

    % endif
  % endfor

% endfor
#endif /* __ANDROID_API__ >= 24 */

__END_DECLS

#endif /* _NDK_CAMERA_METADATA_TAGS_H */

/** @} */
