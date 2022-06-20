#include <stdio.h>
#include <stdlib.h>
#include <lapacke.h>
#include <cblas.h>

#include "common.h"

#define INPUT_SIZE 24
#define LAYER1_SIZE 5
#define LAYER2_SIZE 3
#define LAYER3_SIZE 1

#define ALGO_SIGMOID
// #define ALGO_FAST_SIGMOID

#ifdef ALGO_SIGMOID
    #define ALGO_FUNC sigmoid__fp32
    #define ALGO_STR "sigmoid__fp32"
#endif
#ifdef ALGO_FAST_SIGMOID
    #define ALGO_FUNC fast_sigmoid__fp32
    #define ALGO_STR "fast_sigmoid__fp32"
#endif

// #define __DEBUG_PRINTS__

#define FROM_TXT
// #define FROM_BIN

int main (int argc, char *argv[]) {

    if(argc < 2){
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        exit(-1);
    }

    FILE *inputfile;
    fprintf(stderr, "*** Opening %s as input ***\n", argv[1]);
    inputfile = fopen(argv[1], "r");
    // TODO check for error he'e

    // Parse input dims and allocate memory consquently
    int input_dim;
    fscanf(inputfile, "%d", &input_dim);
    
    fp32 * input;
    input = malloc(input_dim * INPUT_SIZE * sizeof(int));

    // Read input
    for(int i = 0; i < INPUT_SIZE * input_dim; i++){
        fscanf(inputfile, "%f,", &input[i].val);

        #ifdef __DEBUG_PRINTS__
        // Debug print
        printf("%2.0f ", input[i].val);

        // When finished reading line
        if(!((i+1) % INPUT_SIZE)){
            // Moar debug
            printf("\n");
        }
        #endif
    }

    // Allocate layer values
    fp32 * layer1_out     = malloc(input_dim * LAYER1_SIZE * sizeof(fp32));
    fp32 * layer1_bias    = malloc(LAYER1_SIZE * sizeof(fp32));
    fp32 * layer1_weights = malloc(INPUT_SIZE * LAYER1_SIZE * sizeof(fp32));

    fp32 * layer2_out     = malloc(input_dim * LAYER2_SIZE * sizeof(fp32));
    fp32 * layer2_bias    = malloc(LAYER2_SIZE * sizeof(fp32));
    fp32 * layer2_weights = malloc(LAYER1_SIZE * LAYER2_SIZE * sizeof(fp32));

    fp32 * layer3_out     = malloc(input_dim * LAYER3_SIZE * sizeof(fp32));
    fp32 * layer3_bias    = malloc(LAYER3_SIZE * sizeof(fp32));
    fp32 * layer3_weights = malloc(LAYER2_SIZE * LAYER3_SIZE * sizeof(fp32));

    fp32 * layer1_weights_hardcoded = (fp32 *) "\xa4\x0b\x24\xbf\xec\x67\x92\x3f\xf2\xe1\x46\x3d\x8b\x76\x1f\x3e\xc0\x4e\xc7\x3e\x05\x72\xa4\x3e\x3a\x7e\x76\xbd\x00\x71\x38\xbf\xc2\xe7\x2c\xbd\xcc\xe4\x84\xbe\xb7\xfb\x12\xbf\xf2\x67\x29\x3f\xdd\xdc\x57\xbe\xbf\x78\x52\xbe\x06\xfb\xf6\x3b\xac\x35\x62\xbe\x8c\x0e\xf1\xbb\xa9\x12\x59\xbd\x24\xef\xe6\xbe\xe5\x15\x55\x3e\x0c\x78\x7c\xbe\xb1\xc5\xa4\x3e\x0c\x31\x64\xbe\x67\xdf\xb5\x3e\xc4\x62\x26\xbd\x63\x9e\x7c\xba\x29\x51\xd5\x3e\x09\x48\x0a\x3e\x6f\x80\x52\x3c\x3e\x42\x81\xbe\xad\xc4\xb3\xbb\x64\x99\x99\x3d\x1c\x2e\x95\x3e\xb2\xdb\x09\x3f\x2b\x42\x89\x3e\xc0\xd1\xb3\x3e\xe8\xcd\x97\x3d\x70\x5f\x04\xbe\x26\x03\xaf\x3d\x67\x3c\x49\x3e\xe2\x9e\x9d\x3e\xcb\xaf\x1a\xbf\x38\x18\x33\xbe\xd7\xfe\x80\x3e\x21\xcf\xd9\xbd\xca\x86\xad\xbb\xc7\xda\x89\xbd\x1e\x0d\x07\x3e\xc5\x60\xdd\x3e\x51\xbb\x9f\xbe\x2f\x71\x12\xb8\x13\xec\xa6\xbe\x11\x83\x7d\x3e\x18\x5b\xab\xbd\x5b\x99\xba\xbe\xcb\x07\x1b\xbe\xa8\xc2\xda\xbe\x23\x72\xb1\x3e\xcc\xe1\x28\xbe\x58\x01\xed\xbe\xe6\xde\x80\xbc\x73\x97\x81\xbd\xf4\x53\x24\x3e\x40\x88\xb0\xbe\xc3\x55\xc0\x3e\x05\xe1\x01\xbf\x2e\xab\xd4\x3e\x8c\x8c\x04\x3e\x78\x25\x96\x3e\x12\xcc\x4b\xbe\x0d\x04\x2d\xbd\x70\x28\xf5\xbe\x2e\x79\xaf\x3e\x31\x59\x42\xbe\x52\x9d\x95\x3e\x24\x77\x78\xbd\x8a\x37\x46\xbe\x05\x00\xe5\x3d\x47\xee\xc0\x3e\x2f\xdf\xd8\xbd\x79\xe1\x09\xbf\x87\xe7\x95\x3e\x45\x97\xc5\xbe\x3e\x22\x2a\xbe\x0c\x79\xa2\xbe\x7c\xb3\xd9\x3e\x99\x23\x2f\xbc\x4e\x59\x5b\xbe\xe1\xc5\x4d\xbe\xf9\x0e\x6c\xbc\x59\xd6\x6d\x3e\x32\x48\x82\x3e\xdc\x08\xdf\xbe\x69\x1e\xc6\xbc\x30\xa0\xc2\x3e\x8a\x38\xf1\x3d\x8c\x71\x63\x3e\x52\xcf\xce\xbe\x72\x98\x67\xbe\xe4\xbd\x31\x3e\xed\x1d\xf2\xbe\x8d\x8e\xf2\x3e\x4f\xb4\xd3\xbd\x03\x6f\x88\xbe\x63\xf0\x49\xbe\x33\x07\xa4\xbe\xa6\x46\xdb\xbe\x50\xef\x45\x3e\xd6\x3a\xa0\x3e\xbb\x2a\x8d\x3e\x55\xa6\x90\xbe\xfe\x40\xb3\x3e\x7e\x97\x9e\xbe\x76\x0d\x98\xbe\x74\x42\x8e\x3e\xb4\x87\x96\x3e\x59\xe8\xa3\x3d\xcb\xfc\x08\x3e\x66\xec\xae\x3e\x6e\xd2\x75\xbe";

#ifdef FROM_TXT
    FILE *layersfile;
    layersfile = fopen("layers.txt", "r");

    /*********** LAYER 1 ***********/
    fprintf(stderr, "*** Parsing %s values ***\n", "LAYER1");
    // And read layer values (poc)
    for(int i = 0; i < LAYER1_SIZE * INPUT_SIZE; i++){
        fscanf(layersfile, "%x,", &layer1_weights[i].raw);
        // TODO if error exit blabla

        #ifdef __DEBUG_PRINTS__
        // Debug print
        printf("%10.6f  ", layer1_weights[i].val);

        // When finished reading line
        if(!((i+1) % LAYER1_SIZE)){
            // Moar debug
            printf("\n");
        }
        #endif
    }

    // Save to binary file
    FILE *layer1_file;
    layer1_file = fopen("layer1_file.bin", "w");
    fwrite(layer1_weights, INPUT_SIZE*LAYER1_SIZE*sizeof(fp32), 1, layer1_file);
    fclose(layer1_file);
#endif
#ifdef FROM_BIN
    // Save to binary file
    FILE *layer1_file;
    layer1_file = fopen("layer1_file.bin", "r");
    fread(layer1_weights, INPUT_SIZE*LAYER1_SIZE*sizeof(fp32), 1, layer1_file);
    fclose(layer1_file);
#endif

    fprintf(stderr, "*** Parsing %s bias ***\n", "LAYER1");
    for(int i = 0; i < LAYER1_SIZE; i++){
        fscanf(layersfile, "%x,", &layer1_bias[i].raw);
        // TODO if error exit blabla

        #ifdef __DEBUG_PRINTS__
        // Debug print
        printf("%10.6f  ", layer1_bias[i].val);

        // When finished reading line
        if(!((i+1) % LAYER1_SIZE)){
            // Moar debug
            printf("\n");
        }
        #endif
    }

    /*********** LAYER 2 ***********/
    fprintf(stderr, "*** Parsing %s values ***\n", "LAYER2");
    // And read layer values (poc)
    for(int i = 0; i < LAYER2_SIZE * LAYER1_SIZE; i++){
        fscanf(layersfile, "%x,", &layer2_weights[i].raw);
        // TODO if error exit blabla

        #ifdef __DEBUG_PRINTS__
        // Debug print
        printf("%10.6f  ", layer2_weights[i].val);

        // When finished reading line
        if(!((i+1) % LAYER2_SIZE)){
            // Moar debug
            printf("\n");
        }
        #endif
    }
    fprintf(stderr, "*** Parsing %s bias ***\n", "LAYER2");
    for(int i = 0; i < LAYER2_SIZE; i++){
        fscanf(layersfile, "%x,", &layer2_bias[i].raw);
        // TODO if error exit blabla

        #ifdef __DEBUG_PRINTS__
        // Debug print
        printf("%10.6f  ", layer2_bias[i].val);

        // When finished reading line
        if(!((i+1) % LAYER2_SIZE)){
            // Moar debug
            printf("\n");
        }
        #endif
    }

    /*********** LAYER 3 ***********/
    fprintf(stderr, "*** Parsing %s values ***\n", "LAYER3");
    // And read layer values (poc)
    for(int i = 0; i < LAYER3_SIZE * LAYER2_SIZE; i++){
        fscanf(layersfile, "%x,", &layer3_weights[i].raw);
        // TODO if error exit blabla

        #ifdef __DEBUG_PRINTS__
        // Debug print
        printf("%10.6f  ", layer3_weights[i].val);

        // When finished reading line
        if(!((i+1) % LAYER3_SIZE)){
            // Moar debug
            printf("\n");
        }
        #endif
    }
    fprintf(stderr, "*** Parsing %s bias ***\n", "LAYER3");
    for(int i = 0; i < LAYER3_SIZE; i++){
        fscanf(layersfile, "%x,", &layer3_bias[i].raw);
        // TODO if error exit blabla

        #ifdef __DEBUG_PRINTS__
        // Debug print
        printf("%10.6f  ", layer3_bias[i].val);

        // When finished reading line
        if(!((i+1) % LAYER3_SIZE)){
            // Moar debug
            printf("\n");
        }
        #endif
    }


    /*✨✨✨✨✨✨✨✨ BEGIN ALGEBRAIC MAGIC ✨✨✨✨✨✨✨✨*/
    // see cblas know shit is getting there 

    /**
     * Pequeño esquemita de como funciona esta función. Asumamos que tenemos multiplicación de:
     *          A [1000x24] x B [25x5]  ==>  C [1000x5]
     *             ^^^^ ^^       ^^ ^           ^^^^ ^
     *              M   K        K  N            M   N
     *                  lda        ldb              ldc
     * 
     *          AxB = C   ~~~~~>   C = beta*(AB*alpha)
     * 
     * https://developer.apple.com/documentation/accelerate/1513264-cblas_sgemm?language=objc
     * https://www.netlib.org/blas/
     */


    fprintf(stderr, "*** SGEMM %s x %s ***\n", "input", "layer1");
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                input_dim, LAYER1_SIZE, INPUT_SIZE,  1.f,
                (float *) input, INPUT_SIZE,
                (float *) layer1_weights, LAYER1_SIZE,
                1.f, (float *) layer1_out, LAYER1_SIZE);
    // inefficient_matmul(input_dim, INPUT_SIZE, LAYER1_SIZE, input, layer1_weights, layer1_out);


    #ifdef __DEBUG_PRINTS__
    printf("\n\n*** OUTPUT TRAS PRIMERA MULT ***\n");
    for(int i = 0; i < input_dim; i++){
        for(int j = 0; j < LAYER1_SIZE; j++){
            // Debug print
            printf("%10.6f  ", layer1_out[i*LAYER1_SIZE + j].val);
        }
        printf("\n");
    }
    #endif

    fprintf(stderr, "*** Map and Bias %s with %s function ***\n", "layer1_out", ALGO_STR);
    map_and_bias__fp32(layer1_out, layer1_bias, input_dim, LAYER1_SIZE, ALGO_FUNC);

    #ifdef __DEBUG_PRINTS__
    printf("\n\n*** OUTPUT TRAS MAP AND BIAS ***\n");
    for(int i = 0; i < input_dim; i++){
        for(int j = 0; j < LAYER1_SIZE; j++){
            // Debug print
            printf("%10.6f  ", layer1_out[i*LAYER1_SIZE + j].val);
        }
        printf("\n");
    }
    #endif


    fprintf(stderr, "*** SGEMM %s x %s ***\n", "layer1", "layer2");
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                input_dim, LAYER2_SIZE, LAYER1_SIZE,  1.f,
                (float *) layer1_out, LAYER1_SIZE,
                (float *) layer2_weights, LAYER2_SIZE,
                1.f, (float *) layer2_out, LAYER2_SIZE);
    // inefficient_matmul(input_dim, LAYER1_SIZE, LAYER2_SIZE, layer1_out, layer2_weights, layer2_out);


    fprintf(stderr, "*** Map and Bias %s with %s function ***\n", "layer1_out", ALGO_STR);
    map_and_bias__fp32(layer2_out, layer2_bias, input_dim, LAYER2_SIZE, ALGO_FUNC);


    fprintf(stderr, "*** SGEMM %s x %s ***\n", "layer2", "layer3");
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                input_dim, LAYER3_SIZE, LAYER2_SIZE,  1.f,
                (float *) layer2_out, LAYER2_SIZE,
                (float *) layer3_weights, LAYER3_SIZE,
                1.f, (float *) layer3_out, LAYER3_SIZE);
    // inefficient_matmul(input_dim, LAYER2_SIZE, LAYER3_SIZE, layer2_out, layer3_weights, layer3_out);


    fprintf(stderr, "*** Map and Bias %s with %s function ***\n", "layer1_out", ALGO_STR);
    map_and_bias__fp32(layer3_out, layer3_bias, input_dim, LAYER3_SIZE, ALGO_FUNC);

    #ifdef __DEBUG_PRINTS__
    printf("\n\n*** OUTPUTS ***\n");
    for(int i = 0; i < input_dim; i++){
        // Debug print
        printf("%2.5f\n", layer3_out[i].val);
    }
    #endif

    unsigned int greater_count = 0;
    for(int i = 0; i < input_dim; i++){
        if(layer3_out[i].val >= 0.5f) greater_count += 1;
    }

    fprintf(stderr, "\n\n");
    printf("--- SUMMARY ---\n");
    printf(" - Batch size = %d\n", input_dim);
    printf(" - Total >0.5 predictions = %d\n", greater_count);
    printf(" - %% >0.5 = %.2lf%%\n", ((double) greater_count / (double) input_dim)*100);
    printf("\n\n");

    /* welcome to the FREE zone */
    free(input);

    free(layer1_out);
    free(layer1_bias);
    free(layer1_weights);

    free(layer2_out);
    free(layer2_bias);
    free(layer2_weights);

    free(layer3_out);
    free(layer3_bias);
    free(layer3_weights);

    fclose(inputfile);
    fclose(layersfile);
    
    return 0;
}