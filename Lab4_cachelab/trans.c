/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 *
 * Pi
 * April 28 2023
 */ 
#include <stdio.h>
#include "cachelab.h"

/* 
 * s = 5, E = 1, b = 5
 * b = 5 -> block/line size = 32 bytes = 8 ints
 * E = 1 -> each set contains 1 line
 * s = 5 -> 32 sets
 * Cache size: 8 * 32 = 256 ints
 */


int is_transpose(int M, int N, int A[N][M], int B[M][N]);


/*
 * Use 8 x 8 blocks, reason:
 * for N = M = 32, each row contains 32 ints, so the cache can store
 * at most 8 rows of input. Thus, block size <= 8, int general, we need
 * to make the block size as much as possible, so we choose 8.
 * 
 * The diagonal block should cause 23 misses: 23 = 7 + 2 * 8
 * The non-diagonal block should case 16 misses: 16 = 2 * 8
 * When M = N = 32, 287 = 16 * 12 + 23 * 4 + 3, very close to estimation.
 * Actually, the extra 3 misses are for function
 */
void blocksize_8_32_32(int N, int M, int A[N][M], int B[M][N]) {
    int i, j, k, l, diag;  // i, j for block index; k, l for element index in block
    for (i = 0; i < N; i += 8) {
        for (j = 0; j < M; j += 8) {
            for (k = 0; k < 8; ++k) {
                for (l = 0; l < 8; ++l) {
                    if (i + k == j + l) {  // diagonal block
                        diag = i + k;
                        continue;
                    }
                    B[j + l][i + k] = A[i + k][j + l];
                }
                if (i == j) {
                    B[diag][diag] = A[diag][diag];
                }
            }
        }    
    }
}


/* 
 * Use 4 x 4 blocks, reason is same as matrix 32 x 32
 * The cache can store at most 4 rows of input matrix.
 * num of diagonal blocks: 16 = 64 / 4
 * num of non-diagonal blocks: 240 = 16 * 16 - 16
 *
 * 4 x 4: 1795 misses
 */
void blocksize_4_64_64(int N, int M, int A[N][M], int B[M][N]) {
    int i, j, k, l, diag;  // i, j for block index; k, l for element index in block
    for (i = 0; i < N; i += 4) {
        for (j = 0; j < M; j += 4) {
            for (k = 0; k < 4; ++k) {
                for (l = 0; l < 4; ++l) {
                    if (i + k == j + l) {  // diagonal block
                        diag = i + k;
                        continue;
                    }
                    B[j + l][i + k] = A[i + k][j + l];
                }
                if (i == j) {
                    B[diag][diag] = A[diag][diag];
                }
            }
        }
    }
}


/* 
 * Use 8 x 8 blocks, but apply some optimization
 * Use B as cache
 * 8 x 8 (4): 1171 misses
 */
void blocksize_8_4_64_64(int N, int M, int A[N][M], int B[M][N]) {
    int i, j, k, l;
    int a0, a1, a2, a3, a4, a5, a6, a7;
    for (i = 0; i < N; i += 8) {
        for (j = 0; j < M; j += 8) {
            for (k = i ; k < i + 4; ++k) {
                a0 = A[k][j];
                a1 = A[k][j + 1];
                a2 = A[k][j + 2];
                a3 = A[k][j + 3];
                a4 = A[k][j + 4];
                a5 = A[k][j + 5];
                a6 = A[k][j + 6];
                a7 = A[k][j + 7];

                B[j][k] = a0;
                B[j + 1][k] = a1;
                B[j + 2][k] = a2;
                B[j + 3][k] = a3;

                B[j][k + 4] = a4;
                B[j + 1][k + 4] = a5;
                B[j + 2][k + 4] = a6;
                B[j + 3][k + 4] = a7;
            }

            for (l = j + 4; l < j + 8; ++l) {
                a4 = A[i + 4][l - 4]; // A left-down col
                a5 = A[i + 5][l - 4];
                a6 = A[i + 6][l - 4];
                a7 = A[i + 7][l - 4];

                a0 = B[l - 4][i + 4]; // B right-up line
                a1 = B[l - 4][i + 5];
                a2 = B[l - 4][i + 6];
                a3 = B[l - 4][i + 7];

                B[l - 4][i + 4] = a4; // set B right-up line 
                B[l - 4][i + 5] = a5;
                B[l - 4][i + 6] = a6;
                B[l - 4][i + 7] = a7;

                B[l][i] = a0;         // set B left-down col
                B[l][i + 1] = a1;
                B[l][i + 2] = a2;
                B[l][i + 3] = a3;

                B[l][i + 4] = A[i + 4][l];  // transpose right-down of A and B
                B[l][i + 5] = A[i + 5][l];
                B[l][i + 6] = A[i + 6][l];
                B[l][i + 7] = A[i + 7][l];
            }
        }
    }
}


/* 
 * The requirement is not too tight, try different block sizes
 * also no need to consider diagonal
 * 4 x 4: 2425 misses
 * 8 x 8: 2118 misses
 * 16 x 16: 1192 misses 
 */
void blocksize_16_61_67(int N, int M, int A[N][M], int B[M][N]) {
    int i, j, k, l;
    int size = 4;
    for (i = 0; i < N; i += size) {
        for (j = 0; j < M; j += size) {
            for (k = 0; k < size && i + k < N; ++k) {
                for (l = 0; l < size && j + l < M; ++l) {
                    B[j + l][i + k] = A[i + k][j + l];
                }
            }
        }
    }
}


/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    if (M == 32 && N == 32) {
        blocksize_8_32_32(N, M, A, B);
    }
    else if (M == 64 && N == 64) {
        // blocksize_4_64_64(N, M, A, B);
        blocksize_8_4_64_64(N, M, A, B);
    }
    else {
        blocksize_16_61_67(N, M, A, B);
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

