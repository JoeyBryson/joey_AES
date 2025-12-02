CRYPTO_LIBS = -lm -lcrypto

SAN_FLAGS = -fsanitize=address,undefined,leak -fno-omit-frame-pointer -g
CFLAGS += -DUNITY_SUPPORT_64 -DUNITY_OUTPUT_COLOR

CFLAGS += -I.

TEST_BUILD_DIR = $(TEST_DIR)/build

LIB_UTIL = $(BUILD_DIR)/libfileutils.a
LIB_CORE = $(BUILD_DIR)/libcore.a

UNITY_SRC = $(ROOT_DIR)/test-framework/unity.c
USEFUL_FUNC_SRC = $(TEST_DIR)/useful_functions.c

SRCS = $(UNITY_SRC) $(USEFUL_FUNC_SRC)

# EXE_EXT is set by root makefile for Windows builds
EXE_EXT ?=

# For Windows cross-compilation, use Wine to run executables
ifeq ($(EXE_EXT),.exe)
	RUN_PREFIX = wine
else
	RUN_PREFIX =
endif


.PHONY: all test clean test_Sbox_gen test_key_expansion test_aes_functions test_cipher test_inv_cipher test_file_utils

all: test_Sbox_gen test_key_expansion test_aes_functions test_cipher test_inv_cipher test_file_utils
test: all


test_Sbox_gen: $(TEST_BUILD_DIR)/test_Sbox_gen$(EXE_EXT)
	$(RUN_PREFIX) $(TEST_BUILD_DIR)/test_Sbox_gen$(EXE_EXT)

$(TEST_BUILD_DIR)/test_Sbox_gen$(EXE_EXT): test_Sbox_gen.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE)


test_key_expansion: $(TEST_BUILD_DIR)/test_key_expansion$(EXE_EXT)
	$(RUN_PREFIX) $(TEST_BUILD_DIR)/test_key_expansion$(EXE_EXT)

$(TEST_BUILD_DIR)/test_key_expansion$(EXE_EXT): test_key_expansion.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE)


test_aes_functions: $(TEST_BUILD_DIR)/test_aes_functions$(EXE_EXT)
	$(RUN_PREFIX) $(TEST_BUILD_DIR)/test_aes_functions$(EXE_EXT)

$(TEST_BUILD_DIR)/test_aes_functions$(EXE_EXT): test_aes_functions.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE)


test_cipher: $(TEST_BUILD_DIR)/test_cipher$(EXE_EXT)
	$(RUN_PREFIX) $(TEST_BUILD_DIR)/test_cipher$(EXE_EXT)

$(TEST_BUILD_DIR)/test_cipher$(EXE_EXT): test_cipher.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE)


test_inv_cipher: $(TEST_BUILD_DIR)/test_inv_cipher$(EXE_EXT)
	$(RUN_PREFIX) $(TEST_BUILD_DIR)/test_inv_cipher$(EXE_EXT)

$(TEST_BUILD_DIR)/test_inv_cipher$(EXE_EXT): test_inv_cipher.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE)


test_file_utils: $(TEST_BUILD_DIR)/test_file_utils$(EXE_EXT)
	$(RUN_PREFIX) $(TEST_BUILD_DIR)/test_file_utils$(EXE_EXT)

$(TEST_BUILD_DIR)/test_file_utils$(EXE_EXT): test_file_utils.c $(LIB_UTIL) $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_UTIL) $(LIB_CORE) $(LDFLAGS)

test_key_gen: $(TEST_BUILD_DIR)/test_key_gen$(EXE_EXT)
	$(RUN_PREFIX) $(TEST_BUILD_DIR)/test_key_gen$(EXE_EXT)

$(TEST_BUILD_DIR)/test_key_gen$(EXE_EXT): test_key_gen.c $(LIB_UTIL) $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_UTIL) $(LIB_CORE) $(LDFLAGS)

clean:
	rm -f $(TEST_BUILD_DIR)/*
	rm -f test-files/out-files/test_text1.txt
	rm -f test-files/out-files/test_text2.txt
	rm -f test-files/out-files/test_text3.txt
	rm -f test-files/out-files/wales.jpg
	rm -f test-files/cipher-files/test_3_cipher.jwy
	rm -f test-files/cipher-files/big1.jwy
	rm -f test-files/cipher-files-copied/test_3_cipher.jwy