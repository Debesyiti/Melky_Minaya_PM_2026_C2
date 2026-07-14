#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MAX_ARCHIVOS 100

// revisa si el nombre del archivo termina en .png .jpg .jpeg o .bmp
int esImagen(char *nombre)
{
    char *punto = strrchr(nombre, '.');
    if (punto == NULL)
        return 0;

    if (strcasecmp(punto, ".png") == 0) return 1;

    if (strcasecmp(punto, ".jpg") == 0) return 1;

    if (strcasecmp(punto, ".jpeg") == 0) return 1;


    if (strcasecmp(punto, ".bmp") == 0) return 1;

    return 0;
}

int main()
{
    char carpeta[200];
    char lista[MAX_ARCHIVOS][300];
    int total = 0;

    printf("Carpeta a revisar (. para la carpeta actual): ");
    scanf("%s", carpeta);

    DIR *d = opendir(carpeta);
    if (d == NULL)
    {
        printf("No se pudo abrir la carpeta\n");
        return 1;
    }

    struct dirent *archivo;
    printf("\nImagenes encontradas:\n");
    while ((archivo = readdir(d)) != NULL)
    {
        if (esImagen(archivo->d_name))
        {
            strcpy(lista[total], archivo->d_name);
            printf("%d. %s\n", total + 1, lista[total]);
            total++;
        }
    }
    closedir(d);

    if (total == 0)

    {
        printf("No hay imagenes png, jpg o bmp en esa carpeta\n");
        return 0;
    }

    int opcion;
    printf("\nSeleccione el numero de la imagen a convertir: ");
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > total)
    {
        printf("Opcion invalida\n");
        return 1;
    }

    char rutaCompleta[520];
    sprintf(rutaCompleta, "%s/%s", carpeta, lista[opcion - 1]);

    int ancho, alto, canales;
    unsigned char *img = stbi_load(rutaCompleta, &ancho, &alto, &canales, 0);
    if (img == NULL)
    {
        printf("No se pudo cargar la imagen\n");
        return 1;
    }

    // pasar cada pixel a blanco y negro con la formula de luminosidad
    for (int i = 0; i < ancho * alto; i++)
    {
        unsigned char *pixel = img + i * canales;
        unsigned char gris = (unsigned char)(0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2]);
        pixel[0] = gris;
        pixel[1] = gris;
        pixel[2] = gris;
        // si tiene canal alpha (canales==4) se deja igual, no se toca
    }

    // armar el nombre de salida: nombre_bn.ext
    char nombreSalida[470];

    char base[320];
    strcpy(base, lista[opcion - 1]);

    char *punto = strrchr(base, '.');
    char ext[10];
    strcpy(ext, punto);
    *punto = '\0';

    sprintf(nombreSalida, "%s/%s_bn%s", carpeta, base, ext);

    int ok = 0;
    if (strcasecmp(ext, ".png") == 0)
        ok = stbi_write_png(nombreSalida, ancho, alto, canales, img, ancho * canales);

    else if (strcasecmp(ext, ".bmp") == 0)
        ok = stbi_write_bmp(nombreSalida, ancho, alto, canales, img);

        else // jpg o jpeg
            ok = stbi_write_jpg(nombreSalida, ancho, alto, canales, img, 90);

    if (ok)
        printf("\nListo, se guardo la imagen en blanco y negro como: %s\n", nombreSalida);

        else
            printf("\nHubo un error al guardar la imagen\n");

    stbi_image_free(img);

    return 0;
}
