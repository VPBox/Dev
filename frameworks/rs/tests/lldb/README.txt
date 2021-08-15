- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
-   LLDB for Renderscript Test Suite
-
-   16/03/2016
- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

Overview:

    The LLDB for Renderscript test suite is written in python and relies on
    LLDB's python bindings.  The test suite will push several test app's onto
    a target device, and makes coordinated use of LLDB and ADB to run automated
    debug sessions.

Set up the test suite:

    Check out the AOSP and build it for your target. Navigate to
    /frameworks/rs/test/lldb and type mm.  This should successfully build the
    binaries that the testsuite uses. They will be placed in
    <path to out folder>/target/product/<product code name>/data/app, system/lib
    and system/bin.

Prerequisite:

    An lldb-server executable must be present on your device/emulator.
    LLDB must be compiled on your host machine along with its python interface.
    lldb-server and lldb should be built from the same source revisions.

Running the test suite:

    The test suite can be run via the following command:

        > python run_tests.py [-h]
                              [--config path]
                              [--device DEVICE]
                              [--test path]
                              [--install-only]
                              [--no-install]
                              [--no-uninstall]
                              [--print-to-stdout]
                              [--verbose]
                              [--wimpy]
                              [--run-emu]
                              [--adb-path ADB_PATH]
                              [--aosp-product-path AOSP_PRODUCT_PATH]
                              [--blacklist BLACKLIST [BLACKLIST ...]]
                              [--device-port DEVICE_PORT]
                              [--emu-cmd EMU_CMD]
                              [--host-port HOST_PORT]
                              [--lldb-path LLDB_PATH]
                              [--lldb-server-path-device LLDB_SERVER_PATH_DEVICE]
                              [--lldb-server-path-host LLDB_SERVER_PATH_HOST]
                              [--log-file-path LOG_FILE_PATH]
                              [--results-file-path RESULTS_FILE_PATH]
                              [--timeout TIMEOUT]

        optional arguments:
          -h, --help            show this help message and exit
          --config path, -c path
                                Path to a custom config file.
          --device DEVICE, -d DEVICE
                                Specify the device id of the device to test on.
          --test path, -t path  Specify a specific test to run.
          --install-only        It only runs the pre-run stage of the test suite. It
                                installs the required APKs but does not execute the
                                tests.
          --no-install, -n      Stop the test suite installing apks to device.
          --no-uninstall        Stop the test suite uninstalling apks after
                                completion.
          --print-to-stdout     Print all logging information to standard out.
          --verbose, -v         Store extra info in the log.
          --wimpy, -w           Test only a core subset of features.
          --run-emu             Spawn an emulator and run the test suite on that.
                                Specify the emulator command line in the config file
                                or with -emu-cmd.
          --adb-path ADB_PATH   Path to android debug bridge on the host.
          --aosp-product-path AOSP_PRODUCT_PATH
                                The path to the "out" folder of the AOSP repository.
          --blacklist BLACKLIST [BLACKLIST ...]
                                Provide a test blacklist for skipping specific tests.
                                To specify the blacklist from the command line the
                                following can be used: --blacklist test1.py test2.py
                                ...
          --device-port DEVICE_PORT
                                Specify the port number that lldb-server (on the
                                device) listens on. When lldb-server is spawned on the
                                device it will listen on this port. Each successive
                                test will increment onwards from this port.
          --emu-cmd EMU_CMD     The command line for the emulator (if using -run-emu).
          --host-port HOST_PORT
                                Specify host port which lldb-server will be forwarded
                                to. Specify the starting host port number that lldb-
                                server (on the target) will be forwarded to on the
                                host. Each successive test will increment onwards from
                                this initial port.
          --lldb-path LLDB_PATH
                                The path to lldb executable on the host.
          --lldb-server-path-device LLDB_SERVER_PATH_DEVICE
                                Path to the lldb-server executable on the device.
          --lldb-server-path-host LLDB_SERVER_PATH_HOST
                                Path to the lldb-server executable on host (if using
                                -run-emu).
          --log-file-path LOG_FILE_PATH
                                The path to the file where the log will be written.
          --results-file-path RESULTS_FILE_PATH
                                The path to the file where junit results.xml will be
                                written.
          --timeout TIMEOUT     Timeout period for a single command, expressed in
                                seconds

    An optional config file can be passed to the test suite which will provide
    details of your specific environment. The user file should define a custom
    class inheriting from Config in config.py. The class Config presents the
    default set of options, available to be overridden.

    All options in the config file can also be specified on the command line.

    If your config and command line do not specify a path to the host lldb,
    the PYTHONPATH environment variable must be set.  The appropriate value to
    set this to can be obtained by running the following command:

        > lldb -P

    This will print out a path to the lldb python bindings on your local machine.

Build Requirements:


    The following revisions are from the llvm git mirror:

    llvm : 5786b73
    clang: b6d0b32
    lldb : 98712eb

    lldb has the following dependencies:

      Python2.7.6
      swig2.0
      lldb-server

Building LLDB python bindings:

    Linux:

        Build instructions for Linux lldb can be found on the official lldb web
        page:

          http://lldb.llvm.org/build.html

        The following CMake variables should be enabled when generating:

          LLDB_ENABLE_PYTHON_SCRIPTS_SWIG_API_GENERATION = True

        As a post build step, swig will generate the python bindings for lldb.

    Windows:

        Prerequisites:

            Visual Studio 2015
            Custom x64 Python2.7.10 package:
                http://p-nand-q.com/python/2015.08.07-Python2710-x64-vs2015.7z
            Swig Version 3.0.5 (Added to the $PATH)
            Cmake 3.4.0 (So we can generate Visual Studio 2015 solutions)

        Build Python for Windows:

            http://p-nand-q.com/python/building-python-27-with-vs2010.html

            The important thing here is that the above python distribution
            contains debug versions of the libraries and is built with the same
            Visual Studio version we are using so the runtimes do not conflict.

        Build LLDB on Windows:

            Select the Cmake generator "Microsoft Visual Studio 2015 Win64".
            The following CMake variables should be enabled when generating:

                LLDB_DISABLE_PYTHON=False
                LLDB_RELOCATABLE_PYTHON=False
                PYTHON_HOME=<path to the above python release>

            Using cmake-gui is a good idea and lets you make sure that swig has
            been correctly detected.

            In the CMake configure step, you should see something similar to the
            following:
            -- Found Python version 2.7.9
            -- LLDB Found PythonExecutable: E:/Python27/python.exe and
            E:/Python27/python_d.exe
            -- LLDB Found PythonLibs: E:/Python27/libs/python27.lib and
            E:/Python27/libs/python27_d.lib
            -- LLDB Found PythonDLL: E:/Python27/python27.dll and
            E:/Python27/python27_d.dll
            -- LLDB Found PythonIncludeDirs: E:/Python27/Include
            LLDB version: 3.8.0
            Could NOT find Doxygen (missing:  DOXYGEN_EXECUTABLE)
            Found PythonInterp:
            $<$<CONFIG:Debug>:E:/Python27/python_d.exe>$<$<NOT:$<CONFIG:Debug>>:E:/Python27/python.exe>
            (found version "1.4")

            Build LLDB as a RelWithDebInfo build, because debug builds of lldb
            produce an lldb_d.pyd file, but the __init__.py still refers to
            lldb.pyd.

            The LLDB python bindings should have built in
            "llvm_build\RelWithDebInfo\lib\site-packages\lldb". Point the test
            suite towards "llvm_build\RelWithDebInfo\bin\lldb.exe".
            When running the test suite itself, make sure to use the python
            executable from the custom package.

A typical test transcript:

    Located ADB
    Located device ZX1G427S2S
    Pushing all tests...
    Pushed all tests
    Pre run complete
    Found 86 tests
    Running test_allocation_dump_2_cpp.py: PASS
    Running test_breakpoint_fileline_multiple_rs_files_cpp.py: PASS
    Running test_read_local_cpp.py: PASS
    Running test_breakpoint_kernel_multiple_rs_files_cpp.py: PASS
    Running test_multiple_rs_files_cpp.py: PASS
    Running test_breakpoint_kernel_all_multiple_rs_files_cpp.py: PASS
    Running test_dwarf_lang_cpp.py: PASS
    Running test_write_global_element_cpp.py: PASS
    Running test_allocation_list_cpp.py: PASS
    Running test_breakpoint_coordinate_cpp.py: PASS
    Running test_rs_consts_cpp.py: PASS
    Running test_allocation_file_cpp.py: PASS
    Running test_allocation_dump_1_cpp.py: PASS
    Running test_source_step_cpp.py: PASS
    Running test_breakpoint_kernel_2_cpp.py: PASS
    Running test_backtrace_cpp.py: PASS
    Running test_language_subcmds_no_debug_cpp.py: PASS
    Running test_breakpoint_kernel_1_cpp.py: PASS
    Running test_language_subcmds_cpp.py: PASS
    Running test_write_global_cpp.py: PASS
    Running test_invoke_fun_cpp.py: PASS
    Running test_breakpoint_fileline_cpp.py: PASS
    Running test_write_local_cpp.py: PASS
    Running test_breakpoint_kernel_all_cpp.py: PASS
    Running test_write_local_element_cpp.py: PASS
    Running test_call_api_funs_cpp.py: PASS
    Running test_coordinates_cpp.py: PASS
    Running test_read_global_cpp.py: PASS
    Running test_language_subcmds.py: PASS
    Running test_coordinates.py: PASS
    Running test_language_subcmds_no_debug.py: PASS
    Running test_read_local.py: PASS
    Running test_call_api_funs.py: PASS
    Running test_breakpoint_kernel_1.py: PASS
    Running test_breakpoint_fileline.py: PASS
    Running test_breakpoint_fileline_multiple_rs_files.py: PASS
    Running test_rs_consts.py: PASS
    Running test_invoke_fun.py: PASS
    Running test_write_local_element.py: PASS
    Running test_source_step.py: PASS
    Running test_allocation_file.py: PASS
    Running test_allocation_list.py: PASS
    Running test_breakpoint_kernel_multiple_rs_files.py: PASS
    Running test_allocation_dump_1.py: PASS
    Running test_breakpoint_kernel_all.py: PASS
    Running test_allocation_dump_2.py: PASS
    Running test_allocation_dump_struct.py: PASS
    Running test_read_global.py: PASS
    Running test_language.py: PASS
    Running test_dwarf_lang.py: PASS
    Running test_breakpoint_coordinate.py: PASS
    Running test_write_global.py: PASS
    Running test_multiple_rs_files.py: PASS
    Running test_write_global_element.py: PASS
    Running test_breakpoint_kernel_all_multiple_rs_files.py: PASS
    Running test_breakpoint_kernel_2.py: PASS
    Running test_write_local.py: PASS
    Running test_backtrace.py: PASS
    Running test_call_api_funs_jni.py: PASS
    Running test_invoke_fun_jni.py: PASS
    Running test_allocation_dump_1_jni.py: PASS
    Running test_breakpoint_fileline_multiple_rs_files_jni.py: PASS
    Running test_allocation_file_jni.py: PASS
    Running test_breakpoint_fileline_jni.py: PASS
    Running test_source_step_jni.py: PASS
    Running test_coordinates_jni.py: PASS
    Running test_rs_consts_jni.py: PASS
    Running test_breakpoint_kernel_all_multiple_rs_files_jni.py: PASS
    Running test_multiple_rs_files_jni.py: PASS
    Running test_allocation_dump_2_jni.py: PASS
    Running test_allocation_list_jni.py: PASS
    Running test_write_local_element_jni.py: PASS
    Running test_breakpoint_kernel_all_jni.py: PASS
    Running test_breakpoint_coordinate_jni.py: PASS
    Running test_language_subcmds_no_debug_jni.py: PASS
    Running test_read_local_jni.py: PASS
    Running test_dwarf_lang_jni.py: PASS
    Running test_breakpoint_kernel_2_jni.py: PASS
    Running test_breakpoint_kernel_multiple_rs_files_jni.py: PASS
    Running test_write_global_element_jni.py: PASS
    Running test_breakpoint_kernel_1_jni.py: PASS
    Running test_read_global_jni.py: PASS
    Running test_language_subcmds_jni.py: PASS
    Running test_write_global_jni.py: PASS
    Running test_backtrace_jni.py: PASS
    Running test_write_local_jni.py: PASS
    Uninstalled/Deleted all tests
    86 of 86 passed
    100% rate
