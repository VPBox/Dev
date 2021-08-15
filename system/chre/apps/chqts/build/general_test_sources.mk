# Since we add source files to this app somewhat regularly, we prefer
# a central definition shared across all target platforms.

GENERAL_TEST_SRC_FILES = \
    app.cc \
    basic_audio_test.cc \
    basic_gnss_test.cc \
    basic_sensor_test_base.cc \
    basic_sensor_tests.cc \
    basic_wifi_test.cc \
    cell_info_base.cc \
    cell_info_cdma.cc \
    cell_info_gsm.cc \
    cell_info_lte.cc \
    cell_info_tdscdma.cc \
    cell_info_wcdma.cc \
    estimated_host_time_test.cc \
    event_between_apps_test.cc \
    get_time_test.cc \
    gnss_capabilities_test.cc \
    heap_alloc_stress_test.cc \
    heap_exhaustion_stability_test.cc \
    hello_world_test.cc \
    host_awake_suspend_test.cc \
    logging_sanity_test.cc \
    nanoapp_info.cc \
    nanoapp_info_by_app_id_test.cc \
    nanoapp_info_by_instance_id_test.cc \
    nanoapp_info_events_test_observer.cc \
    nanoapp_info_events_test_performer.cc \
    running_info.cc \
    send_event_test.cc \
    send_event_stress_test.cc \
    send_message_to_host_test.cc \
    sensor_info_test.cc \
    simple_heap_alloc_test.cc \
    test.cc \
    timer_cancel_test.cc \
    timer_set_test.cc \
    timer_stress_test.cc \
    version_sanity_test.cc \
    wifi_capabilities_test.cc \
    wwan_capabilities_test.cc \
    wwan_cell_info_test.cc
