/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/*
 * Please fill in the following team_t struct
 */
team_t team = {
        "BB && YEK",    /* Team Name */

        "e2521292",    /* First student ID */
        "Baran Boloğur",    /* First student name */
        
        "e2521722",        /* Second student ID */
        "Yunus Emre Keleş"         /* Second student name */

};


/*******************************************************
 * BATCHED MATRIX MULTIPLICATION \W SUM REDUCTION KERNEL
 *******************************************************/

/*********************************************************************************
 * Your different versions of the batched matrix multiplication functions go here
 *********************************************************************************/

/*
 * naive_batched_mm - The naive baseline version of batched matrix multiplication
 */
char naive_batched_mm_descr[] = "naive_batched_mm: Naive baseline implementation";
void naive_batched_mm(int dim, int *b_mat, int *mat, int *dst) {
    int i,j,k,l;
    
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            for (k = 0; k < dim; k++) {
            	if (i == 0){
            	    dst[RIDX(j, k, dim)] = 0;
            	}
            	for (l = 0; l < dim; l++){
                    dst[RIDX(j, k, dim)] += b_mat[RIDX(i*dim+j, l, dim)] * mat[RIDX(l, k, dim)];
                }
            }
        }
    }
}

/*
 * batched_mm - Your current working version of batched matrix multiplication
 * IMPORTANT: This is the version you will be graded on
 */
char batched_mm_descr[] = "Batched MM with sum reduction: Current working version";
void batched_mm(int dim, int *b_mat, int *mat, int *dst)
{
    int i, j, k;
    int dimkare = dim * dim;

    int* mattranspose = (int *)calloc(dimkare, sizeof(int));
    int idim = 0;
    for(i = 0; i<dim; i++){
        int jdimplusi = i;
        for(j = 0; j<dim; j+=32){
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            mattranspose[idim] = mat[jdimplusi];
            idim++;
            jdimplusi += dim;
            
        }
    }

    int *myArray = (int *)calloc(dimkare, sizeof(int));

    int idimkare = 0;
    for (i = 0; i < dim; i+=1){
        for (j = 0; j < dimkare; j+=32){
            int index2 = idimkare + j;
            myArray[j] += b_mat[index2++];
            myArray[j + 1] += b_mat[index2++];
            myArray[j + 2] += b_mat[index2++];
            myArray[j + 3] += b_mat[index2++];
            myArray[j + 4] += b_mat[index2++];
            myArray[j + 5] += b_mat[index2++];
            myArray[j + 6] += b_mat[index2++];
            myArray[j + 7] += b_mat[index2++];
            myArray[j + 8] += b_mat[index2++];
            myArray[j + 9] += b_mat[index2++];
            myArray[j + 10] += b_mat[index2++];
            myArray[j + 11] += b_mat[index2++];
            myArray[j + 12] += b_mat[index2++];
            myArray[j + 13] += b_mat[index2++];
            myArray[j + 14] += b_mat[index2++];
            myArray[j + 15] += b_mat[index2++];
            myArray[j + 16] += b_mat[index2++];
            myArray[j + 17] += b_mat[index2++];
            myArray[j + 18] += b_mat[index2++];
            myArray[j + 19] += b_mat[index2++];
            myArray[j + 20] += b_mat[index2++];
            myArray[j + 21] += b_mat[index2++];
            myArray[j + 22] += b_mat[index2++];
            myArray[j + 23] += b_mat[index2++];
            myArray[j + 24] += b_mat[index2++];
            myArray[j + 25] += b_mat[index2++];
            myArray[j + 26] += b_mat[index2++];
            myArray[j + 27] += b_mat[index2++];
            myArray[j + 28] += b_mat[index2++];
            myArray[j + 29] += b_mat[index2++];
            myArray[j + 30] += b_mat[index2++];
            myArray[j + 31] += b_mat[index2];
        }
        idimkare += dimkare;
    }

    idim = 0;
    int jdim = 0;
    for(i=0; i<dim; i++){
        jdim = 0;
        for(j=0; j<dim; j++){
            int sum = 0;
            
            for(k=0; k<dim; k+=32){
                int index1 = idim + k;
                int index2 = jdim + k;
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1++] * mattranspose[index2++];
                sum += myArray[index1] * mattranspose[index2];

            }
            jdim += dim;
            dst[idim + j] = sum;
        }
        idim += dim;
    }
}

/*********************************************************************
 * register_batched_mm_functions - Register all of your different versions
 *     of the batched matrix multiplication functions  with the driver by calling the
 *     add_batched_mm_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_batched_mm_functions() {
    add_batched_mm_function(&naive_batched_mm, naive_batched_mm_descr);
    add_batched_mm_function(&batched_mm, batched_mm_descr);
    /* ... Register additional test functions here */
}




/************************
 * POINT REFLECTION KERNEL
 ************************/

/*********************************************************
 * Your different versions of the point reflection go here
 *********************************************************/

/*
 * naive_reflect - The naive baseline version of point reflection
 */
char naive_reflect_descr[] = "Naive Point Reflection: Naive baseline implementation";
void naive_reflect(int dim, int *src, int *dst) {
    int i, j;

    for (i = 0; i < dim; i++) {
	for (j = 0; j < dim; j++) {
	    dst[RIDX(dim-1-i, dim-1-j, dim)] = src[RIDX(i, j, dim)];
	}
    }
}


/*
 * reflect - Your current working version of reflect
 * IMPORTANT: This is the version you will be graded on
 */
char reflect_descr[] = "Point Reflection: Current working version";
void reflect(int dim, int *src, int *dst)
{
	int j;

    int dimkare = dim * dim;
    for (j = 0; j < dim * dim; j+=64) {
        dst[dimkare - j - 1] = src[j];
        dst[dimkare - j - 2] = src[j + 1];
        dst[dimkare - j - 3] = src[j + 2];
        dst[dimkare - j - 4] = src[j + 3];
        dst[dimkare - j - 5] = src[j + 4];
        dst[dimkare - j - 6] = src[j + 5];
        dst[dimkare - j - 7] = src[j + 6];
        dst[dimkare - j - 8] = src[j + 7];
        dst[dimkare - j - 9] = src[j + 8];
        dst[dimkare - j - 10] = src[j + 9];
        dst[dimkare - j - 11] = src[j + 10];
        dst[dimkare - j - 12] = src[j + 11];
        dst[dimkare - j - 13] = src[j + 12];
        dst[dimkare - j - 14] = src[j + 13];
        dst[dimkare - j - 15] = src[j + 14];
        dst[dimkare - j - 16] = src[j + 15];
        dst[dimkare - j - 17] = src[j + 16];
        dst[dimkare - j - 18] = src[j + 17];
        dst[dimkare - j - 19] = src[j + 18];
        dst[dimkare - j - 20] = src[j + 19];
        dst[dimkare - j - 21] = src[j + 20];
        dst[dimkare - j - 22] = src[j + 21];
        dst[dimkare - j - 23] = src[j + 22];
        dst[dimkare - j - 24] = src[j + 23];
        dst[dimkare - j - 25] = src[j + 24];
        dst[dimkare - j - 26] = src[j + 25];
        dst[dimkare - j - 27] = src[j + 26];
        dst[dimkare - j - 28] = src[j + 27];
        dst[dimkare - j - 29] = src[j + 28];
        dst[dimkare - j - 30] = src[j + 29];
        dst[dimkare - j - 31] = src[j + 30];
        dst[dimkare - j - 32] = src[j + 31];
        dst[dimkare - j - 33] = src[j + 32];
        dst[dimkare - j - 34] = src[j + 33];
        dst[dimkare - j - 35] = src[j + 34];
        dst[dimkare - j - 36] = src[j + 35];
        dst[dimkare - j - 37] = src[j + 36];
        dst[dimkare - j - 38] = src[j + 37];
        dst[dimkare - j - 39] = src[j + 38];
        dst[dimkare - j - 40] = src[j + 39];
        dst[dimkare - j - 41] = src[j + 40];
        dst[dimkare - j - 42] = src[j + 41];
        dst[dimkare - j - 43] = src[j + 42];
        dst[dimkare - j - 44] = src[j + 43];
        dst[dimkare - j - 45] = src[j + 44];
        dst[dimkare - j - 46] = src[j + 45];
        dst[dimkare - j - 47] = src[j + 46];
        dst[dimkare - j - 48] = src[j + 47];
        dst[dimkare - j - 49] = src[j + 48];
        dst[dimkare - j - 50] = src[j + 49];
        dst[dimkare - j - 51] = src[j + 50];
        dst[dimkare - j - 52] = src[j + 51];
        dst[dimkare - j - 53] = src[j + 52];
        dst[dimkare - j - 54] = src[j + 53];
        dst[dimkare - j - 55] = src[j + 54];
        dst[dimkare - j - 56] = src[j + 55];
        dst[dimkare - j - 57] = src[j + 56];
        dst[dimkare - j - 58] = src[j + 57];
        dst[dimkare - j - 59] = src[j + 58];
        dst[dimkare - j - 60] = src[j + 59];
        dst[dimkare - j - 61] = src[j + 60];
        dst[dimkare - j - 62] = src[j + 61];
        dst[dimkare - j - 63] = src[j + 62];
        dst[dimkare - j - 64] = src[j + 63];
    }
}

/******************************************************************************
 * register_reflect_functions - Register all of your different versions
 *     of the reflect with the driver by calling the
 *     add_reflect_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 ******************************************************************************/

void register_reflect_functions() {
    add_reflect_function(&naive_reflect, naive_reflect_descr);
    add_reflect_function(&reflect, reflect_descr);
    /* ... Register additional test functions here */
}

