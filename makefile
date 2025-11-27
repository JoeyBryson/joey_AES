ifeq ($(OS),Windows_NT)
    # Native Windows build
    CC = gcc
    LDFLAGS = -lbcrypt
    EXE_EXT = .exe
else
    # Check if cross-compiling for Windows from Linux
    ifdef CROSS_COMPILE_WIN
        CC = x86_64-w64-mingw32-gcc
        LDFLAGS = -lbcrypt
        EXE_EXT = .exe
    else
        # Native Linux build
        CC = gcc
        LDFLAGS =
        EXE_EXT =
    endif
endif

CFLAGS  = -std=c99
CFLAGS += -I.
CFLAGS += -g
CFLAGS += -Wall -Wextra -pedantic
CFLAGS += -DUNITY_SUPPORT_64 -DUNITY_OUTPUT_COLOR

ROOT_DIR := $(CURDIR)
BUILD_DIR := $(ROOT_DIR)/build
TEST_DIR := $(ROOT_DIR)/test-code
LIB_UTIL = $(BUILD_DIR)/libfileutils.a
LIB_CORE = $(BUILD_DIR)/libcore.a
LIBS = $(LIB_UTIL) $(LIB_CORE)
CLI_BINARY = $(BUILD_DIR)/cipher-cli

CFLAGS += -I$(ROOT_DIR)/include
CFLAGS += -I$(ROOT_DIR)/test-framework

export CC
export CFLAGS
export LDFLAGS
export EXE_EXT
export ROOT_DIR
export BUILD_DIR
export TEST_DIR
export LIB_UTIL
export LIB_CORE
export LIBS
export CLI_BINARY


.PHONY: all cli tests clean libs

all: cli

cli: 
	$(MAKE) -C src all

test: libs
	$(MAKE) -C test-code test

libs: 
	$(MAKE) -C src libs 
	

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test-code clean

test_file_utils: libs
	$(MAKE) -C test-code test_file_utils

test_Sbox_gen: libs
	$(MAKE) -C test-code test_Sbox_gen

test_key_expansion: libs
	$(MAKE) -C test-code test_key_expansion
 
test_aes_functions: libs
	$(MAKE) -C test-code test_aes_functions

test_cipher: libs
	$(MAKE) -C test-code test_cipher

test_inv_cipher: libs
	$(MAKE) -C test-code test_inv_cipher

test_key_gen: libs
	$(MAKE) -C test-code test_key_gen

install:
ifeq ($(OS),Windows_NT)
	@echo "Installing joeyaes.exe to C:\Windows\System32"
	@copy /Y build\joeyaes.exe C:\Windows\System32\joeyaes.exe
else
	@echo "Installing joeyaes to /usr/local/bin"
	install -m 755 build/joeyaes /usr/local/bin/joeyaes
endif

uninstall:
ifeq ($(OS),Windows_NT)
	@echo "Removing joeyaes.exe from C:\Windows\System32"
	@del C:\Windows\System32\joeyaes.exe
else
	@echo "Removing joeyaes from /usr/local/bin"
	rm -f /usr/local/bin/joeyaes
endif
