# bootanalyze #

The bootanalyze tool helps to profile boot timing.

Per specific product modify config.yaml file to include
events you are looking for. Config should look like:

    stop_event: <logcat log message which will terminate log collection after reboot>
    events:
      event1_name: <pattern that matches log message>
      event2_.....

On some devise clock is showing incorrect time for first couple of seconds after boot.
To ensure correct adjustment of time, one has to include event in config that will
be present in dmesg log after clock correction.
