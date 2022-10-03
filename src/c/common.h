#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <math.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

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

fp32 fast_sigmoid__fp32(fp32);
fp64 fast_sigmoid__fp64(fp64);

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
void map_and_bias__fp64(fp64 *restrict A, const fp64 *restrict bias, const uint32_t M, const uint32_t N, const char transA, fp64 (* map_function)(fp64 x));


/**
 * @deprecated ONLY FOR TESTING
 */
void inefficient_matmul(const unsigned int nrowsA, const unsigned int mcolsA,
            const unsigned int pcolsB, const fp32 arrayA[],
            const fp32 arrayB[], fp32 arrayR[]);

#endif // __COMMON_H__