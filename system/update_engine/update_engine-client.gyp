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
  'targets': [
    # update_engine client library generated headers. Used by other daemons and
    # by the update_engine_client console program to interact with
    # update_engine.
    {
      'target_name': 'libupdate_engine-client-headers',
      'type': 'none',
      'actions': [
        {
          'action_name': 'update_engine_client-dbus-proxies',
          'variables': {
            'dbus_service_config': 'dbus_bindings/dbus-service-config.json',
            'proxy_output_file': 'include/update_engine/dbus-proxies.h',
            'mock_output_file': 'include/update_engine/dbus-proxy-mocks.h',
            'proxy_path_in_mocks': 'update_engine/dbus-proxies.h',
          },
          'sources': [
            'dbus_bindings/org.chromium.UpdateEngineInterface.dbus-xml',
          ],
          'includes': ['../../../platform2/common-mk/generate-dbus-proxies.gypi'],
        },
      ],
    },
  ],
}
