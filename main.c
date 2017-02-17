#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n 4
#define pi 3.14

struct timespec diff(struct timespec start, struct timespec end);

float** matrix_init() {
	float** matrix;
	matrix = (float**)malloc(sizeof(float*)*n);
	for (int i = 0; i < n; i++) {
		matrix[i] = (float*)malloc(sizeof(float)*n);
		for (int j = 0; j < n; j++) {
				matrix[i][j] = (float)(rand() % 100) / pi;
			}
		}
	return matrix;
}

float** matrix_init_zero() {
	float** matrix;
	matrix = (float**)malloc(sizeof(float*)*n);
	for (int i = 0; i < n; i++) {
		matrix[i] = (float*)calloc(n, sizeof(float));
	}
	return matrix;
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

float** matrix_mul(float** a, float** b) {
	float** c;
	c = matrix_init_zero();
    struct timespec time1, time2, res;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) 
			for (int r = 0; r < n; r++)
				c[i][j] += a[i][r] * b[r][j];
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	res = diff(time1,time2);
	printf("%lld.", (long long)res.tv_sec);
	printf("%ld\n", res.tv_nsec);
	return c;
}

void print_mat(float** mat) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
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

	matrix_c = matrix_mul(matrix_a, matrix_b);
	print_mat(matrix_c);

	return 0;
}
