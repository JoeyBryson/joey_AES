#include "aes.h"


byte_t two_times(byte_t byte) 
{
    if(byte & 0x80) {
        byte <<= 1;
        byte ^= 0x1B;
    } else {
        byte <<= 1;
    }
    return byte;
}

byte_t GF_mul(byte_t a, byte_t b) 
{
    byte_t result = 0;
    byte_t x_pow_two = b;
    for(int i = 0; i < 8; i++) {
        if(a & 1) {
            result ^= x_pow_two;
        }
        x_pow_two = two_times(x_pow_two);
        a >>= 1;
    }
    return result;
}
