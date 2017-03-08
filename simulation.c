// gcc -std=c99 simulation.c misc.c hamming_code.c binary_gauss_jordan.c mt19937ar-cok.c -lm

#include "hamming_code.h"
#include "misc.h"
#include <stdio.h>

typedef void (*Channel_FnPtr)(int*, int*, double, int);
typedef void (*Encoder_FnPtr)(int*, int*);
typedef void (*Decoder_FnPtr)(int*, int*);

double calc_FER_by_MC(double p, long num_of_trials, Encoder_FnPtr encoder, Decoder_FnPtr decoder, Channel_FnPtr channel) {
	long num_of_frame_error = 0;
	
	for(int i = 1; i <= num_of_trials; ++i) {
		int message[K];
		generate_message_at_random(message);
		
		int codeword[N];
		encoder(message, codeword);
		
		int received_word[N];
		channel(codeword, received_word, p, N);
		
		int estimated_word[N];
		decoder(received_word, estimated_word);
				
		if ( neq_narray(codeword, estimated_word, N) )
			++num_of_frame_error;
	}
	
	return (double)num_of_frame_error / num_of_trials;
}

void simulation(Encoder_FnPtr encoder, Decoder_FnPtr decoder, Channel_FnPtr channel, long num_of_trials) {
	for(double p = 1.0; p > 1.0 / num_of_trials; p /= 2) {
		double FER = calc_FER_by_MC(p, num_of_trials, encoder, decoder, channel);
		if (FER < (1.0 / num_of_trials * 10))
			break;
		
		printf("%-4.3f %.6f\n", p, FER);
	}	
}

int main() {
	printf("誤り訂正\n");
	simulation(hamming_encode, hamming_error_decode, BSC, 1e+4);
	
	printf("\n");
	
	printf("消失訂正\n");
	simulation(hamming_encode, hamming_erasure_decode, BEC, 1e+6);
	
	return 0;
}

