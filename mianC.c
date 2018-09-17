/*
 * mianC.c
 *
 *  Created on: Nov 29, 2017
 *      Author: tianyiz
 */

#include<stdlib.h>
#include<stdio.h>
#include<time.h>


/*
void conv_kernel(float* A,int* B, float* result)

{
	int i;
    float array[3]={1.0/3.0,1.0/2.0,1.0/6.0};


    for(i=0;i<*B;i++){

    //clock_t start=clock();

    if(i==0)
    	{result[0]= (array[2]*A[1]+array[1]*A[0]);}

    else if(i==(*B-1))
    {result[i]= (array[1]*A[i]+array[0]*A[i-1]);}

    else
    {result[i]= (array[i-1]*A[0]+array[i]*A[1]+array[i+1]*A[2]);}

    printf("result %d: %.3f\n",i,result[i]);

    }



    //clock_t end=clock();

    //for(i=0;i<count;i++){printf("%.3f ",B[i]);}

    //printf("\nProcess ended. Running time is %f for c output.\n",(double)(end-start)/CLOCKS_PER_SEC);



}
*/

void conv_kernel(float* A,int* size, float*product)

{
	int i;
	float kernel_a[3] = {1.0/3.0, 1.0/2.0, 1.0/6.0};
	float tmp;

	for(i=0;i<*size;i++){
	if(i == 0)
		tmp = (A[i]*kernel_a[i+1] + A[i+1]*kernel_a[i+2]);
	else if (i != 0 && i != *size - 1)
		tmp = (A[i-1]*kernel_a[0] + A[i]*kernel_a[1] + A[i+1]*kernel_a[2]);
	else if (i != 0 && i == *size - 1)
		tmp = (A[i - 1]*kernel_a[0] + A[i]*kernel_a[1]);

	product[i] = tmp;
    printf("result %d: %.3f\n",i,product[i]);

	}
}

int main(){
	float A[5]={1,2,3,4,5};
	float B[10];
	int i=5;
	conv_kernel(A,&i,B);

	return 0;
}


/*
 * convolution.c
 *
 *  Created on: Nov 29, 2017
 *      Author: tianyiz
 */





