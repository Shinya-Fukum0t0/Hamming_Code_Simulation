#include "misc.h"

// i行とj行を入れ替える
void swap_row(int** lhs_mat, int* rhs, int row_size, int col_size, int i, int j) {
	if (i == j) return;
	for (int col = 0; col < col_size; ++col)
		nswap(&lhs_mat[i][col], &lhs_mat[j][col]);
	nswap(&rhs[i], &rhs[j]);
}

void binary_gauss_jordan(int** lhs_mat, int* rhs, int* solution, int row_size, int col_size) {
	for (int i = 0; i < col_size; ++i)
		solution[i] = -1;
	
	for(int row = 0; row < col_size; ++row) {
		int dig_elm = lhs_mat[row][row];
		
		/* 対角要素が0なら，以降の対角要素が1となる行と入れ替える． */
		if (dig_elm == 0) { 
			int swap_idx = row;
			for(; swap_idx < row_size; ++swap_idx)
				if (lhs_mat[swap_idx][row] == 1) break;
			if (swap_idx >= row_size) return;
			swap_row(lhs_mat, rhs, row_size, col_size, row, swap_idx);
		}
		
		/* row行目以外でrow列目の要素が1の行について，row行目を足して0にする． */	
		for(int other_row = 0; other_row < row_size; ++other_row) {
			if (row == other_row) continue;
			if (lhs_mat[other_row][row] == 0) continue;
			for (int col = row; col < col_size; ++col)
				lhs_mat[other_row][col] ^= lhs_mat[row][col];
			rhs[other_row] ^= rhs[row];
		}
	}
	
	for (int i = 0; i < col_size; ++i)
		solution[i] = rhs[i];
}
