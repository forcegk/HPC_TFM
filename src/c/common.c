#include "common.h"

/** FAST SIGMOID **/
fp32 fast_sigmoid__fp32(fp32 x){
    fp32 temp;
    temp.val = 0.5f * ( x.val / (1.f + fabsf(x.val)) + 1.f );
    return temp;
}

fp64 fast_sigmoid__fp64(fp64 x){
    fp64 temp;
    temp.val = 0.5 * ( x.val / (1. + fabs(x.val)) + 1.);
    return temp;
}

/** ACCURATE SIGMOID **/
fp32 sigmoid__fp32(fp32 x){
    fp32 temp;
    temp.val = 1.f / ( 1.f + expf(-x.val) );
    return temp;
}

fp64 sigmoid__fp64(fp64 x){
    fp64 temp;
    temp.val = 1. / ( 1. + exp(-x.val) );
    return temp;
}

/** MAP AND BIAS **/
void map_and_bias__fp32(fp32 *restrict A, const fp32 *restrict bias, const uint32_t M, const uint32_t N, const char transA, fp32 (* map_function)(fp32 x)){
    switch (transA){
    case 'N':
    case 'n':
        for (uint32_t i = 0; i < M; i++){
            for(uint32_t j = 0; j < N; j++){
                A[i*N+j].val = map_function((fp32)(A[i*N+j].val + bias[j].val)).val;
            }
        }
        break;

    case 'T':
    case 't':
        for (uint32_t i = 0; i < M; i++){
            for(uint32_t j = 0; j < N; j++){
                A[i*N+j].val = map_function((fp32)(A[i*N+j].val + bias[i].val)).val;
            }                                                     // ^^^^^
        }                                           // here we change traversing order
        break;
    }
}

void map_and_bias__fp64(fp64 *restrict A, const fp64 *restrict bias, const uint32_t M, const uint32_t N, const char transA, fp64 (* map_function)(fp64 x)){
    switch (transA){
    case 'N':
    case 'n':
        for (uint32_t i = 0; i < M; i++){
            for(uint32_t j = 0; j < N; j++){
                A[i*N+j].val = map_function((fp64)(A[i*N+j].val + bias[j].val)).val;
            }
        }
        break;

    case 'T':
    case 't':
        for (uint32_t i = 0; i < M; i++){
            for(uint32_t j = 0; j < N; j++){
                A[i*N+j].val = map_function((fp64)(A[i*N+j].val + bias[i].val)).val;
            }                                                     // ^^^^^
        }                                           // here we change traversing order
        break;
    }
}
/** END M&B **/





/************************ FOR DELETION ZONE ************************/

/**
 * @deprecated ONLY FOR TESTING
 */
void inefficient_matmul(const unsigned int nrowsA, const unsigned int mcolsA,
            const unsigned int pcolsB, const fp32 arrayA[],
            const fp32 arrayB[], fp32 arrayR[]){
  unsigned int i, j, k;

  for(i = 0; i < nrowsA; i++){
    for(j = 0; j < pcolsB; j++){
        arrayR[i*pcolsB + j].val = 0.f;
        for(k = 0; k < mcolsA; k++){
            arrayR[i*pcolsB + j].val += arrayA[i*mcolsA+k].val * arrayB[k*pcolsB+j].val;
        }
    }
  }
}