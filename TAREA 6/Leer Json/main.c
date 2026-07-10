#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Lector JSON. El programa lee un archivo JSON de alumnos, cuenta los elementos, crea un arreglo con memoria dinamica y los imprime. Se asume que cada alumno esta en una linea del archivo. */

typedef struct    /* Declaracion de la estructura alumno. */
{
    char nombre[30];
    char apellido[30];
    float promedio;
    char materia[30];
} alumno;

int cuenta(FILE *);             /* Prototipo de funcion. Cuenta los alumnos. */
void extrae_cadena(char *, char *, char *);
void extrae_numero(char *, char *, float *);
/* Prototipos de funciones. Extraen los valores de los campos en JSON. */

void main (void)
{
    int n, i;
    alumno *alu;    /* Apuntador para el arreglo dinamico. */
    char linea[300];
    FILE *ar;

    if ((ar = fopen("alumnos.json", "r")) != NULL)
    {
        n = cuenta(ar);         /* Primera pasada: se cuentan los alumnos del archivo. */
        printf("\nSe encontraron %d alumnos en el archivo JSON\n", n);

        alu = (alumno *) malloc(n * sizeof(alumno));
        /* Se reserva memoria dinamica para n estructuras alumno. */

        if (alu != NULL)
        {
            rewind(ar);         /* Se regresa el apuntador al inicio del archivo. */
            i = 0;
            while (!feof(ar) && i < n)
            {
                fgets(linea, 300, ar);
                if (strstr(linea, "\"nombre\"") != NULL)
                /* Si la linea contiene el campo nombre, se procesa como un alumno. */
                {
                    extrae_cadena (linea, "\"nombre\":", alu[i].nombre);
                    extrae_cadena (linea, "\"apellido\":", alu[i].apellido);

                    extrae_numero (linea, "\"promedio\":", &alu[i].promedio);
                    extrae_cadena (linea, "\"materia\":", alu[i].materia);
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

            free(alu);       /* Se libera la memoria dinamica que se habia reservado. */
        }
        else
            printf ("\nNo se pudo reservar memoria dinamica");
    }
    else
        printf("No se puede abrir el archivo");
}

int cuenta(FILE *ap)
/* Esta funcion cuenta el numero de lineas que contienen la etiqueta nombre. */
{
    int con = 0;
    char linea[300];
    while (fgets(linea, 300, ap) != NULL)   /* Se lee mientras fgets tenga exito. */
    {
        if (strstr(linea, "\"nombre\"") != NULL)
            con++;
    }
    return (con);
}

void extrae_cadena(char *linea, char *clave, char *destino)
/* Esta funcion busca la clave dentro de la linea y copia la cadena de caracteres que esta entre las comillas del valor. */
{
    int i, j = 0;
    char *pos;
    char aux[100];
    pos = strstr(linea, clave);         /* Se localiza la posicion de la clave en la linea. */
    if (pos != NULL)
    {
        i = strlen(clave);      /* Se salta la clave. */
        while (pos[i] != '\"' && pos[i] != '\0')    /* Se busca la primera comilla del valor. */
            i++;
        if (pos[i] == '\"')
            i++;
        while (pos[i] != '\"' && pos[i] != '\0')    /* Se copian los caracteres hasta la comilla de cierre. */
        {
            aux[j] = pos[i];
            i++;
            j++;
        }
        aux[j] = '\0';
        strcpy(destino, aux);
    }
}

void extrae_numero(char *linea, char *clave, float *destino)
/* Esta funcion busca la clave dentro de la linea y convierte a real el valor numerico que esta despues de los dos puntos. */
{
    int i, j = 0;
    char *pos;
    char aux[30];
    pos = strstr(linea, clave);
    if (pos != NULL)
    {
        i = strlen(clave);
        while (pos[i] == ' ')    /* Se saltan los espacios si los hubiera. */
        while (pos[i] == ' ')    /* Se saltan los espacios si los hubiera. */
            i++;
        while ((pos[i] >= '0' && pos[i] <= '9') || pos[i] == '.' || pos[i] == '-')
        /* Se copian los caracteres que forman parte del numero: digitos, punto y signo. */
        {
            aux[j] = pos[i];
            i++;
            j++;
        }
        aux[j] = '\0';
        *destino = atof(aux);
        /* Se convierte la cadena a valor real usando la funcion atof. */
    }
}
