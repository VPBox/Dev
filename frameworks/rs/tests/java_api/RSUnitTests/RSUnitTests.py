# Copyright 2017, The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


"""Generates necessary files for unit test versions for testing.

After adding/changing RenderScript unit tests, run `python RSUnitTests.py`
to ensure that forward/backward compatibility tests run properly.

This file is so when updating/adding unit tests there is one central location
for managing the list of unit tests and their versions.

This is necessary since forward compatibility unit tests are chosen at compile
time where backward compatibility tests are chosen at runtime.

Generates a Java file for backward compatibility testing.
Generates an Android.mk file for forward compatibility testing.
"""


import shutil
import sys
import os


# List of platform API versions and the tests that pass on that version as
# well as all newer versions (e.g. tests under 23 also pass on 24, 25, etc.).
# The Slang version that correctly compiles the test is assumed to be the
# same build tools version unless otherwise specified in
# UNIT_TEST_TOOLS_VERSIONS below.
# The test name must correspond to a UT_{}.java file.
# e.g. alloc -> UT_alloc.java
UNIT_TEST_PLATFORM_VERSIONS = {
    19: [
        'alloc',
        'array_alloc',
        'array_init',
        'atomic',
        'bitfield',
        'bug_char',
        'check_dims',
        'clamp_relaxed',
        'clamp',
        'constant',
        'convert_relaxed',
        'convert',
        'copy_test',
        'element',
        'foreach',
        'foreach_bounds',
        'fp_mad',
        'instance',
        'int4',
        'kernel',
        'kernel_struct',
        'math',
        'min',
        'noroot',
        'primitives',
        'refcount',
        'reflection3264',
        'rsdebug',
        'rstime',
        'rstypes',
        'sampler',
        'static_globals',
        'struct',
        'unsigned',
        'vector',
    ],

    21: [
        'foreach_multi',
        'math_agree',
        'math_conformance',
    ],

    23: [
        'alloc_copy',
        'alloc_copyPadded',
        'ctxt_default',
        'kernel2d',
        'kernel2d_oldstyle',
        'kernel3d',
        'rsdebug_23',
        'script_group2_gatherscatter',
        'script_group2_nochain',
        'script_group2_pointwise',
    ],

    24: [
        'fp16',
        'fp16_globals',
        'math_24',
        'math_fp16',
        'reduce_backward',
        'reduce',
        'rsdebug_24',
        'script_group2_float',
        'single_source_alloc',
        'single_source_ref_count',
        'single_source_script',
        'small_struct',
        'small_struct_2',
    ],

    26: [
        'blur_validation',
        'struct_field',
        'struct_field_simple',
    ],
}


# List of tests and the build tools version they compile correctly on.
# The build tools version is the earliest build tools version that can
# compile it correctly, all versions newer than that version are also
# expected to compile correctly.
# Only to override the platform version in UNIT_TEST_PLATFORM_VERSIONS.
# Only affects forward compatibility tests.
# Useful for Slang regression fixes.
UNIT_TEST_TOOLS_VERSIONS = {
    'small_struct': 26,
    'reflection3264': 26,
}


# Tests that only belong to RSTest_Compat (support lib tests)
SUPPORT_LIB_ONLY_UNIT_TESTS = {
    'alloc_supportlib',
    'apitest',
}


# Tests that are skipped in RSTest_Compat (support lib tests)
SUPPORT_LIB_IGNORE_TESTS = {
    'fp16',
    'fp16_globals',
    'math_fp16',
}


# Dictionary mapping unit tests to the corresponding needed .rs files
# Only needed if UT_{}.java does not map to {}.rs
UNIT_TEST_RS_FILES_OVERRIDE = {
    'alloc_copy': [],
    'alloc_copyPadded': [],
    'blur_validation': [],
    'script_group2_float': ['float_test.rs'],
    'script_group2_gatherscatter': ['addup.rs'],
    'script_group2_nochain': ['increment.rs', 'increment2.rs', 'double.rs'],
    'script_group2_pointwise': ['increment.rs', 'double.rs'],
}


# List of API versions and the corresponding build tools release version
# For use with forward compatibility testing
BUILD_TOOL_VERSIONS = {
    21: '21.1.2',
    22: '22.0.1',
    23: '23.0.3',
    24: '24.0.3',
    25: '25.0.2',
}


# ---------- Makefile generation ----------


def WriteMakeCopyright(gen_file):
  """Writes the copyright for a Makefile to a file."""
  gen_file.write(
      '#\n'
      '# Copyright (C) 2017 The Android Open Source Project\n'
      '#\n'
      '# Licensed under the Apache License, Version 2.0 (the "License");\n'
      '# you may not use this file except in compliance with the License.\n'
      '# You may obtain a copy of the License at\n'
      '#\n'
      '#      http://www.apache.org/licenses/LICENSE-2.0\n'
      '#\n'
      '# Unless required by applicable law or agreed to in writing, software\n'
      '# distributed under the License is distributed on an "AS IS" BASIS,\n'
      '# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n'
      '# See the License for the specific language governing permissions and\n'
      '# limitations under the License.\n'
      '#\n\n'
  )


def WriteMakeSrcFiles(gen_file, api_version, src_dirs=['src'],
                      use_build_tools_version=False):
  """Writes applicable LOCAL_SRC_FILES to gen_file.

  Includes everything under ./src, base UnitTest class, and test files.

  api_version: only tests that can run on this version are added."""
  # Get all tests compatible with the build tool version
  # Compatible means build tool version >= test version
  tests = []
  for test_version, tests_for_version in (
      UNIT_TEST_PLATFORM_VERSIONS.iteritems()):
    if api_version >= test_version:
      tests.extend(tests_for_version)
  if use_build_tools_version:
    tests = [x for x in tests if (x not in UNIT_TEST_TOOLS_VERSIONS or
                                  test_version >= UNIT_TEST_TOOLS_VERSIONS[x])]
  tests = sorted(tests)
  gen_file.write(
      'LOCAL_SRC_FILES :=\\\n'
  )
  for src_dir in src_dirs:
    gen_file.write('    $(call all-java-files-under,{})\\\n'.format(src_dir))

  gen_file.write(
      '    $(my_rs_unit_tests_path)/UnitTest.java\\\n'.format(src_dir)
  )
  for test in tests:
    # Add the Java and corresponding rs files to LOCAL_SRC_FILES
    gen_file.write(
        '    $(my_rs_unit_tests_path)/{}\\\n'.format(JavaFileForUnitTest(test))
    )
    for rs_file in RSFilesForUnitTest(test):
      gen_file.write('    $(my_rs_unit_tests_path)/{}\\\n'.format(rs_file))


# ---------- Java file generation ----------


def WriteJavaCopyright(gen_file):
  """Writes the copyright for a Java file to gen_file."""
  gen_file.write(
      '/*\n'
      ' * Copyright (C) 2017 The Android Open Source Project\n'
      ' *\n'
      ' * Licensed under the Apache License, Version 2.0 (the "License");\n'
      ' * you may not use this file except in compliance with the License.\n'
      ' * You may obtain a copy of the License at\n'
      ' *\n'
      ' *      http://www.apache.org/licenses/LICENSE-2.0\n'
      ' *\n'
      ' * Unless required by applicable law or agreed to in writing, software\n'
      ' * distributed under the License is distributed on an "AS IS" BASIS,\n'
      ' * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n'
      ' * See the License for the specific language governing permissions and\n'
      ' * limitations under the License.\n'
      ' */\n\n'
  )


# ---------- Unit Test file functions ----------


def JavaFileForUnitTest(test):
  """Returns the Java file name for a unit test."""
  return 'UT_{}.java'.format(test)


def RSFilesForUnitTest(test):
  """Returns a list of .rs files associated with a test."""
  if test in UNIT_TEST_RS_FILES_OVERRIDE:
    return UNIT_TEST_RS_FILES_OVERRIDE[test]
  else:
    # Default is one .rs file with the same name as the input
    return ['{}.rs'.format(test)]


# ---------- Dirs ----------


def ThisScriptDir():
  """Returns the directory this script is in."""
  return os.path.dirname(os.path.realpath(__file__))


def UnitTestDir():
  """Returns the path to the directory containing the unit tests."""
  return os.path.join(ThisScriptDir(), 'src', 'com', 'android', 'rs',
                      'unittest')


def SupportLibOnlyTestDir():
  """Returns the path to the directory with unit tests for support lib."""
  return os.path.join(ThisScriptDir(), 'supportlibonlysrc', 'com',
                      'android', 'rs', 'unittest')


def SupportLibGenTestDir():
  """Returns the path to the directory with unit tests for support lib."""
  return os.path.join(ThisScriptDir(), 'supportlibsrc_gen', 'com',
                      'android', 'rs', 'unittest')


# ---------- RSTest_Compat/RSTest_Compat19 test generation ----------


def AllUnitTestsExceptSupportLibOnly():
  """Returns a set of all unit tests except SUPPORT_LIB_ONLY_UNIT_TESTS."""
  ret = set()
  for _, tests in UNIT_TEST_PLATFORM_VERSIONS.iteritems():
    ret.update(tests)
  return ret


def CopyUnitTestJavaFileToSupportLibTest(java_file_name, java_file_dir):
  """Copies the Java file to the support lib dir.

  Replaces RenderScript imports with corresponding support lib imports."""
  in_path = os.path.join(java_file_dir, java_file_name)
  out_path = os.path.join(SupportLibGenTestDir(), java_file_name)
  with open(in_path, 'r') as in_file, open(out_path, 'w') as out_file:
    out_file.write(
        '// This file is automatically generated from\n'
        '// frameworks/rs/tests/java_api/RSUnitTests/RSUnitTests.py\n'
    )
    for line in in_file.readlines():
      if line.startswith('import android.renderscript.'):
        line = line.replace('android.renderscript.',
                            'androidx.renderscript.')
      out_file.write(line)


def CopyUnitTestRSToSupportLibTest(rs_file_name, rs_file_dir):
  """Copies the .rs to the support lib dir."""
  in_path = os.path.join(rs_file_dir, rs_file_name)
  out_path = os.path.join(SupportLibGenTestDir(), rs_file_name)
  with open(out_path, 'w') as out_file:
    out_file.write(
        '// This file is automatically generated from\n'
        '// frameworks/rs/tests/java_api/RSUnitTests/RSUnitTests.py\n'
    )
    with open(in_path, 'r') as in_file:
      out_file.write(in_file.read())


def CopySharedRshToSupportLibTest():
  """Copies shared.rsh to the support lib dir.

  Adds a #define RSTEST_COMPAT at the end."""
  shared_rsh = 'shared.rsh'
  CopyUnitTestRSToSupportLibTest(shared_rsh, UnitTestDir())
  with open(os.path.join(SupportLibGenTestDir(), shared_rsh), 'a') as shared:
    shared.write('\n#define RSTEST_COMPAT\n')


def CopyUnitTestToSupportLibTest(test, test_dir):
  """Copies all files corresponding to a unit test to support lib dir."""
  CopyUnitTestJavaFileToSupportLibTest(JavaFileForUnitTest(test), test_dir)
  for rs in RSFilesForUnitTest(test):
    CopyUnitTestRSToSupportLibTest(rs, test_dir)


def GenerateSupportLibUnitTests():
  """Generates all support lib unit tests."""
  if os.path.exists(SupportLibGenTestDir()):
    shutil.rmtree(SupportLibGenTestDir())
  os.makedirs(SupportLibGenTestDir())

  CopySharedRshToSupportLibTest()
  CopyUnitTestJavaFileToSupportLibTest('UnitTest.java', UnitTestDir())

  for test in AllUnitTestsExceptSupportLibOnly() - SUPPORT_LIB_IGNORE_TESTS:
    CopyUnitTestToSupportLibTest(test, UnitTestDir())

  for test in SUPPORT_LIB_ONLY_UNIT_TESTS:
    CopyUnitTestToSupportLibTest(test, SupportLibOnlyTestDir())

  print ('Generated support lib tests at {}'
         .format(SupportLibGenTestDir()))


# ---------- RSTest_Compat19 ----------


def WriteSupportLib19Makefile(gen_file):
  """Writes the Makefile for support lib 19 testing."""
  WriteMakeCopyright(gen_file)
  gen_file.write(
      '# This file is auto-generated by frameworks/rs/tests/java_api/RSUnitTests/RSUnitTests.py.\n'
      '# To change unit tests version, please run the Python script above.\n\n'
      'LOCAL_PATH := $(call my-dir)\n'
      'include $(CLEAR_VARS)\n\n'
      'LOCAL_PACKAGE_NAME := RSTest_Compat19\n'
      'LOCAL_MODULE_TAGS := tests\n\n'
      'LOCAL_STATIC_JAVA_LIBRARIES := \\\n'
      '    android-support-test \\\n'
      '    android-support-v8-renderscript \\\n\n'
      'LOCAL_RENDERSCRIPT_TARGET_API := 19\n'
      'LOCAL_RENDERSCRIPT_COMPATIBILITY := true\n'
      'LOCAL_RENDERSCRIPT_FLAGS := -rs-package-name=androidx.renderscript\n'
      'LOCAL_SDK_VERSION := current\n'
      'LOCAL_MIN_SDK_VERSION := 8\n\n'
      'my_rs_unit_tests_path := ../RSUnitTests/supportlibsrc_gen/com/android/rs/unittest\n'
  )
  WriteMakeSrcFiles(gen_file, 19)
  gen_file.write(
      '\n'
      'include $(BUILD_PACKAGE)\n\n'
      'my_rs_unit_tests_path :=\n\n'
  )


def SupportLib19MakefileLocation():
  """Returns the location of Makefile for backward compatibility 19 testing."""
  return os.path.join(ThisScriptDir(), '..', 'RSTest_CompatLib19', 'Android.mk')


def GenerateSupportLib19():
  """Generates the necessary file for Support Library tests (19)."""
  with open(SupportLib19MakefileLocation(), 'w') as gen_file:
    WriteSupportLib19Makefile(gen_file)
  print ('Generated support lib (19) Makefile at {}'
         .format(SupportLib19MakefileLocation()))


# ---------- RSTestForward ----------


def ForwardTargetName(build_tool_version_name):
  """Returns the target name for a forward compatibility build tool name."""
  make_target_name = 'RSTestForward_{}'.format(build_tool_version_name)
  make_target_name = make_target_name.replace('.', '_')
  return make_target_name


def ForwardDirLocation(build_tool_version_name):
  """Returns location of directory for forward compatibility testing."""
  return os.path.join(ThisScriptDir(), '..', 'RSTestForward',
                      build_tool_version_name)


def ForwardJavaSrcLocation(build_tool_version_name):
  """Returns location of src directory for forward compatibility testing."""
  return os.path.join(ForwardDirLocation(build_tool_version_name), 'src')


def ForwardMakefileLocation(build_tool_version_name):
  """Returns the location of the Makefile for forward compatibility testing."""
  return os.path.join(ForwardDirLocation(build_tool_version_name),
                      'Android.mk')


def ForwardAndroidManifestLocation(build_tool_version_name):
  """Returns AndroidManifest.xml location for forward compatibility testing."""
  return os.path.join(ForwardDirLocation(build_tool_version_name),
                      'AndroidManifest.xml')


def ForwardJavaApiVersionLocation(build_tool_version_name):
  """Returns Java version file location for forward compatibility testing."""
  return os.path.join(ForwardJavaSrcLocation(build_tool_version_name),
                      'RSForwardVersion.java')


def WriteForwardAndroidManifest(gen_file, package):
  """Writes forward compatibility AndroidManifest.xml to gen_file."""
  gen_file.write(
      '<?xml version="1.0" encoding="utf-8"?>\n'
      '<!-- Copyright (C) 2017 The Android Open Source Project\n'
      '\n'
      '     Licensed under the Apache License, Version 2.0 (the "License");\n'
      '     you may not use this file except in compliance with the License.\n'
      '     You may obtain a copy of the License at\n'
      '\n'
      '          http://www.apache.org/licenses/LICENSE-2.0\n'
      '\n'
      '     Unless required by applicable law or agreed to in writing, software\n'
      '     distributed under the License is distributed on an "AS IS" BASIS,\n'
      '     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n'
      '     See the License for the specific language governing permissions and\n'
      '     limitations under the License.\n'
      '\n'
      '     This file is automatically generated by\n'
      '     frameworks/rs/tests/java_api/RSUnitTests/RSUnitTests.py.\n'
      '-->\n'
      '<manifest xmlns:android="http://schemas.android.com/apk/res/android"\n'
      '    package="{}">\n'
      '    <uses-sdk\n'
      '        android:minSdkVersion="21"\n'
      '        android:targetSdkVersion="26" />\n'
      '\n'
      '    <application\n'
      '        android:label="RSTestForward">\n'
      '        <uses-library android:name="android.test.runner" />\n'
      '    </application>\n'
      '\n'
      '    <instrumentation\n'
      '        android:name="androidx.test.runner.AndroidJUnitRunner"\n'
      '        android:targetPackage="{}"\n'
      '        android:label="RenderScript Forward Compatibility Tests" />\n'
      '</manifest>\n'.format(package, package)
  )


def WriteForwardToolsVersion(gen_file, version):
  """Writes forward compatibility Java class with tools version as String."""
  WriteJavaCopyright(gen_file)
  gen_file.write(
      'package com.android.rs.testforward;\n\n'
      'public class RSForwardVersion {{\n'
      '    public static final String VERSION = "{}";\n'
      '}}\n'.format(version)
  )


def WriteForwardMakefile(gen_file, build_tool_version, build_tool_version_name):
  """Writes the Makefile for forward compatibility testing.

  Makefile contains a build target per build tool version
  for forward compatibility testing based on the unit test list at the
  top of this file."""
  WriteMakeCopyright(gen_file)
  gen_file.write(
      '# This file is auto-generated by frameworks/rs/tests/java_api/RSUnitTests/RSUnitTests.py.\n'
      '# To change unit tests version, please run the Python script above.\n\n'
      'ifneq ($(ENABLE_RSTESTS),)\n\n'
      'LOCAL_PATH := $(call my-dir)\n'
      'my_rs_unit_tests_path := ../../RSUnitTests/src/com/android/rs/unittest\n'
  )
  make_target_name = ForwardTargetName(build_tool_version_name)
  gen_file.write(
      '\n'
      '# RSTestForward for build tool version {}\n\n'
      'include $(CLEAR_VARS)\n\n'
      'LOCAL_MODULE_TAGS := tests\n'
      'LOCAL_STATIC_JAVA_LIBRARIES := android-support-test\n'
      'LOCAL_COMPATIBILITY_SUITE := device-tests\n'
      'LOCAL_RENDERSCRIPT_TARGET_API := current\n'
      'LOCAL_PACKAGE_NAME := {}\n'
      'LOCAL_SDK_VERSION := current\n'
      'my_rs_path := $(TOP)/prebuilts/renderscript/host/linux-x86/{}\n'
      'LOCAL_RENDERSCRIPT_CC := $(my_rs_path)/bin/llvm-rs-cc\n'
      'LOCAL_RENDERSCRIPT_INCLUDES_OVERRIDE := $(my_rs_path)/include $(my_rs_path)/clang-include\n'
      'my_rs_path :=\n'.format(
          build_tool_version_name, make_target_name, build_tool_version_name
      )
  )
  WriteMakeSrcFiles(gen_file, build_tool_version, ['../src', 'src'], True)
  gen_file.write(
      '\n'
      'include $(BUILD_PACKAGE)\n\n'
      'my_rs_unit_tests_path :=\n\n'
      'endif\n'
  )


def ForwardMakeTargetsLocation():
  """Returns the location of the file with all forward compatibility targets."""
  return os.path.join(ThisScriptDir(), '..', 'RSTestForward', 'Targets.mk')


def WriteForwardMakeTargets(gen_file):
  """Writes forward compatibility make target names to gen_file."""
  gen_file.write('RSTESTFORWARD_TARGETS := \\\n')
  for build_tool_version_name in sorted(BUILD_TOOL_VERSIONS.values()):
    make_target_name = ForwardTargetName(build_tool_version_name)
    gen_file.write('    {} \\\n'.format(make_target_name))


def GenerateForward():
  """Generates the necessary file for forward compatibility testing."""
  for build_tool_version in sorted(BUILD_TOOL_VERSIONS.keys()):
    build_tool_version_name = BUILD_TOOL_VERSIONS[build_tool_version]
    if not os.path.exists(ForwardDirLocation(build_tool_version_name)):
      os.mkdir(ForwardDirLocation(build_tool_version_name))
      os.mkdir(ForwardJavaSrcLocation(build_tool_version_name))
    with open(ForwardMakefileLocation(build_tool_version_name), 'w') as gen_file:
      WriteForwardMakefile(gen_file, build_tool_version, build_tool_version_name)
    print ('Generated forward compatibility Makefile at {}'
           .format(ForwardMakefileLocation(build_tool_version_name)))
    with open(ForwardAndroidManifestLocation(build_tool_version_name), 'w') as gen_file:
      package = 'com.android.rs.testforward{}'.format(build_tool_version)
      WriteForwardAndroidManifest(gen_file, package)
    print ('Generated forward compatibility AndroidManifest.xml at {}'
           .format(ForwardAndroidManifestLocation(build_tool_version_name)))
    with open(ForwardJavaApiVersionLocation(build_tool_version_name), 'w') as gen_file:
      WriteForwardToolsVersion(gen_file, build_tool_version)
    print ('Generated forward compatibility RSForwardVersion.java at {}'
           .format(ForwardJavaApiVersionLocation(build_tool_version_name)))
  with open(ForwardMakeTargetsLocation(), 'w') as gen_file:
    WriteForwardMakeTargets(gen_file)
  print ('Generated forward compatibility targets at {}'
         .format(ForwardMakeTargetsLocation()))


# ---------- RSTestBackward ----------


def BackwardJavaFileLocation():
  """Returns the location of Java file for backward compatibility testing."""
  return os.path.join(ThisScriptDir(), '..', 'RSTestBackward', 'src', 'com',
                      'android', 'rs', 'testbackward', 'RSTests.java')


def Backward19JavaFileLocation():
  """Returns the location of Java file for backward compatibility 19 testing."""
  return os.path.join(ThisScriptDir(), '..', 'RSTestBackward19', 'src', 'com',
                      'android', 'rs', 'testbackward19', 'RSTests.java')


def Backward19MakefileLocation():
  """Returns the location of Makefile for backward compatibility 19 testing."""
  return os.path.join(ThisScriptDir(), '..', 'RSTestBackward19', 'Android.mk')


def WriteBackwardJavaFile(gen_file, package, max_api_version=None):
  """Writes the Java file for backward compatibility testing to gen_file.

  Java file determines unit tests for backward compatibility
  testing based on the unit test list at the top of this file."""
  WriteJavaCopyright(gen_file)
  gen_file.write(
      'package {};\n'
      '\n'
      'import com.android.rs.unittest.*;\n'
      '\n'
      'import java.util.ArrayList;\n'
      '\n'
      '/**\n'
      ' * This class is auto-generated by frameworks/rs/tests/java_api/RSUnitTests/RSUnitTests.py.\n'
      ' * To change unit tests version, please run the Python script above.\n'
      ' */\n'
      'public class RSTests {{\n'
      '    public static Iterable<Class<? extends UnitTest>> getTestClassesForCurrentAPIVersion() {{\n'
      '        int thisApiVersion = android.os.Build.VERSION.SDK_INT;\n'
      '\n'
      '        ArrayList<Class<? extends UnitTest>> validClasses = new ArrayList<>();'.format(
          package
      )
  )
  for version in sorted(UNIT_TEST_PLATFORM_VERSIONS.keys()):
    if max_api_version is None or version <= max_api_version:
      tests = sorted(UNIT_TEST_PLATFORM_VERSIONS[version])
      gen_file.write(
          '\n\n        if (thisApiVersion >= {}) {{\n'.format(version)
      )
      for test in tests:
        gen_file.write(
            '            validClasses.add(UT_{}.class);\n'.format(test)
        )
      gen_file.write('        }')
  gen_file.write('\n\n        return validClasses;\n    }\n}\n')


def WriteBackward19Makefile(gen_file):
  """Writes the Makefile for backward compatibility 19 testing."""
  WriteMakeCopyright(gen_file)
  gen_file.write(
      '# This file is auto-generated by frameworks/rs/tests/java_api/RSUnitTests/RSUnitTests.py.\n'
      '# To change unit tests version, please run the Python script above.\n\n'
      'LOCAL_PATH := $(call my-dir)\n'
      'include $(CLEAR_VARS)\n\n'
      'LOCAL_MODULE_TAGS := tests\n'
      'LOCAL_STATIC_JAVA_LIBRARIES := android-support-test\n'
      'LOCAL_COMPATIBILITY_SUITE := device-tests\n'
      'LOCAL_RENDERSCRIPT_TARGET_API := 19\n'
      'LOCAL_MIN_SDK_VERSION := 17\n'
      'LOCAL_SDK_VERSION := current\n'
      'LOCAL_PACKAGE_NAME := RSTestBackward19\n'
      'my_rs_unit_tests_path := ../RSUnitTests/src/com/android/rs/unittest\n'
  )
  WriteMakeSrcFiles(gen_file, 19)
  gen_file.write(
      '\n'
      'include $(BUILD_PACKAGE)\n\n'
      'my_rs_unit_tests_path :=\n\n'
  )


def GenerateBackward():
  """Generates Java file for backward compatibility testing."""
  with open(BackwardJavaFileLocation(), 'w') as gen_file:
    WriteBackwardJavaFile(gen_file, 'com.android.rs.testbackward')
  print ('Generated backward compatibility Java file at {}'
         .format(BackwardJavaFileLocation()))


def GenerateBackward19():
  """Generates files for backward compatibility testing for API 19."""
  with open(Backward19JavaFileLocation(), 'w') as gen_file:
    WriteBackwardJavaFile(gen_file, 'com.android.rs.testbackward19', 19)
  print ('Generated backward compatibility (19) Java file at {}'
         .format(Backward19JavaFileLocation()))

  with open(Backward19MakefileLocation(), 'w') as gen_file:
    WriteBackward19Makefile(gen_file)
  print ('Generated backward compatibility (19) Makefile at {}'
         .format(Backward19MakefileLocation()))


# ---------- Main ----------


def DisplayHelp():
  """Prints help message."""
  print >> sys.stderr, ('Usage: {} [forward] [backward] [backward19] [help|-h|--help]\n'
                        .format(sys.argv[0]))
  print >> sys.stderr, ('[forward]: write forward compatibility Makefile to\n    {}\n'
                        .format(ForwardMakefileLocation()))
  print >> sys.stderr, ('[backward]: write backward compatibility Java file to\n    {}\n'
                        .format(BackwardJavaFileLocation()))
  print >> sys.stderr, ('[backward19]: write backward compatibility Java file (19) to\n    {}\n'
                        .format(Backward19JavaFileLocation()))
  print >> sys.stderr, ('[backward19]: write backward compatibility Makefile (19) to\n    {}\n'
                        .format(Backward19MakefileLocation()))
  print >> sys.stderr, ('[supportlib]: generate support lib unit tests to\n    {}\n'
                        .format(SupportLibGenTestDir()))
  print >> sys.stderr, ('[supportlib19]: generate support lib Makefile (19) to\n    {}\n'
                        .format(SupportLib19MakefileLocation()))
  print >> sys.stderr, 'if no options are chosen, then all files are generated'


def main():
  """Parses sys.argv and does stuff."""
  display_help = False
  error = False
  actions = []

  for arg in sys.argv[1:]:
    if arg in ('help', '-h', '--help'):
      display_help = True
    elif arg == 'backward':
      actions.append(GenerateBackward)
    elif arg == 'backward19':
      actions.append(GenerateBackward19)
    elif arg == 'forward':
      actions.append(GenerateForward)
    elif arg == 'supportlib':
      actions.append(GenerateSupportLibUnitTests)
    elif arg == 'supportlib19':
      actions.append(GenerateSupportLib19)
    else:
      print >> sys.stderr, 'unrecognized arg: {}'.format(arg)
      error = True

  if display_help or error:
    DisplayHelp()
  elif actions:
    for action in actions:
      action()
  else:
    # No args - do default action.
    GenerateBackward()
    GenerateBackward19()
    GenerateForward()
    GenerateSupportLibUnitTests()
    GenerateSupportLib19()


if __name__ == '__main__':
  main()
