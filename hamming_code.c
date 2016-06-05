#include "misc.h"
#include "hamming_code.h"
#include "binary_gauss_jordan.h"
#include <stdlib.h>

static int t_gen_mat[][K] = {
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 1},
	{1, 0, 1, 1},
	{1, 1, 0, 1},
	{0, 1, 1, 1}
};

static int parity_mat[N - K][N] = {
	{1, 0, 1, 1, 1, 0, 0},
	{1, 1, 0, 1, 0, 1, 0},
	{0, 1, 1, 1, 0, 0, 1}
};

/* 内部で利用する関数郡 */
void calc_syndrome(int* word, int* syndrome) {
	for (int i = 0; i < N - K; ++i)
		syndrome[i] = inner_prod(parity_mat[i], word, N);
}

// 最大でmax_num_of_erasure個の消失シンボルの位置を検出し，それらのインデックスをerasure_idxに格納する．
// 返り値は受信語に含まれる消失シンボルの数で，max_num_of_erasure個以上あった場合-1を返す．
int find_erasure_idx(int* received_word, int* erasure_idx, int max_num_of_erasure) {
	int num_of_erasure = 0;
	for (int i = 0; i < N; ++i) {
		if (received_word[i] == ERASURE) {
			++num_of_erasure;
			if (num_of_erasure > max_num_of_erasure)
				return -1;
			erasure_idx[num_of_erasure - 1] = i;
		}
	}
	return num_of_erasure;
}

/* 外部に公開する関数郡 */

void hamming_encode(int* message, int* codeword) {	
	for (int i = 0; i < N; ++i)
		codeword[i] = inner_prod(t_gen_mat[i], message, K);
}

void hamming_error_decode(int* received_word, int* estimated_word) {
	static int error_locator[N + 1] = {-1, 6, 5, 1, 4, 2, 0, 3};

	for (int i = 0; i < N; ++i)
		estimated_word[i] = received_word[i];

	int syndrome[N - K];
	calc_syndrome(received_word, syndrome);
	
	int d_syndrome = bin_to_dec(syndrome, N - K);
	if (d_syndrome != 0) {
		int error_idx = error_locator[d_syndrome];
		estimated_word[ error_idx ] = flip( received_word[ error_idx ] );
	}
}

void hamming_erasure_decode(int* received_word, int* estimated_word) {
	for (int i = 0; i < N; ++i)
		estimated_word[i] = received_word[i];

	int erasure_idx[2] = {-1, -1};
	int num_of_erasure = find_erasure_idx(received_word, erasure_idx, 2);
	
	if (num_of_erasure <= 0 || 2 < num_of_erasure)
		return;
	
	/* Hx = 0をAe = bとして解く */
	int solution[2];
	{
		/* 左辺の消失していない列の項を右辺に移行 */
		int rhs[N - K] = {};
		for(int i = 0; i < N - K; ++i) {
			for(int j = 0; j < N; ++j) {
				if (j != erasure_idx[0] && j != erasure_idx[1])
					rhs[i] ^= parity_mat[i][j] * received_word[j];
			}
		}
	
		/* 消失している列の項を左辺の行列とする */
		int** lhs_mat = (int**)malloc((N - K) * num_of_erasure * sizeof(int*));
		for (int i = 0; i < N - K; ++i)
			lhs_mat[i] = (int*)malloc(num_of_erasure * sizeof(int));
		
		for (int row = 0; row < N - K; ++row) {
			for (int col = 0; col < num_of_erasure; ++col )
				lhs_mat[row][col] = parity_mat[row][ erasure_idx[col] ];
		}
	
		/* Ae = bを解き，eを求める． */
		binary_gauss_jordan(lhs_mat, rhs, solution, N - K, num_of_erasure);
		
		/* メモリの解放 */
		for (int i = 0; i < N - K; ++i)
			free(lhs_mat[i]);
		free(lhs_mat);
	}
	
	/* 解を推定語に反映 */
	for (int i = 0; i < num_of_erasure; ++i)
		estimated_word[ erasure_idx[i] ] = solution[i];
}

