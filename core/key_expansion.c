#include "aes.h"
#include <stdio.h>

#include <stdint.h>

static const byte_t round_constants[10] = {
    0x01, 0x02, 0x04, 0x08,
    0x10, 0x20, 0x40, 0x80,
    0x1B, 0x36
};


word_t rot_word(word_t word)
{
    byte_t first = word.byte[0]; 
    word.byte[0] = word.byte[1];
    word.byte[1] = word.byte[2]; 
    word.byte[2] = word.byte[3]; 
    word.byte[3] = first; 
    return word;
}

word_t sub_word(word_t word)
{   
    for(int i = 0; i < 4; i++) {
        word.byte[i] = Sbox(word.byte[i]); 
    }
    return word;
}

round_keys_t expand_key(aes_key_t key)
{
    round_keys_t round_keys;
    round_keys.num_rounds = key.num_key_words + 6;
    round_keys.words = malloc((4*(round_keys.num_rounds + 1))*sizeof(word_t));

    int i = 0;
    for(; i < key.num_key_words; i++) {
        round_keys.words[i] = key.words[i];
    }
    for(; i < 4*round_keys.num_rounds + 4; i++) {
        word_t temp = round_keys.words[i-1];
        if(i%key.num_key_words == 0) {
            temp = rot_word(temp);
            temp = sub_word(temp);
            temp = add_words(temp, (word_t){{round_constants[i/key.num_key_words - 1], 0x00, 0x00, 0x00}});
        }
        else if(key.num_key_words>6 && i % key.num_key_words == 4) {
            temp = sub_word(temp);
        }
        round_keys.words[i] = add_words(round_keys.words[i-key.num_key_words], temp);
    }
    return round_keys;
}