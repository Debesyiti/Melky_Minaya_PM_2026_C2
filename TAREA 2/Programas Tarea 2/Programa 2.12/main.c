#include <stdio.h>
#include <math.h>
/* Igualdad de expresiones. El programa, al recibir como datos T, P y N, comprueba la igualdad de una expresion determinada.
T, P y N: Son variables de tipo entero */

void main(void)
{
    int T, P, N;
    printf("Ingrese los valores de T, P y N: ");
    scanf("%d %d %d", &T, &P, &N);

    if (P != 0)
    {
        if (pow(T / P, N) == (pow (T, N) / pow (P, N)))
            printf("\nSe comprueba la igualdad");

            else printf("\nNo se comprueba la igualdad");
    }
    else printf("\nP tiene que ser diferente de cero");
}

/* Hay un error en el .ibro con el parentesis en la linea 14 maestro, si me guio del libro y uso el mismo numero de parentesis no funciona pq faltaria cerrar 1 */
