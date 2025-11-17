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
AES_SRC   = core/aes.c core/key_expansion.c core/Sbox_gen.c test/useful_functions.c file-utilities/file_encrypt.c file-utilities/file_decrypt.c file-utilities/utils.c

# --- Paths ---
TEST_DIR = test
BUILD_DIR = $(TEST_DIR)/build

# --- Default Target ---
.PHONY: all
all: test_Sbox_gen test_key_expansion test_aes_functions test_cipher test_inv_cipher test_file_utils

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
# --- AES file Tests ---
# ==============================================================
.PHONY: test_file_utils
test_file_utils: $(BUILD_DIR)/test_file_utils.out
	@./$(BUILD_DIR)/test_file_utils.out

$(BUILD_DIR)/test_file_utils.out: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_file_utils.c
	@echo "Compiling AES file util tests..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_file_utils.c -o $@ $(LIBS)

.PHONY: memcheck_file_utils
memcheck_file_utils:
	@echo "Running AddressSanitizer on AES decryption tests..."
	@$(CC) $(ASANFLAGS) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_file_utils.c -o $(BUILD_DIR)/test_file_utils.out $(LIBS)
	@./$(BUILD_DIR)/test_file_utils.out
	@echo "Memory check passed ✅"

# ==============================================================
# --- Clean ---
# ==============================================================
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)/*.out
	@echo "Cleaned build artifacts from $(BUILD_DIR)."

# ==============================================================
# --- Utility: print_joey (pretty-print a .joey file) ---
# ==============================================================
.PHONY: print_joey
print_joey: $(BUILD_DIR)/print_joey
	@# Usage: make print_joey FILE=path/to/file.joey
	@if [ -z "$(FILE)" ]; then \
		echo "Usage: make print_joey FILE=path/to/file.joey"; exit 1; \
	fi
	@if [ ! -f "$(FILE)" ]; then \
		echo "Error: file '$(FILE)' not found"; exit 1; \
	fi
	@./$(BUILD_DIR)/print_joey "$(FILE)"

$(BUILD_DIR)/print_joey: $(TEST_DIR)/print_joey.c
	@echo "Compiling print_joey utility..."
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(TEST_DIR)/print_joey.c -o $@ $(LIBS)

.PHONY: clean-print_joey
clean-print_joey:
	@rm -f $(BUILD_DIR)/print_joey
	@echo "Removed print_joey binary from $(BUILD_DIR)."
