#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_VALUES 40
 
// ENDEREÇOS COMPARTILHADOS
#define RESULTADO (*(volatile unsigned int*)0x00020000)
#define FLAG      (*(volatile unsigned int*)0x00020004)


int fibo(int i) {
    return (i>1) ? fibo(i-1) + fibo(i-2) : i;
}


int main(int argc, char **argv) {

    static int resultadofib, i, j, id ;

    id = 0;

    printf("CPU %d starting fibonacci...\n", id);

    for(i=0; i<NUM_VALUES; i++)  {

        FLAG = (i==(NUM_VALUES-1)) ? 2 : 1;

        resultadofib = fibo(i);

        RESULTADO = resultadofib;

        printf("CPU %d: fib(%3d) = %d [flag:%d] \n", id, i, RESULTADO, FLAG);

        while(FLAG) {}

    }

    printf("terminei CPU %d fibonacci...\n",id);

    return 1;
}
