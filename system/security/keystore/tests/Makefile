##########
# This makefile builds local unit tests that run locally on the development machine.  Note
# that it may be necessary to install some libraries on the dev maching to make the tests
# build.
#
# The same unit tests are also built by Android.mk to run on the target device.  The tests
# should always build and pass in both places.  The on-device test is what really matters,
# of course, but debugging is often somewhat easier on the dev platform.
##########

BASE=../../../..
SUBS=system/core \
	system/keymaster\
	hardware/libhardware \
	external/gtest
GTEST=$(BASE)/external/gtest
KEYMASTER=$(BASE)/system/keymaster

INCLUDES=$(foreach dir,$(SUBS),-I $(BASE)/$(dir)/include) \
	-I $(BASE)/libnativehelper/include/nativehelper \
	-I $(GTEST) -Iinclude

# Add USE_CLANG=1 to the make command line to build with clang, which has better error
# reporting and diagnoses some conditions that GCC doesn't.
ifdef USE_CLANG
CC=/usr/bin/clang
CXX=/usr/bin/clang
CLANG_TEST_DEFINE=-DKEYMASTER_CLANG_TEST_BUILD
COMPILER_SPECIFIC_ARGS=-std=c++11 $(CLANG_TEST_DEFINE)
else
COMPILER_SPECIFIC_ARGS=-std=c++0x -fprofile-arcs
endif

CPPFLAGS=$(INCLUDES) -g -O0 -MD -DHOST_BUILD
CXXFLAGS=-Wall -Werror -Wno-unused -Winit-self -Wpointer-arith	-Wunused-parameter \
        -Werror=sign-compare -Wmissing-declarations -ftest-coverage -fno-permissive \
	-Wno-deprecated-declarations -fno-exceptions -DKEYMASTER_NAME_TAGS \
	$(COMPILER_SPECIFIC_ARGS)

# Uncomment to enable debug logging.
# CXXFLAGS += -DDEBUG

LDLIBS=-lpthread -lstdc++ -lgcov

# This list of sources is used for dependency generation and cleanup.  Add each new source
# file here (not headers).
CPPSRCS=\
	../auth_token_table.cpp \
	auth_token_table_test.cpp \
	gtest_main.cpp \
	$(KEYMASTER)/authorization_set.cpp \
	$(KEYMASTER)/keymaster_tags.cpp \
	$(KEYMASTER)/logger.cpp \
	$(KEYMASTER)/serializable.cpp

CCSRCS=$(GTEST)/src/gtest-all.cc

# This list of binaries determes what gets built and run.  Add each new test binary here.
BINARIES=\
	auth_token_table_test

.PHONY: coverage memcheck massif clean run

%.run: %
	./$<
	touch $@

run: $(BINARIES:=.run)

GTEST_OBJS = $(GTEST)/src/gtest-all.o gtest_main.o

auth_token_table_test: auth_token_table_test.o \
	../auth_token_table.o \
	$(GTEST_OBJS) \
	$(KEYMASTER)/authorization_set.o \
	$(KEYMASTER)/keymaster_tags.o \
	$(KEYMASTER)/logger.o \
	$(KEYMASTER)/serializable.o

coverage: coverage.info
	genhtml coverage.info --output-directory coverage

coverage.info: run
	lcov --capture --directory=. --directory=.. -b . --output-file coverage.info

%.coverage : %
	$(MAKE) clean && $(MAKE) $<
	./$<
	lcov --capture --directory=. --output-file coverage.info
	genhtml coverage.info --output-directory coverage
#UNINIT_OPTS=--track-origins=yes
UNINIT_OPTS=--undef-value-errors=no

MEMCHECK_OPTS=--leak-check=full \
	--show-reachable=yes \
	--vgdb=full \
	$(UNINIT_OPTS) \
	--error-exitcode=1

MASSIF_OPTS=--tool=massif \
	--stacks=yes

%.memcheck : %
	valgrind $(MEMCHECK_OPTS) ./$< && \
	touch $@

%.massif : %
	valgrind $(MASSIF_OPTS) --massif-out-file=$@ ./$<

memcheck: $(BINARIES:=.memcheck)

massif: $(BINARIES:=.massif)

OBJS=$(CPPSRCS:.cpp=.o)
DEPS=$(CPPSRCS:.cpp=.d)
GCOV=$(CPPSRCS:.cpp=.gcov) $(CPPSRCS:.cpp=.gcda) $(CPPSRCS:.cpp=.gcno)

clean:
	rm -f $(OBJS) $(DEPS) $(BINARIES) $(GCOV) \
		$(BINARIES:=.run) $(BINARIES:=.memcheck) $(BINARIES:=.massif) \
		*gcov *gcno *gcda coverage.info
	rm -rf coverage

-include $(CPPSRCS:.cpp=.d)
-include $(CCSRCS:.cc=.d)

