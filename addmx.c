#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_STR_SIZE 64
#define CHAR_INT_OFFSET 48

void printusages(){
	printf("usage: addmx file1 file2\n");
}

typedef struct {
	int n;
	int m;
	int *vals;
} matrix;

matrix* matrix_new(int n, int m){
	matrix* u = (matrix*) malloc(sizeof(matrix));
	u->n=n;
	u->m=m;
	u->vals = (int*) malloc((u->n * u->m) * sizeof(int));
	return u;
}

int matrix_get(int i, int j, matrix* u){
	return *(u->vals + i * u->m +j);
}

void matrix_set(int i, int j, int val, matrix* u){
	if(i<u->n && i>=0 && j<u->m && j>=0){
		*(u->vals + i * u->m +j)=val;
	}
}

void matrix_print(matrix* u){
	int i,j;
	printf("%d x %d \n", u->n, u->m);
	for(i=0; i<u->n; i++){
		for(j=0; j<u->m; j++){
			printf("%d ", matrix_get(i,j,u));
		}
		printf("\n");
	
	}
}

int main(int argc, char *argv[]){
	FILE *fp1, *fp2;
	int n1, n2, m1, m2, index=-1;
	bool parent=true;
	switch(argc){
		case 1:printusages(); return EXIT_FAILURE;
		case 2:printusages(); return EXIT_FAILURE;
		case 3:fp1 = fopen(argv[1], "r");
		       fp2 = fopen(argv[2], "r"); break;
		default: printusages(); return EXIT_FAILURE;
	}

	if(fp1==NULL || fp2==NULL){
		printf("A file couldn't be read\n");
		return 0;
	}	

	
	n1=fgetc(fp1)-CHAR_INT_OFFSET;
	n2=fgetc(fp2)-CHAR_INT_OFFSET;
	fgetc(fp1); // skip 'x' in file1 line 1	
	fgetc(fp2); // skip 'x' in file2 line 1
	m1=fgetc(fp1)-CHAR_INT_OFFSET;
	m2=fgetc(fp2)-CHAR_INT_OFFSET;
	fgetc(fp1); // skip '\n' in file1 line 1	
	fgetc(fp2); // skip '\n' in file2 line 1

	printf("n1:%d, n2:%d, m1:%d, m2:%d\n", n1, n2, m1, m2);

	if(n1!=n2 || m1!=m2){
		printf("The matrices are not of the same size!\n");
		return 0;
	}
	
	matrix* mtx1 = matrix_new(n1, m1);
	matrix* mtx2 = matrix_new(n2, m2);
	matrix* mtx3 = matrix_new(n1, m1);
	
	int i, j;
	char c1, c2;

	for(i=0; i<n1; i++){
		for(j=0; j<m1; j++){
			c1 = fgetc(fp1);
			c2 = fgetc(fp2);
			if(c1!=' ' && c1!='\n' && c2!=' ' && c2!='\n'){
				matrix_set(i,j,(int)(c1)-CHAR_INT_OFFSET,mtx1);
				matrix_set(i,j,(int)(c2)-CHAR_INT_OFFSET,mtx2);
			}else{
				j--;
			}
		}

	}

	for(i=0; i<m1; i++){ // creates m children directly from parent
		if(!parent){break;}
		if(fork()==0){
			parent=false;
		}
		index++;
	}
	if(!parent){
		for(i=0; i<n1; i++){
			matrix_set(i, index, matrix_get(i, index, mtx1) + matrix_get(i,index,mtx2), mtx3);
		}
		exit(0);
	}else{
		pid_t wpid;
		while((wpid=wait(NULL))>0);
	}
	matrix_print(mtx3);

return (0);
}

