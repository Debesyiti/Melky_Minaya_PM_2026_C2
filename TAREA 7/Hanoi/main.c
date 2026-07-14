#include <stdio.h>
#include <math.h>

int mov = 1;
FILE *archivo;

void Hanoi(int n, char ini, char fin, char ayu)
{
    if (n == 1)
    {
        printf("Movimiento %d: mover disco 1 de %c a %c\n", mov, ini, fin);
        fprintf(archivo, "Movimiento %d: mover disco 1 de %c a %c\n", mov, ini, fin);
        mov++;
        return;
    }

    Hanoi(n - 1, ini, ayu, fin);

    printf("Movimiento %d: mover disco %d de %c a %c\n", mov, n, ini, fin);
    fprintf(archivo, "Movimiento %d: mover disco %d de %c a %c\n", mov, n, ini, fin);
    mov++;

    Hanoi(n - 1, ayu, fin, ini);
}

int main()
{
    int dis;

    do
    {
        printf("Cuantos discos quiere mover?: ");
        scanf("%d", &dis);
        if (dis <= 0)
        {
            printf("Error, el numero de discos tiene que ser mayor que cero\n");
        }
    }
    while (dis <= 0);

    archivo = fopen("movimientos.txt", "w");
    if (archivo == NULL)
    {
        printf("No se pudo crear el archivo\n");
        return 1;
    }

    printf("\nMovimientos:\n");
    Hanoi(dis, 'A', 'C', 'B');

    int total = pow(2, dis) - 1;
    printf("\nTotal de movimientos: %d\n", total);
    printf("Los movimientos quedaron guardados en movimientos.txt\n");

    fclose(archivo);
    return 0;
}
