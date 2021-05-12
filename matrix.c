#include "matrix.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int mat_mul(int** a, int** b, int** c, int len){
        int i, j, k;
        for(i = 0 ; i < len ; i ++){
                for(j = 0 ; j < len ; j ++){
                        c[i][j] = 0;
                        for(k = 0 ; k < len ; k ++){
                                c[i][j] += a[i][k] * b[k][j];
                        }
                }
        }
        return 0;
}
int mat_add(int** a, int** b, int**c, int len){
        int i, j;
        for(int i = 0 ; i < len ; i ++){
                c[i][j] = 0;
                for(int j = 0 ; j < len ; j ++){
                        c[i][j] = a[i][j] + b[i][j];
                }
        }
        return 0;
}
int mat_sub(int** a, int** b, int**c, int len){
        int i, j;
        for(int i = 0 ; i < len ; i ++){
                c[i][j] = 0;
                for(int j = 0 ; j < len ; j ++){
                        c[i][j] = a[i][j] - b[i][j];
                }
        }
        return 0;
}
int mat_inv(int** a, int** b, int len){
        int i, j, k;
}
typedef struct{
        int i;
        int** src1;
        int** src2;
        int** dst;
        int len;
}matmul_arg_t;

void *mat_mul_th_kernel(void *arg){
        int j, k;
        matmul_arg_t *parg =(matmul_arg_t*)arg;
        int i = parg->i;
        int **src1 = parg->src1;
        int **src2 = parg->src2;
        int **dst = parg->dst;
        int len = parg->len;
        //printf("\nthread %d is started", i);
        int result = 0;
        for(j = 0 ; j < len ; j ++){
                result = 0;
                for(k = 0 ; k < len ; k ++){
                        result += src1[i][k] * src2[k][j];
                }
                dst[i][j] = result;
        }
}

int mat_mul_th(int** src1, int** src2, int** dst, int len, int num_thread){
        int i, j ,res;
        matmul_arg_t *arg;
        pthread_t *a_thread;
        void *thread_result;
	if(len < num_thread){
		num_thread = len;
	}	
        a_thread = (pthread_t *)malloc(num_thread * sizeof(pthread_t)); 
	arg = (matmul_arg_t *)malloc(len * sizeof(matmul_arg_t));
	int unit_size = len/num_thread;
	
	for(i = 0 ; i < num_thread ; i ++){
		for(j = i*unit_size; j <  (i+1) * unit_size ; j++){
			arg[j].i = j;
			arg[j].src1 = src1;
			arg[j].src2 = src2;
			arg[j].dst = dst;
			arg[j].len = len;
			res = pthread_create(a_thread + i , NULL, mat_mul_th_kernel, (void*)(arg+j));
			//printf("thread(%d : %d) is created!!\n", i, j);
			if(res != 0){
				perror("Thread creation failed");
				exit(EXIT_FAILURE);
			}	
		}
	}

	for(i = 0 ; i < len - (unit_size * num_thread) ; i ++){
		int k = (unit_size * num_thread) + i;
		arg[k].i = k;
		arg[k].src1 = src1;
		arg[k].src2 = src2;
		arg[k].dst = dst;
		arg[k].len = len;
		res = pthread_create(a_thread+i, NULL, mat_mul_th_kernel, (void*)(arg+k));
		if(res != 0){
			perror("Thread creation failed");
			exit(EXIT_FAILURE);
		}
	}
        for(i = 0 ; i < num_thread ; i ++){
                res = pthread_join(a_thread[i], &thread_result);
                if(res != 0){
                        perror("Thread join failed");
                        exit(EXIT_FAILURE);
                }
        }

	free(arg);
	free(a_thread);
	
	return 0;
}
// 0: Success
// 1: Inberse matrix isn't existed.
