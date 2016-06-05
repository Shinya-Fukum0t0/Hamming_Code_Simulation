#include "misc.h"
#include "mt19937ar-cok.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 乱数関連 */
void init_random() {
	init_genrand((unsigned)time(NULL));	
}

double uniform_random() {
	return genrand_real2();
}

void generate_message_at_random(int* message) {
	int zero_message[K] = {0};
	BSC(zero_message, message, 0.5, K);
}

/* 2進数関連 */

int flip(int bin_val) {
	if ( bin_val == 0 ) {
		return 1;
	} else if (bin_val == 1) {
		return 0;
	} else {
		fprintf(stderr, "flip関数に0でも1でもない数%dが入力されました．\n", bin_val);
		exit(1);
	}
}

int bin_to_dec(int* bin, int length) {
	int dec = 0;
	for(int i = 0; i < length; ++i)
		dec += bin[i] * pow(2, (length - 1) - i);

	return dec;
}

int inner_prod(int* a, int* b, int size) {
	int ret = 0;
	for(int i = 0; i < size; ++i)
		ret += a[i] * b[i];
		
	return ret % 2;
}

/* 通信路 */
void BSC(int* in_word, int* out_word, double prob, int length) {
	for(int i = 0; i < length; ++i) {
		if ( prob > uniform_random() )
			out_word[i] = flip( in_word[i] );
		else
			out_word[i] = in_word[i];
	}
}

void BEC(int* in_word, int* out_word, double prob, int length) {
	for(int i = 0; i < length; ++i) {
		if (prob > uniform_random() )
			out_word[i] = ERASURE;
		else
			out_word[i] = in_word[i];
	}
}

/* int型配列用ユーティリティ */
void show_narray(int *narray, int length) {
	for(int i = 0; i < length; ++i)
		printf("%d ", narray[i]);
	printf("\n");
}

bool neq_narray(int *a, int*b, int length) {
	for(int i = 0; i < length; ++i) {
		if ( a[i] != b[i] )
			return true;
	}
	return false;
}

void nswap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

