#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <math.h>

typedef union _fp32 {
    uint32_t raw;
    float val;
    uint8_t byte[4];
} fp32;

typedef union _fp64 {
    uint64_t raw;
    double val;
    uint8_t byte[8];
} fp64;


/** FAST SIGMOID **/
// #define fast_sigmoid(X) _Generic((X), \
// fp32:fast_sigmoid__fp32, \
// fp64:fast_sigmoid__fp64 \
// )(X)

// #pragma inline fast_sigmoid__fp32, fast_sigmoid__fp64  // not working with gcc, incompatible with icc
fp32 fast_sigmoid__fp32(fp32);
fp64 fast_sigmoid__fp64(fp64);

// #pragma inline sigmoid__fp32, sigmoid__fp64
fp32 sigmoid__fp32(fp32);
fp64 sigmoid__fp64(fp64);

/** MAP AND BIAS **/
/**
 * @brief Mapping function for applying to sgemm result to include bias and transfer function.
 * 
 * @param A Input matrix of dimmensions MxN where bias addition and mapping is done in place.
 * @param bias Bias array. Must be of dimmensions 1xN.
 * @param M Number of rows of matrix A.
 * @param N Number of columns of matrix A.
 * @param transA Specifies if matrix A is transposed. Keep in mind that, if A matrix is transposed, M and N should swap places for this function to work as intended.
 * @param map_function Function returning an fp32, which will be applied like: A[i][j] = map_function(A[i][j]+bias).
 */
void map_and_bias__fp32(fp32 *restrict A, const fp32 *restrict bias, const uint32_t M, const uint32_t N, const char transA, fp32 (* map_function)(fp32 x));


void inefficient_matmul(const unsigned int nrowsA, const unsigned int mcolsA,
            const unsigned int pcolsB, const fp32 arrayA[],
            const fp32 arrayB[], fp32 arrayR[]);

#endif // __COMMON_H__