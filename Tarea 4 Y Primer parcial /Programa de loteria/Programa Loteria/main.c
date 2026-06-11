#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Programa de jugadas de loteria.
   Los numeros van del 1 al 40.
   El premio es la cantidad de dinero jugada por 1000. mucha suelllteeee */

void main(void)
{
     int opcion, cantidad, i, ganador;
     int numeros[100];
     float dinero[100];
     float premio;

     cantidad = 0;
     srand(time(NULL)); do
    {
         printf("\n----- LOTERIA -----\n");
         printf("1. Jugar\n");
         printf("2. Elegir cantidad de jugadas\n");
         printf("3. Salir\n");
         printf("Elija una opcion: ");
         scanf("%d", &opcion);

         switch (opcion)
        {
            case 1:

                if (cantidad == 0)
                printf("\nPrimero debe elegir la cantidad de jugadas (opcion 2).\n"); else
                    {
                        for (i = 0; i < cantidad; i++)

                    {
                        printf("\nJugada %d\n", i + 1); do
                        {
                printf("Ingrese el numero (1 al 40): ");

                    scanf("%d", &numeros[i]);

                    if (numeros[i] < 1 || numeros[i] > 40)
                        printf("Numero invalido, intente de nuevo.\n");
                        }
                        while (numeros[i] < 1 || numeros[i] > 40);

                        printf("Ingrese el dinero jugado: ");
                        scanf("%f", &dinero[i]);
                    }

            ganador = rand() % 40 + 1;
            printf("\nEl numero ganador del sorteo es: %d\n", ganador);

                premio = 0;

            for (i = 0; i < cantidad; i++)
                    {
                         if (numeros[i] == ganador)
                        {
                        printf("La jugada %d gano! Premio: %.2f\n", i + 1, dinero[i] * 1000);
                    premio = premio + dinero[i] * 1000;
                        }

                        else
                            printf("La jugada %d no gano.\n", i + 1);
                    }
                    printf("\nPremio total ganado: %.2f\n", premio);
                }
                break;

                case 2:
                    do
                {
                     printf("\nIngrese la cantidad de jugadas: ");
                     scanf("%d", &cantidad);
                     if (cantidad < 1 || cantidad > 100)
                         printf("Cantidad invalida (1 a 100).\n");
                }
                while (cantidad < 1 || cantidad > 100); break;

                case 3:

                printf("\nGracias por jugar. Adios.\n"); break;

                default: printf("\nOpcion no valida.\n");
        }
    }
    while (opcion != 3);
}
