## -*- coding: utf-8 -*-
##
## Copyright (C) 2015 The Android Open Source Project
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##      http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##
\
## Generate a list of only Static, Controls, or Dynamic properties.
<%def name="single_kind_keys(kind_name)">\
% for outer_namespace in metadata.outer_namespaces: ## assumes single 'android' namespace
  % for section in outer_namespace.sections:
    % if section.find_first(lambda x: isinstance(x, metadata_model.Entry) and x.kind == kind_name) and \
         any_visible(section, kind_name, ('public','ndk_public') ):
      % for inner_namespace in get_children_by_filtering_kind(section, kind_name, 'namespaces'):
## We only support 1 level of inner namespace, i.e. android.a.b and android.a.b.c works, but not android.a.b.c.d
## If we need to support more, we should use a recursive function here instead.. but the indentation gets trickier.
        % for entry in filter_visibility(inner_namespace.merged_entries, ('public','ndk_public')):
          % if not entry.synthetic:
        case ${ndk(entry.name) | csym}:
          % else:
            assert(False),"A synthetic key should not present in NDK!"
          % endif
       % endfor
    % endfor
    % for entry in filter_visibility( \
        get_children_by_filtering_kind(section, kind_name, 'merged_entries'), \
                                         ('public','ndk_public')):
      % if not entry.synthetic:
        case ${ndk(entry.name) | csym}:
      % endif
    % endfor
    % endif
  % endfor
% endfor
</%def>\
/*@O~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~
 * The key entries below this point are generated from metadata
 * definitions in /system/media/camera/docs. Do not modify by hand or
 * modify the comment blocks at the start or end.
 *~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~*/

bool
ACameraMetadata::isCaptureRequestTag(const uint32_t tag) {
    // Skip check for vendor keys
    if (isVendorTag(tag)) {
        return true;
    }

    switch (tag) {
${single_kind_keys("controls")}\
            return true;
        default:
            return false;
    }
}

// System tags that should be hidden from users
std::unordered_set<uint32_t> ACameraMetadata::sSystemTags ({
    % for sec in find_all_sections(metadata):
      % for entry in remove_synthetic(find_unique_entries(sec)):
        % if entry.applied_visibility == "system":
    ${entry.name | csym},
        % endif
      % endfor
    %endfor
});

/*~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~
 * End generated code
 *~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~O@*/
