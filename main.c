#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <immintrin.h>

#define N 1024
#define PI 3.14
#define L3_BLOCK_SIZE 512
#define L2_BLOCK_SIZE 128
#define L1_BLOCK_SIZE 32

struct timespec diff(struct timespec start, struct timespec end);

float** matrix_init() {
    float** mat;
    struct timespec tmp;
    mat = (float**)malloc(sizeof(float*) * N);
    for (int i = 0; i < N; i++) {
        mat[i] = (float*)malloc(sizeof(float) * N);
        for (int j = 0; j < N; j++) {
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tmp);
            srand(tmp.tv_nsec);
            mat[i][j] = (float)(rand() % 100) / PI;
        }
    }
    return mat;
}

void matrix_zero(float** mat) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = 0;
        }
    }
}



struct timespec diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

void matrix_naive(float** c, float** a, float** b)
{
    matrix_zero(c);
    struct timespec time1, time2, res;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int r = 0; r < N; r++)
                c[i][j] += a[i][r] * b[r][j];

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    res = diff(time1,time2);
    dprintf(2, "naive: %lld.", (long long)res.tv_sec);
    dprintf(2, "%ld\n", res.tv_nsec);
}

void dgemm(float** c, float** a, float** b)
{
    matrix_zero(c);
    struct timespec time1, time2, res;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

    for(int cy = 0; cy < N; cy++ ){
        for(int by = 0; by < N; by++){
            for(int bx = 0; bx < N; bx++){
                c[cy][bx] += a[cy][by] * b[by][bx];
            }
        }
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    res = diff(time1,time2);
    dprintf(2, "advanced: %lld.", (long long)res.tv_sec);
    dprintf(2, "%ld\n", res.tv_nsec);
}

void dgemm_intrinsics(float** c, float** a, float** b)
{
    matrix_zero(c);
    struct timespec time1, time2, res;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

    __m256 aa, bb, cc;
    for(size_t cy = 0; cy < N; cy++ ){
        for(size_t by = 0; by < N; by++){
            aa = _mm256_broadcast_ss(&(a[cy][by]));
            for(size_t bx = 0; bx < N; bx += 8){
                bb = _mm256_loadu_ps(&(b[by][bx]));
                cc = _mm256_loadu_ps(&(c[cy][bx]));
                cc = _mm256_add_ps(cc, _mm256_mul_ps(aa, bb));
                _mm256_storeu_ps(&(c[cy][bx]), cc);
            }
        }
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    res = diff(time1,time2);
    dprintf(2, "avx: %lld.", (long long)res.tv_sec);
    dprintf(2, "%ld\n", res.tv_nsec);
}

void dgemm_cache(float** c, float** a, float** b){
    matrix_zero(c);
    struct timespec time1, time2, res;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

    for(int cy = 0; cy < N; cy += L3_BLOCK_SIZE){
        for(int by = 0; by < N; by += L3_BLOCK_SIZE){
            for(int bx = 0; bx < N; bx+=L3_BLOCK_SIZE){

                for(int cy3 = cy; cy3 < cy + L3_BLOCK_SIZE; cy3 += L2_BLOCK_SIZE){
                    for(int by3 = by; by3 < by + L3_BLOCK_SIZE; by3 += L2_BLOCK_SIZE){
                        for(int bx3 = bx; bx3 < bx + L3_BLOCK_SIZE; bx3 += L2_BLOCK_SIZE){

                            for(int cy2 = cy3; cy2 < cy3 + L2_BLOCK_SIZE; cy2 += L1_BLOCK_SIZE){
                                for(int by2 = by3; by2 < by3 + L2_BLOCK_SIZE; by2 += L1_BLOCK_SIZE){
                                    for(int bx2 = bx3; bx2 < bx3 + L2_BLOCK_SIZE; bx2 += L1_BLOCK_SIZE){

                                        for(int cy1 = cy2; cy1 < cy2 + L1_BLOCK_SIZE; cy1++){
                                            for(int by1 = by2; by1 < by2 + L1_BLOCK_SIZE; by1++){
                                                for(int bx1 = bx2; bx1 < bx2 + L1_BLOCK_SIZE; bx1++){
                                                    c[cy1][bx1] += a[cy1][by1] * b[by1][bx1];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    res = diff(time1,time2);
    dprintf(2, "cache: %lld.", (long long)res.tv_sec);
    dprintf(2, "%ld\n", res.tv_nsec);
}

void print_mat(float** mat) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", mat[i][j]);
        }
        printf("\n");
    }
}

int main() {
    float** matrix_a;
    float** matrix_b;
    float** matrix_c;

    matrix_a = matrix_init();
    matrix_b = matrix_init();
    matrix_c = matrix_init();
    /** printf("%f", matrix_b[0][0]); */

    /** matrix_a = matrix_init(); */
    /** matrix_b = matrix_init(); */
    /** matrix_c = matrix_init(); */

    /** matrix_naive(matrix_c, matrix_a, matrix_b); */
    /** print_mat(matrix_c); */

    dgemm(matrix_c, matrix_a, matrix_b);
    /** dgemm_intrinsics(matrix_c, matrix_a, matrix_b); */
    dgemm_cache(matrix_c, matrix_a, matrix_b);
    /** print_mat(matrix_c);  */

    return 0;
}
