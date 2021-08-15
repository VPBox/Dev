#####
# Local unit test Makefile
#
# This makefile builds and runs the keymaster unit tests locally on the development
# machine, not on an Android device.  Android.mk builds the same tests into the
# "keymaster_tests" binary for execution on-device, but this Makefile runs them locally,
# for a very fast edit/build/test development cycle.
#
# To build and run these tests, one pre-requisite must be manually installed: BoringSSL.
# This Makefile expects to find BoringSSL in a directory adjacent to $ANDROID_BUILD_TOP.
# To get and build it, first install the Ninja build tool (e.g. apt-get install
# ninja-build), then do:
#
# cd $ANDROID_BUILD_TOP/..
# git clone https://boringssl.googlesource.com/boringssl
# cd boringssl
# mdkir build
# cd build
# cmake -GNinja ..
# ninja
#
# Then return to $ANDROID_BUILD_TOP/system/keymaster and run "make".
#####

BASE=../..
SUBS=system/core \
	hardware/libhardware \
	external/gtest \
	system/security/softkeymaster \
	system/security/keystore
GTEST=$(BASE)/external/googletest/googletest

INCLUDES=$(foreach dir,$(SUBS),-I $(BASE)/$(dir)/include) \
	-I $(BASE)/libnativehelper/include/nativehelper \
	-I $(GTEST)/include -isystem $(GTEST) -Iinclude -I$(BASE)/../boringssl/include

ifdef FORCE_32_BIT
ARCH_FLAGS = -m32
endif

ifdef USE_GCC
CXXFLAGS +=-std=c++14 -fprofile-arcs -ftest-coverage
CFLAGS += -fprofile-arcs -ftest-coverage
else
CC=$(BASE)/prebuilts/clang/host/linux-x86/clang-r339409b/bin/clang
CXX=$(BASE)/prebuilts/clang/host/linux-x86/clang-r339409b/bin/clang++
CXXFLAGS +=-std=c++14 -DKEYMASTER_CLANG_TEST_BUILD
CFLAGS += -DKEYMASTER_CLANG_TEST_BUILD
endif

LDFLAGS += $(ARCH_FLAGS)
CPPFLAGS = $(INCLUDES) -g -O0 -MD -MP $(ARCH_FLAGS) -DKEYMASTER_UNIT_TEST_BUILD -DHOST_BUILD
CXXFLAGS += -Wall -Werror -Wno-unused -Winit-self -Wpointer-arith -Wunused-parameter \
	-Werror=sign-compare -Werror=return-type -fno-permissive \
	-Wno-deprecated-declarations -fno-exceptions -DKEYMASTER_NAME_TAGS $(ARCH_FLAGS)
CFLAGS += $(ARCH_FLAGS) -DKEYMASTER_UNIT_TEST_BUILD -DHOST_BUILD

# Uncomment to enable debug logging.
# CXXFLAGS += -DDEBUG

LDLIBS=-L$(BASE)/../boringssl/build/crypto -lcrypto -lpthread -lstdc++ -lgcov

CPPSRCS=\
	km_openssl/aes_key.cpp \
	km_openssl/aes_operation.cpp \
	km_openssl/triple_des_key.cpp \
	km_openssl/triple_des_operation.cpp \
	android_keymaster/android_keymaster.cpp \
	android_keymaster/android_keymaster_messages.cpp \
	tests/android_keymaster_messages_test.cpp \
	tests/android_keymaster_test.cpp \
	tests/android_keymaster_test_utils.cpp \
	android_keymaster/android_keymaster_utils.cpp \
	km_openssl/asymmetric_key.cpp \
	km_openssl/asymmetric_key_factory.cpp \
	km_openssl/attestation_record.cpp \
	km_openssl/block_cipher_operation.cpp \
	tests/attestation_record_test.cpp \
	key_blob_utils/auth_encrypted_key_blob.cpp \
	android_keymaster/authorization_set.cpp \
	tests/authorization_set_test.cpp \
	km_openssl/ec_key.cpp \
	km_openssl/ec_key_factory.cpp \
	legacy_support/ec_keymaster0_key.cpp \
	legacy_support/ec_keymaster1_key.cpp \
	legacy_support/ecdsa_keymaster1_operation.cpp \
	km_openssl/ecdsa_operation.cpp \
	km_openssl/ecies_kem.cpp \
	tests/ecies_kem_test.cpp \
	tests/gtest_main.cpp \
	km_openssl/ckdf.cpp \
	tests/hkdf_test.cpp \
	km_openssl/hkdf.cpp \
	tests/hkdf_test.cpp \
	km_openssl/hmac.cpp \
	km_openssl/hmac_key.cpp \
	km_openssl/hmac_operation.cpp \
	tests/hmac_test.cpp \
	key_blob_utils/integrity_assured_key_blob.cpp \
	km_openssl/iso18033kdf.cpp \
	km_openssl/kdf.cpp \
	tests/kdf1_test.cpp \
	tests/kdf2_test.cpp \
	tests/kdf_test.cpp \
	tests/key_blob_test.cpp \
	legacy_support/keymaster0_engine.cpp \
	legacy_support/keymaster1_engine.cpp \
	android_keymaster/keymaster_configuration.cpp \
	tests/keymaster_configuration_test.cpp \
	android_keymaster/keymaster_enforcement.cpp \
	km_openssl/soft_keymaster_enforcement.cpp \
	tests/keymaster_enforcement_test.cpp \
	android_keymaster/keymaster_tags.cpp \
	android_keymaster/logger.cpp \
	km_openssl/nist_curve_key_exchange.cpp \
	tests/nist_curve_key_exchange_test.cpp \
	key_blob_utils/ocb_utils.cpp \
	km_openssl/openssl_err.cpp \
	km_openssl/openssl_utils.cpp \
	android_keymaster/operation.cpp \
	android_keymaster/operation_table.cpp \
	km_openssl/rsa_key.cpp \
	km_openssl/rsa_key_factory.cpp \
	legacy_support/rsa_keymaster0_key.cpp \
	legacy_support/rsa_keymaster1_key.cpp \
	legacy_support/rsa_keymaster1_operation.cpp \
	km_openssl/rsa_operation.cpp \
	android_keymaster/serializable.cpp \
	contexts/soft_keymaster_context.cpp \
	contexts/soft_keymaster_device.cpp \
	contexts/pure_soft_keymaster_context.cpp \
	km_openssl/symmetric_key.cpp \
	km_openssl/software_random_source.cpp \
	contexts/soft_attestation_cert.cpp \
	km_openssl/attestation_utils.cpp \
	key_blob_utils/software_keyblobs.cpp \
	km_openssl/wrapped_key.cpp

CCSRCS=$(GTEST)/src/gtest-all.cc
CSRCS=key_blob_utils/ocb.c

OBJS=$(CPPSRCS:.cpp=.o) $(CCSRCS:.cc=.o) $(CSRCS:.c=.o)
DEPS=$(CPPSRCS:.cpp=.d) $(CCSRCS:.cc=.d) $(CSRCS:.c=.d)

BINARIES = \
	tests/android_keymaster_messages_test \
	tests/android_keymaster_test \
	tests/attestation_record_test \
	tests/authorization_set_test \
	tests/ecies_kem_test \
	tests/ckdf_test \
	tests/hkdf_test \
	tests/hmac_test \
	tests/kdf1_test \
	tests/kdf2_test \
	tests/kdf_test \
	tests/key_blob_test \
	tests/keymaster_configuration_test \
	tests/keymaster_enforcement_test \
	tests/nist_curve_key_exchange_test

.PHONY: coverage memcheck massif clean run

%.run: %
	./$<
	touch $@

run: $(BINARIES:=.run)

coverage: coverage.info
	genhtml coverage.info --output-directory coverage

coverage.info: run
	lcov --capture --directory=. --output-file coverage.info

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
	--error-exitcode=1 \
	--suppressions=valgrind.supp \
	--gen-suppressions=all

MASSIF_OPTS=--tool=massif \
	--stacks=yes

%.memcheck : %
	valgrind $(MEMCHECK_OPTS) ./$< && \
	touch $@

%.massif : %
	valgrind $(MASSIF_OPTS) --massif-out-file=$@ ./$<

memcheck: $(BINARIES:=.memcheck)

massif: $(BINARIES:=.massif)

GTEST_OBJS = $(GTEST)/src/gtest-all.o tests/gtest_main.o

tests/keymaster_configuration_test: tests/keymaster_configuration_test.o \
	android_keymaster/authorization_set.o \
	android_keymaster/serializable.o \
	android_keymaster/logger.o \
	android_keymaster/keymaster_configuration.o \
	$(GTEST_OBJS)

tests/hmac_test: tests/hmac_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	android_keymaster/authorization_set.o \
	km_openssl/hmac.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/ckdf_test: tests/ckdf_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	android_keymaster/authorization_set.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	android_keymaster/serializable.o \
	km_openssl/ckdf.o \
	km_openssl/openssl_err.o \
	$(GTEST_OBJS)

tests/hkdf_test: tests/hkdf_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	android_keymaster/authorization_set.o \
	km_openssl/hkdf.o \
	km_openssl/hmac.o \
	km_openssl/kdf.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/kdf_test: tests/kdf_test.o \
	android_keymaster/android_keymaster_utils.o \
	km_openssl/kdf.o \
	android_keymaster/logger.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/kdf1_test: tests/kdf1_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	android_keymaster/authorization_set.o \
	km_openssl/iso18033kdf.o \
	km_openssl/kdf.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/kdf2_test: tests/kdf2_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	android_keymaster/authorization_set.o \
	km_openssl/iso18033kdf.o \
	km_openssl/kdf.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/nist_curve_key_exchange_test: tests/nist_curve_key_exchange_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/authorization_set.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	km_openssl/nist_curve_key_exchange.o \
	km_openssl/openssl_err.o \
	km_openssl/openssl_utils.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/ecies_kem_test: tests/ecies_kem_test.o \
	android_keymaster/android_keymaster_utils.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/authorization_set.o \
	km_openssl/ecies_kem.o \
	km_openssl/hkdf.o \
	km_openssl/hmac.o \
	km_openssl/kdf.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	km_openssl/nist_curve_key_exchange.o \
	km_openssl/openssl_err.o \
	km_openssl/openssl_utils.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/authorization_set_test: tests/authorization_set_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/authorization_set.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/key_blob_test: tests/key_blob_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	key_blob_utils/auth_encrypted_key_blob.o \
	android_keymaster/authorization_set.o \
	key_blob_utils/integrity_assured_key_blob.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	key_blob_utils/ocb.o \
	key_blob_utils/ocb_utils.o \
	km_openssl/openssl_err.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/android_keymaster_messages_test: tests/android_keymaster_messages_test.o \
	android_keymaster/android_keymaster_messages.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	android_keymaster/authorization_set.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/android_keymaster_test: tests/android_keymaster_test.o \
	android_keymaster/android_keymaster.o \
	android_keymaster/android_keymaster_messages.o \
	android_keymaster/android_keymaster_utils.o \
	android_keymaster/authorization_set.o \
	android_keymaster/keymaster_enforcement.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	android_keymaster/operation.o \
	android_keymaster/operation_table.o \
	android_keymaster/serializable.o \
	contexts/pure_soft_keymaster_context.o \
	contexts/soft_attestation_cert.o \
	contexts/soft_keymaster_context.o \
	contexts/soft_keymaster_device.o \
	key_blob_utils/auth_encrypted_key_blob.o \
	key_blob_utils/integrity_assured_key_blob.o \
	key_blob_utils/ocb.o \
	key_blob_utils/ocb_utils.o \
	key_blob_utils/software_keyblobs.o \
	km_openssl/aes_key.o \
	km_openssl/aes_key.o \
	km_openssl/aes_operation.o \
	km_openssl/aes_operation.o \
	km_openssl/asymmetric_key.o \
	km_openssl/asymmetric_key_factory.o \
	km_openssl/attestation_record.o \
	km_openssl/attestation_utils.o \
	km_openssl/block_cipher_operation.o \
	km_openssl/ckdf.o \
	km_openssl/ec_key.o \
	km_openssl/ec_key_factory.o \
	km_openssl/ecdsa_operation.o \
	km_openssl/hmac_key.o \
	km_openssl/hmac_operation.o \
	km_openssl/openssl_err.o \
	km_openssl/openssl_utils.o \
	km_openssl/rsa_key.o \
	km_openssl/rsa_key_factory.o \
	km_openssl/rsa_operation.o \
	km_openssl/soft_keymaster_enforcement.o \
	km_openssl/software_random_source.o \
	km_openssl/symmetric_key.o \
	km_openssl/triple_des_key.o \
	km_openssl/triple_des_operation.o \
	km_openssl/wrapped_key.o \
	legacy_support/ec_keymaster0_key.o \
	legacy_support/ec_keymaster1_key.o \
	legacy_support/ecdsa_keymaster1_operation.o \
	legacy_support/keymaster0_engine.o \
	legacy_support/keymaster1_engine.o \
	legacy_support/rsa_keymaster0_key.o \
	legacy_support/rsa_keymaster1_key.o \
	legacy_support/rsa_keymaster1_operation.o \
	tests/android_keymaster_test_utils.o \
	$(BASE)/system/security/keystore/keyblob_utils.o \
	$(GTEST_OBJS)

tests/keymaster_enforcement_test: tests/keymaster_enforcement_test.o \
	android_keymaster/android_keymaster_messages.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	android_keymaster/authorization_set.o \
	android_keymaster/keymaster_enforcement.o \
	km_openssl/ckdf.o \
	km_openssl/openssl_err.o \
	km_openssl/soft_keymaster_enforcement.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/attestation_record_test: tests/attestation_record_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	km_openssl/attestation_record.o \
	android_keymaster/authorization_set.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	km_openssl/openssl_err.o \
	android_keymaster/serializable.o \
	$(GTEST_OBJS)

tests/wrapped_key_test: tests/wrapped_key_test.o \
	tests/android_keymaster_test_utils.o \
	android_keymaster/android_keymaster_utils.o \
	km_openssl/attestation_record.o \
	android_keymaster/authorization_set.o \
	android_keymaster/keymaster_tags.o \
	android_keymaster/logger.o \
	km_openssl/openssl_err.o \
	android_keymaster/serializable.o \
	km_openssl/wrapped_key.o \
	$(GTEST_OBJS)

$(GTEST)/src/gtest-all.o: CXXFLAGS:=$(subst -Wmissing-declarations,,$(CXXFLAGS))

clean:
	rm -f $(OBJS) $(DEPS) $(BINARIES) \
		$(BINARIES:=.run) $(BINARIES:=.memcheck) $(BINARIES:=.massif) \
		*gcov *gcno *gcda coverage.info
	rm -rf coverage

-include $(CPPSRCS:.cpp=.d)
-include $(CCSRCS:.cc=.d)
