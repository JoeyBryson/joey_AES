#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

void buffer_to_state(state_t* state, byte_t* buffer)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state->col[i].byte[j] = buffer[4 * i + j];
		}
	}
}

void state_to_buffer(state_t* state, byte_t* buffer)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			buffer[4 * i + j] = state->col[i].byte[j];
		}
	}
}

state_t x_or_states(state_t state_a, state_t state_b)
{
	state_t output;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			output.col[i].byte[j] = state_a.col[i].byte[j] ^ state_b.col[i].byte[j];
		}
	}
	return output;
}