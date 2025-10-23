# --- Compiler and Flags ---
CC = gcc
LIBS = -lm -lcrypto

CFLAGS  = -std=c99
CFLAGS += -I.
CFLAGS += -I..
CFLAGS += -g
CFLAGS += -Wall -Wextra -pedantic -Werror
CFLAGS += -DUNITY_SUPPORT_64 -DUNITY_OUTPUT_COLOR

ASANFLAGS  = -fsanitize=address
ASANFLAGS += -fno-common -fno-omit-frame-pointer

# --- Common Sources ---
UNITY_SRC = test-framework/unity.c
AES_SRC   = aes.c key_expansion.c Sbox_gen.c

# --- Paths ---
TEST_DIR = test-files

# --- Default Target ---
.PHONY: all
all: test_Sbox_gen test_key_expansion

# ==============================================================
# --- Sbox_gen Tests ---
# ==============================================================
.PHONY: test_Sbox_gen
test_Sbox_gen: $(TEST_DIR)/test_Sbox_gen.out
	@./$(TEST_DIR)/test_Sbox_gen.out

$(TEST_DIR)/test_Sbox_gen.out: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_Sbox_gen.c
	@echo "Compiling S-box tests..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_Sbox_gen.c -o $@ $(LIBS)

# --- Memory Check ---
.PHONY: memcheck_Sbox_gen
memcheck_Sbox_gen: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_Sbox_gen.c
	@echo "Running AddressSanitizer on S-box tests..."
	@$(CC) $(ASANFLAGS) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_Sbox_gen.c -o $(TEST_DIR)/memcheck_Sbox_gen.out $(LIBS)
	@./$(TEST_DIR)/memcheck_Sbox_gen.out
	@echo "Memory check passed ✅"

# ==============================================================
# --- Key Expansion Tests ---
# ==============================================================
.PHONY: test_key_expansion
test_key_expansion: $(TEST_DIR)/test_key_expansion.out
	@./$(TEST_DIR)/test_key_expansion.out

$(TEST_DIR)/test_key_expansion.out: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_key_expansion.c
	@echo "Compiling AES key expansion tests..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_key_expansion.c -o $@ $(LIBS)

# --- Memory Check ---
.PHONY: memcheck_key_expansion
memcheck_key_expansion: $(AES_SRC) $(UNITY_SRC) $(TEST_DIR)/test_key_expansion.c
	@echo "Running AddressSanitizer on AES key expansion tests..."
	@$(CC) $(ASANFLAGS) $(CFLAGS) $(UNITY_SRC) $(AES_SRC) $(TEST_DIR)/test_key_expansion.c -o $(TEST_DIR)/memcheck_key_expansion.out $(LIBS)
	@./$(TEST_DIR)/memcheck_key_expansion.out
	@echo "Memory check passed ✅"

# ==============================================================
# --- Clean ---
# ==============================================================
.PHONY: clean
clean:
	@rm -rf *.o *.out *.out.dSYM $(TEST_DIR)/*.out
	@echo "Cleaned build artifacts."
