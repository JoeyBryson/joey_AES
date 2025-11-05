# --- Compiler and Flags ---
CC = gcc
LIBS = -lm -lcrypto

CFLAGS  = -std=c99
CFLAGS += -I.
CFLAGS += -I..
CFLAGS += -g
CFLAGS += -Wall -Wextra -pedantic
CFLAGS += -DUNITY_SUPPORT_64 -DUNITY_OUTPUT_COLOR

ASANFLAGS  = -fsanitize=address
ASANFLAGS += -fno-common -fno-omit-frame-pointer

# --- Common Sources ---
UNITY_SRC = test-framework/unity.c
AES_SRC   = core/aes.c core/key_expansion.c core/Sbox_gen.c test-files/useful_functions.c

# --- Paths ---
TEST_DIR = test-files
BUILD_DIR = $(TEST_DIR)/build

# --- Default Target ---
.PHONY: all
all: test_Sbox_gen test_key_expansion test_aes_functions test_cipher test_inv_cipher

# ==============================================================
# --- Sbox_gen Tests ---
# ==============================================================
.PHONY: test_Sbox_gen
test_Sbox_gen: $(BUILD_DIR)/test_Sbox_gen.out
	@./$(BUILD_DIR)/test_Sbox_gen.out

$(BUILD_DIR)/test_Sbox_gen.out: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_Sbox_gen.c
	@echo "Compiling S-box tests..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_Sbox_gen.c -o $@ $(LIBS)

.PHONY: memcheck_Sbox_gen
memcheck_Sbox_gen:
	@echo "Running AddressSanitizer on S-box tests..."
	@$(CC) $(ASANFLAGS) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_Sbox_gen.c -o $(BUILD_DIR)/memcheck_Sbox_gen.out $(LIBS)
	@./$(BUILD_DIR)/memcheck_Sbox_gen.out
	@echo "Memory check passed ✅"

# ==============================================================
# --- Key Expansion Tests ---
# ==============================================================
.PHONY: test_key_expansion
test_key_expansion: $(BUILD_DIR)/test_key_expansion.out
	@./$(BUILD_DIR)/test_key_expansion.out

$(BUILD_DIR)/test_key_expansion.out: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_key_expansion.c
	@echo "Compiling AES key expansion tests..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_key_expansion.c -o $@ $(LIBS)

.PHONY: memcheck_key_expansion
memcheck_key_expansion:
	@echo "Running AddressSanitizer on AES key expansion tests..."
	@$(CC) $(ASANFLAGS) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_key_expansion.c -o $(BUILD_DIR)/memcheck_key_expansion.out $(LIBS)
	@./$(BUILD_DIR)/memcheck_key_expansion.out
	@echo "Memory check passed ✅"

# ==============================================================
# --- AES Core Function Tests ---
# ==============================================================
.PHONY: test_aes_functions
test_aes_functions: $(BUILD_DIR)/test_aes_functions.out
	@./$(BUILD_DIR)/test_aes_functions.out

$(BUILD_DIR)/test_aes_functions.out: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_aes_functions.c
	@echo "Compiling AES core function tests..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_aes_functions.c -o $@ $(LIBS)

.PHONY: memcheck_aes_functions
memcheck_aes_functions:
	@echo "Running AddressSanitizer on AES core function tests..."
	@$(CC) $(ASANFLAGS) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_aes_functions.c -o $(BUILD_DIR)/memcheck_aes_functions.out $(LIBS)
	@./$(BUILD_DIR)/memcheck_aes_functions.out
	@echo "Memory check passed ✅"

# ==============================================================
# --- AES Encrypt Tests ---
# ==============================================================
.PHONY: test_cipher
test_cipher: $(BUILD_DIR)/test_cipher.out
	@./$(BUILD_DIR)/test_cipher.out

$(BUILD_DIR)/test_cipher.out: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_cipher.c
	@echo "Compiling AES encryption tests..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_cipher.c -o $@ $(LIBS)

.PHONY: memcheck_cipher
memcheck_cipher:
	@echo "Running AddressSanitizer on AES encryption tests..."
	@$(CC) $(ASANFLAGS) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_cipher.c -o $(BUILD_DIR)/memcheck_cipher.out $(LIBS)
	@./$(BUILD_DIR)/memcheck_cipher.out
	@echo "Memory check passed ✅"

# ==============================================================
# --- AES Decrypt Tests ---
# ==============================================================
.PHONY: test_inv_cipher
test_inv_cipher: $(BUILD_DIR)/test_inv_cipher.out
	@./$(BUILD_DIR)/test_inv_cipher.out

$(BUILD_DIR)/test_inv_cipher.out: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_inv_cipher.c
	@echo "Compiling AES decryption tests..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_inv_cipher.c -o $@ $(LIBS)

.PHONY: memcheck_inv_cipher
memcheck_inv_cipher:
	@echo "Running AddressSanitizer on AES decryption tests..."
	@$(CC) $(ASANFLAGS) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_inv_cipher.c -o $(BUILD_DIR)/memcheck_inv_cipher.out $(LIBS)
	@./$(BUILD_DIR)/memcheck_inv_cipher.out
	@echo "Memory check passed ✅"

# ==============================================================
# --- Clean ---
# ==============================================================
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)/*.out
	@echo "Cleaned build artifacts from $(BUILD_DIR)."
