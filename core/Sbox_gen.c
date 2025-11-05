#include "aes.h"
#include <stdio.h>

byte_t Sbox_arr[256];
byte_t inv_Sbox_arr[256];
bool Sbox_is_initialized;

//
byte_t affine_transform(byte_t byte)
{
    byte_t mat_row = 0x8F; // 10001111₂
    byte_t add_vector = 0x63; // 01100011₂
    byte_t output = 0;

    for(int i = 0; i < 8; i++) {
        byte_t rotated = (mat_row >> i) | (mat_row << (8 - i));
        uint8_t bit = 0;

        for (int k = 0; k < 8; k++) {
            uint8_t byte_bit = ((byte >> (7-k)) & 1);
            uint8_t mat_bit = ((rotated >> k) & 1);
            bit ^= byte_bit & mat_bit;
        }

        uint8_t add = (add_vector >> i) & 1;
        uint8_t out_bit = bit ^ add;
        output |= (out_bit << i);
    }

    return output;
}

//byte^(-1) = byte^(254) = 
//byte^(11111110₂)= 
//byte^(2^7+2^6+2^5+2^4+2^3+2^2+2^1) = 
//byte^(2^1)*byte(2^2)...
byte_t inverse(byte_t byte)
{
    if(byte == 0) return 0;
    byte_t squares = byte;
    byte_t mask = 254;
    byte_t result = 1;

    for(int i = 0; i < 8; i++) {
        if(1 & mask) {
            result = GF_mul(result, squares);
        }
        squares = GF_mul(squares, squares);
        mask >>= 1;
    }
    return result;
}

void init_Sbox(void)
{
    if(Sbox_is_initialized) return;

    for (int i = 0; i < 256; i++) {
        byte_t byte = (byte_t) i;
        byte = inverse(byte);
        byte = affine_transform(byte);
        Sbox_arr[i] = byte;
        inv_Sbox_arr[byte] = i;
    }

    Sbox_is_initialized = true;
}
