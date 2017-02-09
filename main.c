#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define nout 220
#define nin 4
#define pi 3.14

struct timespec diff(struct timespec start, struct timespec end);

float**** matrix_init(){
	float**** matrix;
	matrix = (float****)malloc(sizeof(float***)*nout);
	for(int yout = 0; yout < nout; yout++){
		matrix[yout] = (float***)malloc(sizeof(float**)*nout);
		for(int xout = 0; xout < nout; xout++){
			matrix[yout][xout] = (float**)malloc(sizeof(float*)*nin);
			for(int yin = 0; yin < nin; yin++){
				matrix[yout][xout][yin] = (float*)malloc(sizeof(float)*nin);
				for(int xin = 0; xin < nin; xin++){
					srand(time(NULL));
					int r = rand() % 100;
					matrix[yout][xout][yin][xin] = (float)r / pi;
				}
			}
		}
	}
	return matrix;
}

float**** matrix_init_zero(){
	float**** matrix;
	matrix = (float****)malloc(sizeof(float***)*nout);
	for(int yout = 0; yout < nout; yout++){
		matrix[yout] = (float***)malloc(sizeof(float**)*nout);
		for(int xout = 0; xout < nout; xout++){
			matrix[yout][xout] = (float**)malloc(sizeof(float*)*nin);
			for(int yin = 0; yin < nin; yin++){
				matrix[yout][xout][yin] = (float*)malloc(sizeof(float)*nin);
				for(int xin = 0; xin < nin; xin++){
					matrix[yout][xout][yin][xin] = 0;
				}
			}
		}
	}
	return matrix;
}

struct timespec diff(struct timespec start, struct timespec end){
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

float**** matrix_mul(float**** a, float**** b){
	float**** c;
	c = matrix_init_zero();

	struct timespec time1, time2, res;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	for(int jout = 0; jout < nout; jout++)
		for(int iout = 0; iout < nout; iout++)
			for(int rout = 0; rout < nout; rout++)
				for(int iin = 0; iin < nin; iin++)	
					for(int rin = 0; rin <nin; rin++)
						for(int jin = 0; jin < nin; jin++)
							c[jout][iout][jin][iin] += a[rout][jout][rin][iin] * b[rout][jout][rin][jin];
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	res = diff(time1,time2);
	printf("%lld.", (long long)res.tv_sec);
	printf("%ld\n", res.tv_nsec);
	return c;
}

int main(){	
	float**** matrix_a;
	float**** matrix_b;
	float**** matrix_c;
	
	matrix_a = matrix_init();
	matrix_b = matrix_init();
	matrix_c = matrix_init();

	matrix_c = matrix_mul(matrix_a, matrix_b);

	return 0;
}
