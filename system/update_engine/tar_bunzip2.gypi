#
# Copyright (C) 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
{
  'variables': {
    'out_dir': '<(SHARED_INTERMEDIATE_DIR)/<(image_out_dir)',
  },
  'rules': [
    {
      'rule_name': 'tar-bunzip2',
      'extension': 'bz2',
      'outputs': [
        # The .flag file is used to mark the timestamp of the file extraction
        # and re-run this action if a new .bz2 file is generated.
        '<(out_dir)/<(RULE_INPUT_ROOT).flag',
      ],
      'action': [
        'sh',
        '-c',
        'tar -xvf "<(RULE_INPUT_PATH)" -C "<(out_dir)" && touch <(out_dir)/<(RULE_INPUT_ROOT).flag',
      ],
      'msvs_cygwin_shell': 0,
      'process_outputs_as_sources': 1,
      'message': 'Unpacking file <(RULE_INPUT_PATH)',
    },
  ],
}
