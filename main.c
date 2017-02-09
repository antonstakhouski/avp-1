#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define nout 150
#define nin 4
#define pi 3.14

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

float**** matrix_mul(float**** a, float**** b){
	float**** c;
	c = matrix_init_zero();
	clock_t start = clock();

	for(int jout = 0; jout < nout; jout++)
		for(int iout = 0; iout < nout; iout++)
			for(int rout = 0; rout < nout; rout++)
				for(int jin = 0; jin < nin; jin++)	
					for(int iin = 0; iin < nin; iin++)
						for(int rin = 0; rin < nin; rin++)
							c[jout][iout][jin][iin] += a[rout][jout][rin][iin] * b[rout][jout][rin][jin];
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("%f", seconds);
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
