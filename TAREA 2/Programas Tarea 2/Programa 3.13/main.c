#include <stdio.h>
#include <stdlib.h>
/* Fibonacci. El programa calcula y escribe los primeros 50 numeros de Fibonacci.
I, PRI, SEG, SIG: Variables de tipo entero. */

void main(void)
{
    int I, PRI = 0, SEG = 1, SIG;
    printf("\t %d \t %d", PRI, SEG);
     for (I = 3; I <= 50; I++) /* aqui hay un detalle si lo pongo justo como esta en el libro me da error, pero si le quito el espacio entre el simbolo de menor qué, se ejecuta correctamente, lo dejare arreglado que funcione.*/
     {
         SIG = PRI + SEG;
         PRI = SEG;
         SEG = SIG;
         printf("\t %d", SIG);
     }
}
