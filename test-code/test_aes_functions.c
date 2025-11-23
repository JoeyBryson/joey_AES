#include "aes.h"
#include "useful_functions.h"
#include "unity.h"
#include <stdint.h>

void setUp(void) {}
void tearDown(void) {}


/*All test data from Appendix B - Cipher example, round 1. On page.34 https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf*/
void test_add_round_key(void) {
    state_t state = {{
        {{0x04, 0xE0, 0x48, 0x28}},
        {{0x66, 0xCB, 0xF8, 0x06}},
        {{0x81, 0x19, 0xD3, 0x26}},
        {{0xE5, 0x9A, 0x7A, 0x4C}}
    }};
    word_t round_key[4] = {
        {{0xA0, 0xFA, 0xFE, 0x17}},
        {{0x88, 0x54, 0x2C, 0xB1}},
        {{0x23, 0xA3, 0x39, 0x39}},
        {{0x2A, 0x6C, 0x76, 0x05}}
    };
    state_t expected = {{
        {{0xA4, 0x1A, 0xB6, 0x3F}},
        {{0xEE, 0x9F, 0xD4, 0xB7}},
        {{0xA2, 0xBA, 0xEA, 0x1F}},
        {{0xCF, 0xF6, 0x0C, 0x49}}
    }};
    add_round_key(&state, round_key);
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            TEST_ASSERT_EQUAL_HEX8(expected.col[c].byte[r], state.col[c].byte[r]);
}

void test_sub_bytes(void) {
    state_t state = {{
        {{0x19, 0x3D, 0xE3, 0xBE}},
        {{0xA0, 0xF4, 0xE2, 0x2B}},
        {{0x9A, 0xC6, 0x8D, 0x2A}},
        {{0xE9, 0xF8, 0x48, 0x08}}
    }};
    state_t expected = {{
        {{0xD4, 0x27, 0x11, 0xAE}},
        {{0xE0, 0xBF, 0x98, 0xF1}},
        {{0xB8, 0xB4, 0x5D, 0xE5}},
        {{0x1E, 0x41, 0x52, 0x30}}
    }};
    init_Sbox();
    sub_bytes(&state);
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            TEST_ASSERT_EQUAL_HEX8(expected.col[c].byte[r], state.col[c].byte[r]);
}

void test_shift_rows(void) {
    state_t state = {{
        {{0xD4, 0x27, 0x11, 0xAE}},
        {{0xE0, 0xBF, 0x98, 0xF1}},
        {{0xB8, 0xB4, 0x5D, 0xE5}},
        {{0x1E, 0x41, 0x52, 0x30}}
    }};
    state_t expected = {{
        {{0xD4, 0xBF, 0x5D, 0x30}},
        {{0xE0, 0xB4, 0x52, 0xAE}},
        {{0xB8, 0x41, 0x11, 0xF1}},
        {{0x1E, 0x27, 0x98, 0xE5}}
    }};
    shift_rows(&state);
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            TEST_ASSERT_EQUAL_HEX8(expected.col[c].byte[r], state.col[c].byte[r]);
}

void test_mix_columns(void) {
    state_t state = {{
        {{0xD4, 0xBF, 0x5D, 0x30}},
        {{0xE0, 0xB4, 0x52, 0xAE}},
        {{0xB8, 0x41, 0x11, 0xF1}},
        {{0x1E, 0x27, 0x98, 0xE5}}
    }};
    state_t expected = {{
        {{0x04, 0x66, 0x81, 0xE5}},
        {{0xE0, 0xCB, 0x19, 0x9A}},
        {{0x48, 0xF8, 0xD3, 0x7A}},
        {{0x28, 0x06, 0x26, 0x4C}}
    }};
    mix_columns(&state);
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            TEST_ASSERT_EQUAL_HEX8(expected.col[c].byte[r], state.col[c].byte[r]);
}

int main(void) {
    UNITY_BEGIN();
    init_Sbox();
    RUN_TEST(test_sub_bytes);
    RUN_TEST(test_shift_rows);
    RUN_TEST(test_mix_columns);
    RUN_TEST(test_add_round_key);
    return UNITY_END();
}
