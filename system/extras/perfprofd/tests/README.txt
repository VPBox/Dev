Native tests for 'perfprofd'. Please run with

   runtest --path=system/extras/perfprofd/tests

(where runtest == $ANDROID_BUILD_TOP"/development/testrunner/runtest.py).

Notes:

1. Several of the testpoints in this unit tests perform a live 'simpleperf'
run on the device (if you are using a userdebug build, simpleperf should
already be available in /system/xbin/simpleperf).

2. Part of the test is a system-wide profile, meaning that you will
need to run 'adb root' prior to test execution.

3. The daemon under test, perfprofd, is broken into a main function, a
"core" library, and a "utils library. Picture:

	+-----------+   perfprofdmain.o
	| perfprofd |
	| main()    |   1-liner; calls perfprofd_main()
	+-----------+
	   |
	   v
	+-----------+   perfprofdcore.a
	| perfprofd |
	| core      |   most of the interesting code is here;
	|           |   calls into utils library when for
	+-----------+   operations such as sleep, log, etc
	   |
	   v
	+-----------+   perfprofdutils.a
	| perfprofd |
	| utils     |   real implementations of perfprofd_sleep,
	|           |   perfprofd_log_* etc
	+-----------+

Because the daemon tends to spend a lot of time sleeping/waiting,
it is impractical to try to test it directly. Instead we insert a
mock utilities layer and then have a test driver that invokes the
daemon main function. Picture for perfprofd_test:

	+----------------+   perfprofd_test.cc
	| perfprofd_test |
	|                |   makes calls into perfprofd_main(),
	+----------------+   then verifies behavior
	   |
	   v
	+-----------+   perfprofdcore.a
	| perfprofd |
	| core      |   same as above
	+-----------+
	   |
	   v
	+-----------+   perfprofdmockutils.a
	| perfprofd |
	| mockutils |   mock implementations of perfprofd_sleep,
	|           |   perfprofd_log_* etc
	+-----------+

The mockup versions of perfprofd_sleep() and  perfprofd_log_* do
simply log the fact that they are called; the test driver can
then examine the log to make sure that the daemon is doing
what it is supposed to be doing.
