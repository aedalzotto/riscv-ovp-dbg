/*
 *
 * Copyright (c) 2005-2019 Imperas Software Ltd., www.imperas.com
 *
 * The contents of this file are provided under the Software License
 * Agreement that you accepted before downloading this file.
 *
 * This source forms part of the Software and can be used for educational,
 * training, and demonstration purposes but cannot be used for derivative
 * works except in cases where the derivative works require OVP technology
 * to run.
 *
 * For open source models released under licenses that you can use for
 * derivative works, please visit www.OVPworld.org or www.imperas.com
 * for the location of the open source models.
 *
 */

#include <stdio.h>
#define NUM 3
#define SHAREDMEM 0x00
// Buffer in shared memory area
volatile unsigned *buffer = (volatile unsigned *) (SHAREDMEM + NUM*16*4);

// Calculate Fibonnacci series
unsigned fib(unsigned i) {
    return (i>1) ? fib(i-1) + fib(i-2) : i;
}

int main () {
    unsigned i, b, v;
    printf("app%u starting\n", NUM);
    for ( i = 0 ; i < 50000 ; i++ ) {
        // Do some processing
        v = fib(10);

        // Write to buffer
        for ( b = 0 ; b < 16 ; b++ )
            buffer[b] = v + NUM;

        // Check values in buffer
        for ( b = 0 ; b < 16 ; b++ ) {
            if ( buffer[b] != v + NUM ) {
                printf("app%u: buffer[%u] corrupt\n", NUM, b);
                return 1;
            }
        }
    }
    printf("app%u complete\n", NUM);
    return 0;
}
