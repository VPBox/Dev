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

# TODO: Rename these files to pass this check.
# gyplint: disable=GypLintSourceFileNames
{
  'variables': {
    'USE_chrome_network_proxy': '1',
    'USE_chrome_kiosk_app': '1',
  },
  'target_defaults': {
    'variables': {
      'deps': [
        'libbrillo-<(libbase_ver)',
        'libchrome-<(libbase_ver)',
        # system_api depends on protobuf (or protobuf-lite). It must appear
        # before protobuf here or the linker flags won't be in the right
        # order.
        'system_api',
        'protobuf-lite',
      ],
      # The -DUSE_* flags are passed from platform2.py. We use sane defaults
      # here when these USE flags are not defined. You can set the default value
      # for the USE flag in the ebuild.
      'USE_hwid_override%': '0',
    },
    'cflags': [
      '-g',
      '-ffunction-sections',
      '-Wall',
      '-Wextra',
      '-Werror',
      '-Wno-unused-parameter',
    ],
    'cflags_cc': [
      '-fno-strict-aliasing',
      '-Wnon-virtual-dtor',
    ],
    'ldflags': [
      '-Wl,--gc-sections',
    ],
    'defines': [
      '__CHROMEOS__',
      '_FILE_OFFSET_BITS=64',
      '_POSIX_C_SOURCE=199309L',
      'USE_BINDER=<(USE_binder)',
      'USE_DBUS=<(USE_dbus)',
      'USE_FEC=0',
      'USE_HWID_OVERRIDE=<(USE_hwid_override)',
      'USE_CHROME_KIOSK_APP=<(USE_chrome_kiosk_app)',
      'USE_CHROME_NETWORK_PROXY=<(USE_chrome_network_proxy)',
      'USE_MTD=<(USE_mtd)',
      'USE_OMAHA=1',
      'USE_SHILL=1',
    ],
    'include_dirs': [
      # We need this include dir because we include all the local code as
      # "update_engine/...".
      '<(platform2_root)/../aosp/system',
      '<(platform2_root)/../aosp/system/update_engine/client_library/include',
    ],
  },
  'targets': [
    # Protobufs.
    {
      'target_name': 'update_metadata-protos',
      'type': 'static_library',
      'variables': {
        'proto_in_dir': '.',
        'proto_out_dir': 'include/update_engine',
      },
      'sources': [
        'update_metadata.proto',
      ],
      'includes': ['../../../platform2/common-mk/protoc.gypi'],
    },
    # Chrome D-Bus bindings.
    {
      'target_name': 'update_engine-dbus-adaptor',
      'type': 'none',
      'variables': {
        'dbus_adaptors_out_dir': 'include/dbus_bindings',
        'dbus_xml_extension': 'dbus-xml',
      },
      'sources': [
        'dbus_bindings/org.chromium.UpdateEngineInterface.dbus-xml',
      ],
      'includes': ['../../../platform2/common-mk/generate-dbus-adaptors.gypi'],
    },
    {
      'target_name': 'update_engine-dbus-kiosk-app-client',
      'type': 'none',
      'actions': [{
        'action_name': 'update_engine-dbus-kiosk-app-client-action',
        'variables': {
          'mock_output_file': 'include/kiosk-app/dbus-proxy-mocks.h',
          'proxy_output_file': 'include/kiosk-app/dbus-proxies.h',
        },
        'sources': [
          'dbus_bindings/org.chromium.KioskAppService.dbus-xml',
        ],
        'includes': ['../../../platform2/common-mk/generate-dbus-proxies.gypi'],
      }],
    },
    # The payload application component and common dependencies.
    {
      'target_name': 'libpayload_consumer',
      'type': 'static_library',
      'dependencies': [
        'update_metadata-protos',
      ],
      # TODO(deymo): Remove unused dependencies once we stop including files
      # from the root directory.
      'variables': {
        'exported_deps': [
          'libcrypto',
          'xz-embedded',
          'libbspatch',
          'libpuffpatch',
        ],
        'deps': ['<@(exported_deps)'],
      },
      'all_dependent_settings': {
        'variables': {
          'deps': [
            '<@(exported_deps)',
          ],
        },
      },
      'link_settings': {
        'variables': {
          'deps': [
            '<@(exported_deps)',
          ],
        },
        'libraries': [
          '-lbz2',
          '-lrt',
        ],
      },
      'sources': [
        'common/action_processor.cc',
        'common/boot_control_stub.cc',
        'common/clock.cc',
        'common/constants.cc',
        'common/cpu_limiter.cc',
        'common/error_code_utils.cc',
        'common/hash_calculator.cc',
        'common/http_common.cc',
        'common/http_fetcher.cc',
        'common/hwid_override.cc',
        'common/multi_range_http_fetcher.cc',
        'common/platform_constants_chromeos.cc',
        'common/prefs.cc',
        'common/proxy_resolver.cc',
        'common/subprocess.cc',
        'common/terminator.cc',
        'common/utils.cc',
        'payload_consumer/bzip_extent_writer.cc',
        'payload_consumer/cached_file_descriptor.cc',
        'payload_consumer/delta_performer.cc',
        'payload_consumer/download_action.cc',
        'payload_consumer/extent_reader.cc',
        'payload_consumer/extent_writer.cc',
        'payload_consumer/file_descriptor.cc',
        'payload_consumer/file_descriptor_utils.cc',
        'payload_consumer/file_writer.cc',
        'payload_consumer/filesystem_verifier_action.cc',
        'payload_consumer/install_plan.cc',
        'payload_consumer/mount_history.cc',
        'payload_consumer/payload_constants.cc',
        'payload_consumer/payload_metadata.cc',
        'payload_consumer/payload_verifier.cc',
        'payload_consumer/postinstall_runner_action.cc',
        'payload_consumer/verity_writer_stub.cc',
        'payload_consumer/xz_extent_writer.cc',
      ],
      'conditions': [
        ['USE_mtd == 1', {
          'sources': [
            'payload_consumer/mtd_file_descriptor.cc',
          ],
          'link_settings': {
            'libraries': [
              '-lmtdutils',
            ],
          },
        }],
      ],
    },
    # The main daemon static_library with all the code used to check for updates
    # with Omaha and expose a DBus daemon.
    {
      'target_name': 'libupdate_engine',
      'type': 'static_library',
      'dependencies': [
        'libpayload_consumer',
        'update_metadata-protos',
        'update_engine-dbus-adaptor',
      ],
      'variables': {
        'exported_deps': [
          'dbus-1',
          'expat',
          'libcurl',
          'libdebugd-client',
          'libmetrics-<(libbase_ver)',
          'libpower_manager-client',
          'libsession_manager-client',
          'libshill-client',
          'libssl',
          'libupdate_engine-client',
          'vboot_host',
        ],
        'conditions':[
          ['USE_dlc == 1', {
            'exported_deps' : [
              'libdlcservice-client',
            ],
          }],
        ],
        'deps': ['<@(exported_deps)'],
      },
      'all_dependent_settings': {
        'variables': {
          'deps': [
            '<@(exported_deps)',
          ],
        },
      },
      'link_settings': {
        'variables': {
          'deps': [
            '<@(exported_deps)',
          ],
        },
        'libraries': [
          '-lbz2',
          '-lpolicy-<(libbase_ver)',
          '-lrootdev',
          '-lrt',
        ],
      },
      'sources': [
        'boot_control_chromeos.cc',
        'certificate_checker.cc',
        'common_service.cc',
        'connection_manager.cc',
        'connection_utils.cc',
        'daemon.cc',
        'dbus_connection.cc',
        'dbus_service.cc',
        'hardware_chromeos.cc',
        'image_properties_chromeos.cc',
        'libcurl_http_fetcher.cc',
        'metrics_reporter_omaha.cc',
        'metrics_utils.cc',
        'omaha_request_action.cc',
        'omaha_request_params.cc',
        'omaha_response_handler_action.cc',
        'omaha_utils.cc',
        'p2p_manager.cc',
        'payload_state.cc',
        'power_manager_chromeos.cc',
        'real_system_state.cc',
        'shill_proxy.cc',
        'update_attempter.cc',
        'update_boot_flags_action.cc',
        'update_manager/boxed_value.cc',
        'update_manager/chromeos_policy.cc',
        'update_manager/default_policy.cc',
        'update_manager/enough_slots_ab_updates_policy_impl.cc',
        'update_manager/enterprise_device_policy_impl.cc',
        'update_manager/evaluation_context.cc',
        'update_manager/interactive_update_policy_impl.cc',
        'update_manager/next_update_check_policy_impl.cc',
        'update_manager/official_build_check_policy_impl.cc',
        'update_manager/out_of_box_experience_policy_impl.cc',
        'update_manager/policy.cc',
        'update_manager/policy_test_utils.cc',
        'update_manager/real_config_provider.cc',
        'update_manager/real_device_policy_provider.cc',
        'update_manager/real_random_provider.cc',
        'update_manager/real_shill_provider.cc',
        'update_manager/real_system_provider.cc',
        'update_manager/real_time_provider.cc',
        'update_manager/real_updater_provider.cc',
        'update_manager/staging_utils.cc',
        'update_manager/state_factory.cc',
        'update_manager/update_manager.cc',
        'update_manager/update_time_restrictions_policy_impl.cc',
        'update_manager/weekly_time.cc',
        'update_status_utils.cc',
      ],
      'conditions': [
        ['USE_chrome_network_proxy == 1', {
          'sources': [
            'chrome_browser_proxy_resolver.cc',
          ],
        }],
        ['USE_chrome_kiosk_app == 1', {
          'dependencies': [
            'update_engine-dbus-kiosk-app-client',
          ],
        }],
        ['USE_dlc == 1', {
          'sources': [
            'dlcservice_chromeos.cc',
          ],
        }],
        ['USE_dlc == 0', {
          'sources': [
            'common/dlcservice_stub.cc',
          ],
        }],
      ],
    },
    # update_engine daemon.
    {
      'target_name': 'update_engine',
      'type': 'executable',
      'dependencies': [
        'libupdate_engine',
      ],
      'sources': [
        'main.cc',
      ],
    },
    # update_engine client library.
    {
      'target_name': 'libupdate_engine_client',
      'type': 'static_library',
      'variables': {
        'deps': [
          'dbus-1',
          'libupdate_engine-client',
        ],
      },
      'sources': [
        'client_library/client.cc',
        'client_library/client_dbus.cc',
        'update_status_utils.cc',
      ],
      'include_dirs': [
        'client_library/include',
      ],
    },
    # update_engine console client.
    {
      'target_name': 'update_engine_client',
      'type': 'executable',
      'dependencies': [
        'libupdate_engine_client',
      ],
      'sources': [
        'common/error_code_utils.cc',
        'omaha_utils.cc',
        'update_engine_client.cc',
      ],
    },
    # server-side code. This is used for delta_generator and unittests but not
    # for any client code.
    {
      'target_name': 'libpayload_generator',
      'type': 'static_library',
      'dependencies': [
        'libpayload_consumer',
        'update_metadata-protos',
      ],
      'variables': {
        'exported_deps': [
          'ext2fs',
          'libbsdiff',
          'libpuffdiff',
          'liblzma',
        ],
        'deps': ['<@(exported_deps)'],
      },
      'all_dependent_settings': {
        'variables': {
          'deps': [
            '<@(exported_deps)',
          ],
        },
      },
      'link_settings': {
        'variables': {
          'deps': [
            '<@(exported_deps)',
          ],
        },
      },
      'sources': [
        'common/file_fetcher.cc',
        'payload_generator/ab_generator.cc',
        'payload_generator/annotated_operation.cc',
        'payload_generator/blob_file_writer.cc',
        'payload_generator/block_mapping.cc',
        'payload_generator/boot_img_filesystem.cc',
        'payload_generator/bzip.cc',
        'payload_generator/cycle_breaker.cc',
        'payload_generator/deflate_utils.cc',
        'payload_generator/delta_diff_generator.cc',
        'payload_generator/delta_diff_utils.cc',
        'payload_generator/ext2_filesystem.cc',
        'payload_generator/extent_ranges.cc',
        'payload_generator/extent_utils.cc',
        'payload_generator/full_update_generator.cc',
        'payload_generator/graph_types.cc',
        'payload_generator/graph_utils.cc',
        'payload_generator/inplace_generator.cc',
        'payload_generator/mapfile_filesystem.cc',
        'payload_generator/payload_file.cc',
        'payload_generator/payload_generation_config_chromeos.cc',
        'payload_generator/payload_generation_config.cc',
        'payload_generator/payload_signer.cc',
        'payload_generator/raw_filesystem.cc',
        'payload_generator/squashfs_filesystem.cc',
        'payload_generator/tarjan.cc',
        'payload_generator/topological_sort.cc',
        'payload_generator/xz_chromeos.cc',
      ],
    },
    # server-side delta generator.
    {
      'target_name': 'delta_generator',
      'type': 'executable',
      'dependencies': [
        'libpayload_consumer',
        'libpayload_generator',
      ],
      'link_settings': {
        'ldflags!': [
          '-pie',
        ],
      },
      'sources': [
        'payload_generator/generate_delta_main.cc',
      ],
    },
    {
      'target_name': 'update_engine_test_libs',
      'type': 'static_library',
      'variables': {
        'deps': [
          'libshill-client-test',
        ],
      },
      'dependencies': [
        'libupdate_engine',
      ],
      'includes': [
        '../../../platform2/common-mk/common_test.gypi',
      ],
      'sources': [
        'common/fake_prefs.cc',
        'common/mock_http_fetcher.cc',
        'common/test_utils.cc',
        'fake_shill_proxy.cc',
        'fake_system_state.cc',
        'payload_consumer/fake_file_descriptor.cc',
        'payload_generator/fake_filesystem.cc',
        'update_manager/umtest_utils.cc',
      ],
    },
  ],
  'conditions': [
    ['USE_test == 1', {
      'targets': [
        # Public keys used for unit testing.
        {
          'target_name': 'update_engine-testkeys',
          'type': 'none',
          'variables': {
            'openssl_pem_in_dir': '.',
            'openssl_pem_out_dir': 'include/update_engine',
          },
          'sources': [
            'unittest_key.pem',
            'unittest_key2.pem',
            'unittest_key_RSA4096.pem',
          ],
          'includes': ['../../../platform2/common-mk/openssl_pem.gypi'],
        },
        # Unpacks sample images used for testing.
        {
          'target_name': 'update_engine-test_images',
          'type': 'none',
          'variables': {
            'image_out_dir': '.',
          },
          'sources': [
            'sample_images/sample_images.tar.bz2',
          ],
          'includes': ['tar_bunzip2.gypi'],
        },
        # Test HTTP Server.
        {
          'target_name': 'test_http_server',
          'type': 'executable',
          'sources': [
            'common/http_common.cc',
            'test_http_server.cc',
          ],
        },
        # Test subprocess helper.
        {
          'target_name': 'test_subprocess',
          'type': 'executable',
          'sources': [
            'test_subprocess.cc',
          ],
        },
        # Main unittest file.
        {
          'target_name': 'update_engine_unittests',
          'type': 'executable',
          'variables': {
            'deps': [
              'libbrillo-test-<(libbase_ver)',
              'libchrome-test-<(libbase_ver)',
              'libdebugd-client-test',
              'libpower_manager-client-test',
              'libsession_manager-client-test',
              'libshill-client-test',
            ],
          },
          'dependencies': [
            'libupdate_engine',
            'libpayload_generator',
            'update_engine_test_libs',
          ],
          'sources': [
            'boot_control_chromeos_unittest.cc',
            'certificate_checker_unittest.cc',
            'common/action_pipe_unittest.cc',
            'common/action_processor_unittest.cc',
            'common/action_unittest.cc',
            'common/cpu_limiter_unittest.cc',
            'common/hash_calculator_unittest.cc',
            'common/http_fetcher_unittest.cc',
            'common/hwid_override_unittest.cc',
            'common/prefs_unittest.cc',
            'common/proxy_resolver_unittest.cc',
            'common/subprocess_unittest.cc',
            'common/terminator_unittest.cc',
            'common/utils_unittest.cc',
            'common_service_unittest.cc',
            'connection_manager_unittest.cc',
            'hardware_chromeos_unittest.cc',
            'image_properties_chromeos_unittest.cc',
            'metrics_reporter_omaha_unittest.cc',
            'metrics_utils_unittest.cc',
            'omaha_request_action_unittest.cc',
            'omaha_request_params_unittest.cc',
            'omaha_response_handler_action_unittest.cc',
            'omaha_utils_unittest.cc',
            'p2p_manager_unittest.cc',
            'payload_consumer/bzip_extent_writer_unittest.cc',
            'payload_consumer/cached_file_descriptor_unittest.cc',
            'payload_consumer/delta_performer_integration_test.cc',
            'payload_consumer/delta_performer_unittest.cc',
            'payload_consumer/download_action_unittest.cc',
            'payload_consumer/extent_reader_unittest.cc',
            'payload_consumer/extent_writer_unittest.cc',
            'payload_consumer/file_descriptor_utils_unittest.cc',
            'payload_consumer/file_writer_unittest.cc',
            'payload_consumer/filesystem_verifier_action_unittest.cc',
            'payload_consumer/postinstall_runner_action_unittest.cc',
            'payload_consumer/xz_extent_writer_unittest.cc',
            'payload_generator/ab_generator_unittest.cc',
            'payload_generator/blob_file_writer_unittest.cc',
            'payload_generator/block_mapping_unittest.cc',
            'payload_generator/boot_img_filesystem_unittest.cc',
            'payload_generator/cycle_breaker_unittest.cc',
            'payload_generator/deflate_utils_unittest.cc',
            'payload_generator/delta_diff_utils_unittest.cc',
            'payload_generator/ext2_filesystem_unittest.cc',
            'payload_generator/extent_ranges_unittest.cc',
            'payload_generator/extent_utils_unittest.cc',
            'payload_generator/full_update_generator_unittest.cc',
            'payload_generator/graph_utils_unittest.cc',
            'payload_generator/inplace_generator_unittest.cc',
            'payload_generator/mapfile_filesystem_unittest.cc',
            'payload_generator/payload_file_unittest.cc',
            'payload_generator/payload_generation_config_unittest.cc',
            'payload_generator/payload_signer_unittest.cc',
            'payload_generator/squashfs_filesystem_unittest.cc',
            'payload_generator/tarjan_unittest.cc',
            'payload_generator/topological_sort_unittest.cc',
            'payload_generator/zip_unittest.cc',
            'payload_state_unittest.cc',
            'testrunner.cc',
            'update_attempter_unittest.cc',
            'update_boot_flags_action_unittest.cc',
            'update_manager/boxed_value_unittest.cc',
            'update_manager/chromeos_policy_unittest.cc',
            'update_manager/evaluation_context_unittest.cc',
            'update_manager/generic_variables_unittest.cc',
            'update_manager/prng_unittest.cc',
            'update_manager/real_device_policy_provider_unittest.cc',
            'update_manager/real_random_provider_unittest.cc',
            'update_manager/real_shill_provider_unittest.cc',
            'update_manager/real_system_provider_unittest.cc',
            'update_manager/real_time_provider_unittest.cc',
            'update_manager/real_updater_provider_unittest.cc',
            'update_manager/staging_utils_unittest.cc',
            'update_manager/update_manager_unittest.cc',
            'update_manager/update_time_restrictions_policy_impl_unittest.cc',
            'update_manager/variable_unittest.cc',
            'update_manager/weekly_time_unittest.cc',
          ],
        },
      ],
    }],
    # Fuzzer target.
    ['USE_fuzzer == 1', {
      'targets': [
        {
          'target_name': 'update_engine_omaha_request_action_fuzzer',
          'type': 'executable',
          'variables': {
            'deps': [
              'libbrillo-test-<(libbase_ver)',
              'libchrome-test-<(libbase_ver)',
            ],
          },
          'includes': [
            '../../../platform2/common-mk/common_fuzzer.gypi',
          ],
          'dependencies': [
            'libupdate_engine',
            'update_engine_test_libs',
          ],
          'sources': [
            'omaha_request_action_fuzzer.cc',
          ],
        },
      ],
    }],
  ],
}
