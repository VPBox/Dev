# Install

~~~
$ m -j hidl-doc
~~~

# Usage

View usage info:

~~~
$ ./bin/hidl-doc -h
~~~

Parse the audio `types.hal` file in the Android repo and output
generated HTML reference to the reference directory. Enable verbose mode:

~~~
$ ./bin/hidl-doc -v -i /path/to/android/hardware/interfaces/audio/2.0/types.hal \
  -o /path/to/output/en/reference/hidl/
~~~

Parse all HAL files in the Android `/hardware/interfaces/` directory
and output generated HTML reference docs to reference directory. Skip files
that encounter doc parse errors:

~~~
$ ./bin/hidl-doc -v -s -i /path/to/android/hardware/interfaces/ \
  -o /path/to/output/en/reference/hidl/
~~~
# Templates

HTML templates are used to generate the output docs and are in the
`resources/template/` directory. Since these files are bundled up in the fat jar
file, if you make any changes to the templates, `hidl-doc.jar` must be rebuilt.
