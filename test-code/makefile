CRYPTO_LIBS = -lm -lcrypto

ASANFLAGS  = -fsanitize=address
ASANFLAGS += -fno-common -fno-omit-frame-pointer

CFLAGS += -I.

TEST_BUILD_DIR = $(TEST_DIR)/build

LIB_UTIL = $(BUILD_DIR)/libfileutils.a
LIB_CORE = $(BUILD_DIR)/libcore.a

UNITY_SRC = $(ROOT_DIR)/test-framework/unity.c
USEFULL_FUNC_SRC = $(TEST_DIR)/useful_functions.c

SRCS = $(UNITY_SRC) $(USEFULL_FUNC_SRC)


.PHONY: all test clean test_Sbox_gen test_key_expansion test_aes_functions test_cipher test_inv_cipher test_file_utils

all: test_Sbox_gen test_key_expansion test_aes_functions test_cipher test_inv_cipher test_file_utils
test: all


test_Sbox_gen: $(TEST_BUILD_DIR)/test_Sbox_gen
	$(TEST_BUILD_DIR)/test_Sbox_gen

$(TEST_BUILD_DIR)/test_Sbox_gen: test_Sbox_gen.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE)


test_key_expansion: $(TEST_BUILD_DIR)/test_key_expansion
	$(TEST_BUILD_DIR)/test_key_expansion

$(TEST_BUILD_DIR)/test_key_expansion: test_key_expansion.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE) $(CRYPTO_LIBS)


test_aes_functions: $(TEST_BUILD_DIR)/test_aes_functions
	$(TEST_BUILD_DIR)/test_aes_functions

$(TEST_BUILD_DIR)/test_aes_functions: test_aes_functions.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE)


test_cipher: $(TEST_BUILD_DIR)/test_cipher
	$(TEST_BUILD_DIR)/test_cipher

$(TEST_BUILD_DIR)/test_cipher: test_cipher.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE)


test_inv_cipher: $(TEST_BUILD_DIR)/test_inv_cipher
	$(TEST_BUILD_DIR)/test_inv_cipher

$(TEST_BUILD_DIR)/test_inv_cipher: test_inv_cipher.c $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_CORE)


test_file_utils: $(TEST_BUILD_DIR)/test_file_utils
	$(TEST_BUILD_DIR)/test_file_utils

$(TEST_BUILD_DIR)/test_file_utils: test_file_utils.c $(LIB_UTIL) $(LIB_CORE)
	$(CC) $(CFLAGS) $(ASANFLAGS) $(SRCS) -o $@ $< $(LIB_UTIL) $(LIB_CORE)


clean:
	rm -f $(TEST_BUILD_DIR)/*
