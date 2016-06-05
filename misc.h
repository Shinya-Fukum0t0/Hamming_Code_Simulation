#ifndef MISC_H
#define MISC_H

#include <stdbool.h>

/* 定数 */
#define K 4
#define N 7
#define ERASURE 2

/* 乱数関連 */
void init_random();
double uniform_random();
void generate_message_at_random(int* message);

/* 2進数関連 */
int flip(int bin_val);
int bin_to_dec(int* bin, int length);
int inner_prod(int* a, int* b, int size);

/* 通信路 */
void BSC(int* in_word, int* out_word, double prob, int length);
void BEC(int* in_word, int* out_word, double prob, int length);

/* int型配列用ユーティリティ */
void show_narray(int *narray, int length);
bool neq_narray(int *a, int*b, int length);

void nswap(int* a, int* b);

#endif // MISC_H
