#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

// lee los primeros bytes del archivo y dice que tipo es de verdad
// segun su contenido (no segun el nombre)
char *tipoReal(char *ruta)
{
    FILE *f = fopen(ruta, "rb");
    if (f == NULL)
        return "no se pudo abrir";

    unsigned char cabecera[8];
    int leidos = fread(cabecera, 1, 8, f);
    fclose(f);

    if (leidos < 4)
        return "desconocido";

    // PNG: 89 50 4E 47 0D 0A 1A 0A

    if (cabecera[0] == 0x89 && cabecera[1] == 0x50 && cabecera[2] == 0x4E && cabecera[3] == 0x47)
        return "PNG";

    // JPG: FF D8 FF

    if (cabecera[0] == 0xFF && cabecera[1] == 0xD8 && cabecera[2] == 0xFF)
        return "JPG";

    // BMP: 42 4D ("BM")

    if (cabecera[0] == 0x42 && cabecera[1] == 0x4D)
        return "BMP";

    return "desconocido";
}

// saca la extension del nombre del archivo y la deja en mayusculas
void extensionArchivo(char *nombre, char *destino)
{
    char *punto = strrchr(nombre, '.');
    if (punto == NULL)
    {
        strcpy(destino, "");
        return;
    }

    punto++; // saltar el punto
    int i = 0;
    while (punto[i] != '\0')
    {
        destino[i] = toupper(punto[i]);
        i++;
    }
    destino[i] = '\0';

    if (strcmp(destino, "JPEG") == 0)
        strcpy(destino, "JPG");
}

int main()
{
    char carpeta[200];
    printf("Carpeta a revisar (. para la carpeta actual): ");
    scanf("%s", carpeta);

    DIR *d = opendir(carpeta);
    if (d == NULL)
    {
        printf ("No se pudo abrir la carpeta\n");
        return 1;
    }

    struct dirent *archivo;
    printf ("\n%-25s %-12s %-12s %s\n", "Archivo", "Extension", "Contenido", "Resultado");
    printf ("______________________________________________\n");

    while ((archivo = readdir(d)) != NULL)
    {
        char ext[20];
        extensionArchivo(archivo->d_name, ext);

        // solo revisar png, jpg y bmp, el resto se ignora
        if (strcmp(ext, "PNG") != 0 && strcmp(ext, "JPG") != 0 && strcmp(ext, "BMP") != 0)
            continue;

        char ruta[400];
        sprintf(ruta, "%s/%s", carpeta, archivo->d_name);

        char *real = tipoReal(ruta);

        char resultado[20];
        if (strcmp(ext, real) == 0)
            strcpy(resultado, "VALIDO");

            else
            strcpy(resultado, "NO COINCIDE");

            printf("%-25s %-12s %-12s %s\n", archivo->d_name, ext, real, resultado);
    }

    closedir(d);
    return 0;
}
