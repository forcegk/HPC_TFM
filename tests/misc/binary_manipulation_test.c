#include <stdio.h>
#include "common.h"

int main (int argc, char *argv[]) {
    fp32 a;
    a.raw = 0xbedce196;

    printf("\n\n%x\n", a.raw);
    printf("%f\n", a.val);
    printf("%hhx  %hhx  %hhx  %hhx\n", a.byte[0], a.byte[1], a.byte[2], a.byte[3]);

    a.byte[3] = 0xbf;

    printf("\n\n%x\n", a.raw);
    printf("%f\n", a.val);
    printf("%hhx  %hhx  %hhx  %hhx\n", a.byte[0], a.byte[1], a.byte[2], a.byte[3]);

//// DEPRECATED: Using generics is not worth it when we can hard code it from python.
////            however, I am bringing it back to this specific test, cuz why not?
/** FAST SIGMOID **/
#define fast_sigmoid(X) _Generic((X), \
fp32:fast_sigmoid__fp32, \
fp64:fast_sigmoid__fp64 \
)(X)

    fp32 b = fast_sigmoid(a);
    printf("\n\n%x\n", b.raw);
    printf("%f\n", b.val);
    return 0;
}
