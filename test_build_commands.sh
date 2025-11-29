#!/bin/bash

# Script to test all build commands from BUILD.md
# This script tests both Linux and Windows (cross-compile) builds

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "======================================"
echo "Testing Build Commands for joey_AES"
echo "======================================"
echo ""

# Function to print test status
print_status() {
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ PASSED${NC}: $1"
    else
        echo -e "${RED}✗ FAILED${NC}: $1"
        exit 1
    fi
}

# Function to check if file exists
check_file_exists() {
    if [ -f "$1" ]; then
        echo -e "${GREEN}✓ File exists${NC}: $1"
        return 0
    else
        echo -e "${RED}✗ File missing${NC}: $1"
        return 1
    fi
}

# Clean before starting
echo "=== Cleaning previous builds ==="
make clean
print_status "make clean"
echo ""

# Test 1: Build Linux binary
echo "=== Test 1: Building Linux binary ==="
make
print_status "make (Linux build)"
check_file_exists "build/joeyaes"
echo ""

# Test 2: Clean again
echo "=== Test 2: Clean build ==="
make clean
print_status "make clean"
echo ""

# Test 3: Build Windows binary (cross-compile)
echo "=== Test 3: Cross-compiling for Windows ==="
if command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    make w=1
    print_status "make w=1 (Windows cross-compile)"
    check_file_exists "build/joeyaes.exe"
else
    echo -e "${YELLOW}⚠ SKIPPED${NC}: MinGW-w64 not installed (cross-compiler not available)"
fi
echo ""

# Test 4: Clean again
echo "=== Test 4: Clean build ==="
make clean
print_status "make clean"
echo ""

# Test 5: Build libs
echo "=== Test 5: Building libraries ==="
make libs
print_status "make libs"
check_file_exists "build/libfileutils.a"
check_file_exists "build/libcore.a"
echo ""

# Test 6: Run all tests (Linux)
echo "=== Test 6: Running all tests (Linux) ==="
make tests
print_status "make tests"
echo ""

# Test 7: Run individual test files (Linux)
echo "=== Test 7: Running individual tests (Linux) ==="
test_targets=(
    "test_file_utils"
    "test_Sbox_gen"
    "test_key_expansion"
    "test_aes_functions"
    "test_cipher"
    "test_inv_cipher"
    "test_key_gen"
)

for test in "${test_targets[@]}"; do
    echo "Running: make $test"
    make "$test"
    print_status "make $test"
done
echo ""

# Test 8: Clean again
echo "=== Test 8: Clean build ==="
make clean
print_status "make clean"
echo ""

# Test 9: Run all tests (Windows cross-compile)
if command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo "=== Test 9: Running all tests (Windows cross-compile) ==="
    make tests w=1
    print_status "make tests w=1"
    echo ""
    
    # Test 10: Run individual test files (Windows)
    echo "=== Test 10: Running individual tests (Windows) ==="
    for test in "${test_targets[@]}"; do
        echo "Running: make $test w=1"
        make "$test" w=1
        print_status "make $test w=1"
    done
    echo ""
else
    echo -e "${YELLOW}⚠ SKIPPED${NC}: Tests 9-10 - MinGW-w64 not installed"
fi

# Test 11: Build CLI
echo "=== Test 11: Building CLI ==="
make clean
make cli
print_status "make cli"
check_file_exists "build/joeyaes"
echo ""

# Test 12: Install (requires sudo - optional)
echo "=== Test 12: Install/Uninstall (optional - requires sudo) ==="
read -p "Do you want to test 'make install' and 'make uninstall'? (requires sudo) [y/N]: " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    sudo make install
    print_status "sudo make install"
    check_file_exists "/usr/local/bin/joeyaes"
    
    sudo make uninstall
    print_status "sudo make uninstall"
    
    if [ ! -f "/usr/local/bin/joeyaes" ]; then
        echo -e "${GREEN}✓ File removed${NC}: /usr/local/bin/joeyaes"
    else
        echo -e "${RED}✗ File still exists${NC}: /usr/local/bin/joeyaes"
    fi
else
    echo -e "${YELLOW}⚠ SKIPPED${NC}: Install/uninstall tests"
fi
echo ""

# Final cleanup
echo "=== Final cleanup ==="
make clean
print_status "make clean"
echo ""

echo "======================================"
echo -e "${GREEN}All tests completed successfully!${NC}"
echo "======================================"
