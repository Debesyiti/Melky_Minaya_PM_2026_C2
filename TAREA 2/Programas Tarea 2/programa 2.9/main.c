#include <stdio.h>
#include <math.h>

/* Expresion. El programa, al recibir como datos tres valores enteros, establece si los mismos satisfacen una expresion determinada.


R,T y Q: Son variables de tipo entero.
RES: ws variable de tipo real */


void main (void)
{
    float RES;
    int R, T, Q;
    printf("Ingrese los valores de R, T, Q:");
    scanf("%d %d %d", &R, &T, &Q);
    RES = pow(R, 4) - pow(T, 3) + 4 * pow (Q,2);
    if (RES < 820)

        printf("\nR = %d\tT = %d\t Q = %d", R, T, Q);



}
