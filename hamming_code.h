#ifndef HAMMING_CODE_H
#define HAMMING_CODE_H

#include "misc.h"

void hamming_encode(int* message, int* codeword);
void hamming_error_decode(int* received_word, int* estimated_word);
void hamming_erasure_decode(int* received_word, int* estimated_word);
bool hamming_is_codeword(int * word);
//void calc_syndrome(int* word, int* syndrome);

#endif // HAMMING_CODE_H
