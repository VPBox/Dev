#!/system/bin/sh

# Side test for b/124769206.
if [ -z "$BOOTCLASSPATH" ] ; then
  log -t apex_tests -p f "BOOTCLASSPATH is perceived as empty"
  exit 1
fi

# Look for the session data, but do not fail and instead print a literal
# otherwise. Sleep is an attempt to ensure that the message always reaches
# logcat.
/system/bin/logwrapper \
  /system/bin/sh -c \
    'ls /apex/com.android.apex.test_package/etc/sample_prebuilt_file \
     || echo "PreInstall Test" ; sleep 5s'
