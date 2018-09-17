/*
 * convolution.c
 *
 *  Created on: Nov 29, 2017
 *      Author: tianyiz
 */


#include<stdlib.h>
#include<time.h>

void convolution(float* A){
	float kernel[3]={1.0/3.0,1.0/2.0,1.0/6.0};
	int lengthA=sizeof(A)/sizeof(A[0]);
	int i;
	float* B=(float*)calloc(lengthA,sizeof(float));


	printf("The result vector value is: ");

	clock_t start=clock();

	for(i=0;i<lengthA;i++){
		if(i==0){
			B[i]=kernel[2]*A[0];
		}
		else if(i==1){
			B[i]=kernel[2]*A[1]+kernel[1]*A[0];
		}
		else if(i==lengthA-1){
			B[i]=kernel[i]*A[0];
		}
		else if(i==lengthA-2){
			B[i]=kernel[i]*A[1]+kernel[i-1]*A[0];
		}
		else{
			B[i]=kernel[2]*A[i]+kernel[1]*A[i-1]+kernel[0]*A[i-2];
		}
	}

	clock_t end=clock();

	for(i=0;i<lengthA;i++){
		printf("%f.2 ",B[i]);
	}

	printf("\nProcess ended. Running time is %f for c output.\n",(double)(end-start));

}


