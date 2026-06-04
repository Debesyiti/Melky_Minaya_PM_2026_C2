#include <stdio.h>
#include <stdlib.h>
/* Cubo-3. El programa calcula el cubo de los 10 primeros numeros naturales con la ayuda de una función y utilizando parámetros por valor. */
int cubo (int);  /* Prototipo de funcion. El parametro es de tipo entero*/

void main (void)
{
    int I;
    for (I = 1; I <= 10; I++)
        printf("\nEl cubo de I es:%d", cubo(I));
        /* Llamada a la funcion cubo. El paso del parámetro es por valor. */
}
int cubo (int K) /* K es un parámetro por valor de tipo entero. */
/* La función calcula el cubo del parámetro K. */
{
    return (K*K*K);
}

