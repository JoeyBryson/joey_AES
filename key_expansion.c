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
    byte_t first = word.b[0]; 
    word.b[0] = word.b[1];
    word.b[1] = word.b[2]; 
    word.b[2] = word.b[3]; 
    word.b[3] = first; 
    return word;
}

word_t sub_word(word_t word)
{   
    for(int i = 0; i < 4; i++) {
        word.b[i] = Sbox[word.b[i]]; 
    }
    return word;
}

word_t add_words(word_t word1, word_t word2)
{   
    word_t output;
    for(int i = 0; i < 4; i++) {
        output.b[i] = word1.b[i]^word2.b[i];
    }
    return output;
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