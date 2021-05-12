#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "matrix.h"
void print_matrix(int**, char*, int);
double wtime();
void matinit(int ***p_a, int ***p_b, int ***p_c, int len);
int main(int argc, char** argv){
        int **a;
        int **b;
        int **c;
        int i, j, k;
        int len, num_thread;
        double  start, stop;
        if(argc == 2){
                len = atoi(argv[1]);
                num_thread = len;//default value
        }
        else if(argc ==3){
                len = atoi(argv[1]);
                num_thread = atoi(argv[2]);
        }
        else{
                printf("Usage: matrix [row][num_thread]\n");
        }
        matinit(&a, &b, &c, len);
        
	//start = wtime();
        //if(mat_mul(a, b, c, len)!=0){
        //        fprintf(stderr, "Failed to Matrix multiplication\n");
        //        exit(1);
        //}
        //stop = wtime();
        //print_matrix(a, "A", len);
        //print_matrix(b, "B", len);
       	//print_matrix(c, "C", len);
        //printf("Processing time %f\n", (double)(stop-start));
	


	for(int i = 0 ; i < len ; i ++){
		for(int j = 0 ; j < len ; j ++){
			c[i][j] = 0;
		}
	}	
	
	
	printf("\n========== multiplication with threads ==========\n");
        start = wtime();
        if(mat_mul_th(a, b, c, len, num_thread)!=0){
                fprintf(stderr, "Failed to Matrix multiplication\n");
                exit(1);
        }
        stop = wtime();
        //print_matrix(a, "A", len);
        //print_matrix(b, "B", len);
       //print_matrix(c, "C", len);
        printf("Processing time %f\n", (double)(stop-start));
        return 0;
}
void matinit(int ***p_a, int ***p_b, int ***p_c, int len){
        int **a;
        int **b;
        int **c;
        int i, j;
        a = (int **)malloc(len * sizeof(int*));
        b = (int **)malloc(len * sizeof(int*));
        c = (int **)malloc(len * sizeof(int*));
        for(i = 0 ; i < len ; i ++){
                a[i] = (int*)malloc(len * sizeof(int));
                b[i] = (int*)malloc(len * sizeof(int));
                c[i] = (int*)malloc(len * sizeof(int));
        }
        srandom((unsigned int)time(NULL));
        for(i = 0 ; i < len ; i ++){
                for(j = 0 ; j < len ; j ++){
                        a[i][j] = random()%10;
                        b[i][j] = random()%10;
                        c[i][j] = 0;
                }
        }
        *p_a = a;
        *p_b = b;
	*p_c = c;
}
double wtime(){
        static int sec = -1;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        if(sec < 0) sec = tv.tv_sec;
        return (tv.tv_sec - sec)+ 1.0e-6*tv.tv_usec;
}
void print_matrix(int**  matrix, char* name, int len){
        int i, j, k;
        printf("\n%c matrix \n", *name);
        for(i = 0 ; i < len ; i ++){
                for(j = 0 ; j < len ; j ++){
                        printf("%d ", matrix[i][j]);
                }
                printf("\n");
        }
}
