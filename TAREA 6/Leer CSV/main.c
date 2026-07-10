#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Lector CSV. El programa lee un archivo CSV de alumnos, cuenta los elementos, crea un arreglo con memoria dinamica y los imprime. Se asume que la primera linea del archivo contiene los nombres de las columnas y cada campo esta separado por comas. */

typedef struct      /* Declaracion de la estructura alumno. */
{
    char nombre[30];
    char apellido[30];
    float promedio;
    char materia[30];
} alumno;

int cuenta(FILE *);      /* Prototipo de funcion. Cuenta los alumnos. */
void separa (char *, char *, char *, char *, char *);
/* Prototipo de funcion. Separa una linea CSV en sus cuatro campos. */

void main(void)
{
    int n, i;
    alumno *alu;     /* Apuntador para el arreglo dinamico. */
    char linea[200], campo3[30];
    FILE *ar;

    if ((ar = fopen("alumnos.csv", "r")) != NULL)
    {
        n = cuenta(ar);
         /* Primera pasada: se cuentan los alumnos del archivo, sin contar el encabezado. */

        printf("\nSe encontraron %d alumnos en el archivo CSV\n", n);

        alu = (alumno *) malloc(n * sizeof(alumno));
        /* Se reserva memoria dinamica para n estructuras alumno. */

        if (alu != NULL)
        {
            rewind(ar);   /* Se regresa el apuntador al inicio del archivo. */
            fgets(linea, 200, ar);  /* Se descarta la primera linea porque es el encabezado. */
            i = 0;
            while (!feof(ar) && i < n)
            {
                fgets(linea, 200, ar);
                if (strlen(linea) > 5)  /* Se procesan solo lineas con contenido real. */
                {
                    separa(linea, alu[i].nombre, alu[i].apellido, campo3, alu[i].materia);
                    alu[i].promedio = atof(campo3);
                    /* La funcion atof convierte la cadena del tercer campo a un valor real. */
                    i++;
                }
            }
            fclose(ar);

            printf("\nDatos de los alumnos:\n");
            for (i = 0; i < n; i++)
            {
                printf("\nAlumno %d:", i+1);
                printf("\n  Nombre:   %s", alu[i].nombre);
                printf("\n  Apellido: %s", alu[i].apellido);
                printf("\n  Promedio: %.2f", alu[i].promedio);
                printf("\n  Materia:  %s\n", alu[i].materia);
            }

            free(alu);   /* Se libera la memoria dinamica que se habia reservado. */
        }
        else
            printf("\nNo se pudo reservar memoria dinamica");
    }
    else
        printf("No se puede abrir el archivo");
}

int cuenta(FILE *ap)
/* Esta funcion cuenta el numero de lineas del archivo sin considerar el encabezado. */
{
    int con = 0;
    char linea[200];
    fgets(linea, 200, ap);    /* Se descarta la primera linea (encabezado). */
    while (fgets(linea, 200, ap) != NULL)   /* Se lee mientras fgets tenga exito. */
    {
        if (strlen(linea) > 5)    /* Solo se cuentan lineas con contenido real. */
            con++;
    }
    return (con);
}
void separa (char *linea, char *c1, char *c2, char *c3, char *c4)
/* Esta funcion separa una linea CSV en sus cuatro campos usando la coma como separador. Los resultados se guardan en c1, c2, c3 y c4. */
{
    int i = 0, j = 0, campo = 0;
    char *destinos[4];
    destinos[0] = c1;
    destinos[1] = c2;
    destinos[2] = c3;
    destinos[3] = c4;
    while (linea[i] != '\0' && linea[i] != '\n' && campo < 4)
    {
        if (linea[i] == ',')          /* Al encontrar una coma se termina el campo actual y se pasa al siguiente. */
        {
            destinos[campo][j] = '\0';
            campo++;
            j = 0;
        }
        else
        {
            destinos[campo][j] = linea[i];
            j++;
        }
        i++;
    }
    destinos[campo][j] = '\0';     /* Se cierra el ultimo campo con el fin de cadena. */
}
