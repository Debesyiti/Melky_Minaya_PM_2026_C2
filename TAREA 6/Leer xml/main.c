#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Lector XML. El programa lee un archivo XML de alumnos, cuenta los elementos, crea un arreglo con memoria dinamica y los imprime. */

typedef struct    /* Declaracion de la estructura alumno. */
{
    char nombre[30];
    char apellido[30];
    float promedio;
    char materia[30];
} alumno;

int cuenta(FILE *);   /* Prototipo de funcion. Cuenta los alumnos. */
void extrae(char *, char *);  /* Prototipo de funcion. Extrae el contenido entre etiquetas. */

void main(void)
{
    int n, i;
    alumno *alu;  /* Apuntador para el arreglo dinamico. */
    char linea[200];
    FILE *ar;

    if ((ar = fopen("alumnos.xml", "r")) != NULL)
    {
        n = cuenta(ar);      /* Primera pasada: se cuentan los alumnos del archivo. */

        printf("\nSe encontraron %d alumnos en el archivo XML\n", n);

        alu = (alumno *) malloc(n * sizeof(alumno));
        /* Se reserva memoria dinamica para n estructuras alumno. */

        if (alu != NULL)
        {
            rewind(ar);         /* Se regresa el apuntador al inicio del archivo. */
            i = 0;
            while (!feof(ar) && i < n)
            {
                fgets(linea, 200, ar);
                if (strstr(linea, "<alumno>") != NULL)
                /* Cuando se detecta la etiqueta de apertura se leen las cuatro lineas siguientes con los datos del alumno. */
                {
                    fgets(linea, 200, ar);
                    extrae(linea, alu[i].nombre);
                    fgets(linea, 200, ar);
                    extrae(linea, alu[i].apellido);
                    fgets(linea, 200, ar);

                    extrae(linea, linea);    /* Se reutiliza la variable linea para el promedio. */
                    alu[i].promedio = atof(linea);
                    /* La funcion atof convierte la cadena de caracteres a un valor real. */
                    fgets(linea, 200, ar);
                    extrae(linea, alu[i].materia);
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

            free(alu);  /* Se libera la memoria dinamica que se habia reservado. */
        }
        else
            printf("\nNo se pudo reservar memoria dinamica");
    }
    else
        printf("No se puede abrir el archivo");
}

int cuenta(FILE *ap)
/* Esta funcion cuenta el numero de etiquetas <alumno> que hay en el archivo. */
{
    int con = 0;
    char linea[200];
    while (fgets(linea, 200, ap) != NULL)       /* Se lee mientras fgets tenga exito. */
    {
        if (strstr(linea, "<alumno>") != NULL)  /* Se busca la subcadena <alumno> en la linea. */
            con++;
    }
    return (con);
}

void extrae(char *linea, char *destino)
/* Esta funcion extrae el contenido que esta entre las etiquetas de apertura y de cierre. Recibe la linea completa y guarda el resultado en destino. */
{
    int i = 0, j = 0;
    char aux[100];
    while (linea[i] != '>' && linea[i] != '\0')     /* Se avanza hasta encontrar el > de la etiqueta de apertura. */
        i++;
    if (linea[i] == '>')
        i++;
    while (linea[i] != '<' && linea[i] != '\0')     /* Se copian los caracteres hasta encontrar el < de la etiqueta de cierre. */
    {
        aux[j] = linea[i];
        i++;
        j++;
    }
    aux[j] = '\0';
    strcpy(destino, aux);
}
