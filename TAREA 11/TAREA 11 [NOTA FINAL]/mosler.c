/* ============================================================
   ANALISIS DE RIESGOS POR EL METODO MOSLER
   ------------------------------------------------------------
   El metodo Mosler permite identificar y clasificar la gravedad
   de un riesgo dentro de una empresa. Se apoya en 6 criterios,
   cada uno calificado del 1 al 5:

     F = Funcion       : dano que causa a la actividad de la empresa
     S = Sustitucion   : dificultad para reponer los bienes danados
     P = Profundidad   : perturbacion / dano a la imagen
     E = Extension     : alcance geografico del dano
     A = Agresion      : probabilidad de que el riesgo ocurra
     V = Vulnerabilidad: probabilidad de que cause dano real

   Con esos valores se calcula:
     I  = F * S     (Importancia del suceso)
     D  = P * E     (Danos ocasionados)
     C  = I + D     (Caracter del riesgo)
     Pb = A * V     (Probabilidad)
     ER = C * Pb    (Evaluacion del Riesgo)

   Clasificacion final segun ER (rango de 2 a 1250):
        2 -  250  Muy bajo
      251 -  500  Bajo
      501 -  750  Normal
      751 - 1000  Alto
     1001 - 1250  Muy alto

   Caracteristicas tecnicas del programa:
     - Memoria dinamica (malloc / realloc / free)
     - Guardado en TXT, CSV, JSON y XML
     - Seleccion y creacion de archivos especificos
     - Modulo de IA (sistema experto) para recomendaciones
     - Uso de la libreria externa cJSON

   Nombre: Melky Minaya
   Matricula: 2026-0327
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define CAPACIDAD_INICIAL 5
#define MAX_NOMBRE        80
#define MAX_DESC         200
#define MAX_AREA          60
#define MAX_RUTA         200

/* ------------------------------------------------------------
   ESTRUCTURA DEL RIESGO
   ------------------------------------------------------------ */
typedef struct {
    int  id;
    char nombre[MAX_NOMBRE];
    char descripcion[MAX_DESC];
    char area[MAX_AREA];

    /* Criterios Mosler (valores del 1 al 5) */
    int F;   /* Funcion        */
    int S;   /* Sustitucion    */
    int P;   /* Profundidad    */
    int E;   /* Extension      */
    int A;   /* Agresion       */
    int V;   /* Vulnerabilidad */

    /* Resultados calculados */
    int I;   /* Importancia = F * S */
    int D;   /* Danos       = P * E */
    int C;   /* Caracter    = I + D */
    int Pb;  /* Probabilidad= A * V */
    int ER;  /* Evaluacion  = C * Pb */
    char clasificacion[20];
} Riesgo;

/* ------------------------------------------------------------
   VARIABLES GLOBALES (memoria dinamica)
   ------------------------------------------------------------ */
Riesgo *riesgos    = NULL;   /* puntero al bloque de riesgos */
int     total      = 0;      /* cuantos riesgos hay          */
int     capacidad  = 0;      /* cuantos caben actualmente    */
char    archivoActual[MAX_RUTA] = "riesgos.txt";

/* ============================================================
   FUNCIONES DE ENTRADA SEGURA
   ============================================================ */

void quitarSalto(char *texto)
{
    size_t largo = strlen(texto);
    if (largo > 0 && texto[largo - 1] == '\n')
        texto[largo - 1] = '\0';
}

void leerTexto(const char *mensaje, char *destino, int tamano)
{
    printf("%s", mensaje);
    if (fgets(destino, tamano, stdin) != NULL)
        quitarSalto(destino);
}

int leerEntero(const char *mensaje)
{
    char linea[100];
    int  valor;

    while (1)
    {
        printf("%s", mensaje);
        if (fgets(linea, sizeof(linea), stdin) == NULL) return 0;
        if (sscanf(linea, "%d", &valor) == 1)
            return valor;
        printf("  >> Dato invalido. Escriba un numero entero.\n");
    }
}

/* Lee un criterio Mosler validando que este entre 1 y 5 */
int leerCriterio(const char *nombre, const char *ayuda)
{
    char linea[100];
    int  valor;

    printf("\n  %s\n", nombre);
    printf("  %s\n", ayuda);

    while (1)
    {
        printf("  Valor (1 a 5): ");
        if (fgets(linea, sizeof(linea), stdin) == NULL) return 1;
        if (sscanf(linea, "%d", &valor) == 1 && valor >= 1 && valor <= 5)
            return valor;
        printf("  >> Solo se permiten valores del 1 al 5.\n");
    }
}

/* ============================================================
   MEMORIA DINAMICA
   ============================================================ */

/* Reserva el bloque inicial de memoria */
void iniciarMemoria(void)
{
    capacidad = CAPACIDAD_INICIAL;
    riesgos   = (Riesgo *) malloc(capacidad * sizeof(Riesgo));

    if (riesgos == NULL)
    {
        printf("Error: no se pudo reservar memoria.\n");
        exit(1);
    }
    total = 0;
}

/* Si el bloque se llena, lo agranda al doble */
void asegurarEspacio(void)
{
    Riesgo *nuevo;

    if (total < capacidad) return;   /* todavia cabe */

    capacidad = capacidad * 2;
    nuevo = (Riesgo *) realloc(riesgos, capacidad * sizeof(Riesgo));

    if (nuevo == NULL)
    {
        printf("Error: no se pudo ampliar la memoria.\n");
        exit(1);
    }
    riesgos = nuevo;
    printf("  (memoria ampliada a %d riesgos)\n", capacidad);
}

/* Libera la memoria al terminar */
void liberarMemoria(void)
{
    if (riesgos != NULL)
    {
        free(riesgos);
        riesgos   = NULL;
        total     = 0;
        capacidad = 0;
    }
}

/* ============================================================
   CALCULOS DEL METODO MOSLER
   ============================================================ */

void clasificarRiesgo(Riesgo *r)
{
    if      (r->ER <= 250)  strcpy(r->clasificacion, "MUY BAJO");
    else if (r->ER <= 500)  strcpy(r->clasificacion, "BAJO");
    else if (r->ER <= 750)  strcpy(r->clasificacion, "NORMAL");
    else if (r->ER <= 1000) strcpy(r->clasificacion, "ALTO");
    else                    strcpy(r->clasificacion, "MUY ALTO");
}

void calcularRiesgo(Riesgo *r)
{
    r->I  = r->F * r->S;      /* Importancia del suceso */
    r->D  = r->P * r->E;      /* Danos ocasionados      */
    r->C  = r->I + r->D;      /* Caracter del riesgo    */
    r->Pb = r->A * r->V;      /* Probabilidad           */
    r->ER = r->C * r->Pb;     /* Evaluacion del riesgo  */
    clasificarRiesgo(r);
}

/* Recalcula todos los riesgos de la lista */
void calcularTodos(void)
{
    int i;
    for (i = 0; i < total; i++)
        calcularRiesgo(&riesgos[i]);
    printf("\nSe recalcularon %d riesgo(s) correctamente.\n", total);
}

int buscarPorId(int id)
{
    int i;
    for (i = 0; i < total; i++)
        if (riesgos[i].id == id)
            return i;
    return -1;
}

int generarId(void)
{
    int i, mayor = 0;
    for (i = 0; i < total; i++)
        if (riesgos[i].id > mayor)
            mayor = riesgos[i].id;
    return mayor + 1;
}

/* ============================================================
   GUARDAR EN LOS DISTINTOS FORMATOS
   ============================================================ */

/* Devuelve un puntero a la extension del archivo (".json", ".xml"...) */
const char *extensionDe(const char *ruta)
{
    const char *punto = strrchr(ruta, '.');
    return (punto == NULL) ? "" : punto;
}

/* ---------- TXT: texto plano legible ---------- */
int guardarTXT(const char *ruta)
{
    FILE *f = fopen(ruta, "w");
    int i;

    if (f == NULL) return 0;

    fprintf(f, "ANALISIS DE RIESGOS - METODO MOSLER\n");
    fprintf(f, "Total de riesgos: %d\n", total);
    fprintf(f, "=========================================================\n");

    for (i = 0; i < total; i++)
    {
        fprintf(f, "ID          : %d\n",   riesgos[i].id);
        fprintf(f, "NOMBRE      : %s\n",   riesgos[i].nombre);
        fprintf(f, "DESCRIPCION : %s\n",   riesgos[i].descripcion);
        fprintf(f, "AREA        : %s\n",   riesgos[i].area);
        fprintf(f, "F=%d S=%d P=%d E=%d A=%d V=%d\n",
                riesgos[i].F, riesgos[i].S, riesgos[i].P,
                riesgos[i].E, riesgos[i].A, riesgos[i].V);
        fprintf(f, "I=%d D=%d C=%d Pb=%d ER=%d\n",
                riesgos[i].I, riesgos[i].D, riesgos[i].C,
                riesgos[i].Pb, riesgos[i].ER);
        fprintf(f, "CLASIFICACION: %s\n", riesgos[i].clasificacion);
        fprintf(f, "---------------------------------------------------------\n");
    }

    fclose(f);
    return 1;
}

/* ---------- CSV: separado por comas ---------- */
int guardarCSV(const char *ruta)
{
    FILE *f = fopen(ruta, "w");
    int i;

    if (f == NULL) return 0;

    /* Linea de encabezados */
    fprintf(f, "id,nombre,descripcion,area,F,S,P,E,A,V,I,D,C,Pb,ER,clasificacion\n");

    for (i = 0; i < total; i++)
    {
        fprintf(f, "%d,\"%s\",\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
                riesgos[i].id, riesgos[i].nombre, riesgos[i].descripcion,
                riesgos[i].area,
                riesgos[i].F, riesgos[i].S, riesgos[i].P,
                riesgos[i].E, riesgos[i].A, riesgos[i].V,
                riesgos[i].I, riesgos[i].D, riesgos[i].C,
                riesgos[i].Pb, riesgos[i].ER, riesgos[i].clasificacion);
    }

    fclose(f);
    return 1;
}

/* ---------- JSON: usando la libreria cJSON ---------- */
int guardarJSON(const char *ruta)
{
    cJSON *raiz, *arreglo, *obj;
    char  *texto;
    FILE  *f;
    int    i;

    raiz    = cJSON_CreateObject();
    arreglo = cJSON_CreateArray();

    cJSON_AddStringToObject(raiz, "metodo", "Mosler");
    cJSON_AddNumberToObject(raiz, "total", total);

    for (i = 0; i < total; i++)
    {
        obj = cJSON_CreateObject();
        cJSON_AddNumberToObject(obj, "id",          riesgos[i].id);
        cJSON_AddStringToObject(obj, "nombre",      riesgos[i].nombre);
        cJSON_AddStringToObject(obj, "descripcion", riesgos[i].descripcion);
        cJSON_AddStringToObject(obj, "area",        riesgos[i].area);
        cJSON_AddNumberToObject(obj, "F",  riesgos[i].F);
        cJSON_AddNumberToObject(obj, "S",  riesgos[i].S);
        cJSON_AddNumberToObject(obj, "P",  riesgos[i].P);
        cJSON_AddNumberToObject(obj, "E",  riesgos[i].E);
        cJSON_AddNumberToObject(obj, "A",  riesgos[i].A);
        cJSON_AddNumberToObject(obj, "V",  riesgos[i].V);
        cJSON_AddNumberToObject(obj, "I",  riesgos[i].I);
        cJSON_AddNumberToObject(obj, "D",  riesgos[i].D);
        cJSON_AddNumberToObject(obj, "C",  riesgos[i].C);
        cJSON_AddNumberToObject(obj, "Pb", riesgos[i].Pb);
        cJSON_AddNumberToObject(obj, "ER", riesgos[i].ER);
        cJSON_AddStringToObject(obj, "clasificacion", riesgos[i].clasificacion);
        cJSON_AddItemToArray(arreglo, obj);
    }

    cJSON_AddItemToObject(raiz, "riesgos", arreglo);

    texto = cJSON_Print(raiz);          /* convierte el objeto a texto */
    f = fopen(ruta, "w");

    if (f == NULL)
    {
        cJSON_Delete(raiz);
        free(texto);
        return 0;
    }

    fprintf(f, "%s\n", texto);
    fclose(f);

    cJSON_Delete(raiz);   /* libera el arbol JSON */
    free(texto);          /* libera el texto generado */
    return 1;
}

/* ---------- XML ---------- */
int guardarXML(const char *ruta)
{
    FILE *f = fopen(ruta, "w");
    int i;

    if (f == NULL) return 0;

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<analisis metodo=\"Mosler\" total=\"%d\">\n", total);

    for (i = 0; i < total; i++)
    {
        fprintf(f, "  <riesgo id=\"%d\">\n", riesgos[i].id);
        fprintf(f, "    <nombre>%s</nombre>\n",           riesgos[i].nombre);
        fprintf(f, "    <descripcion>%s</descripcion>\n", riesgos[i].descripcion);
        fprintf(f, "    <area>%s</area>\n",               riesgos[i].area);
        fprintf(f, "    <criterios F=\"%d\" S=\"%d\" P=\"%d\" E=\"%d\" A=\"%d\" V=\"%d\"/>\n",
                riesgos[i].F, riesgos[i].S, riesgos[i].P,
                riesgos[i].E, riesgos[i].A, riesgos[i].V);
        fprintf(f, "    <resultados I=\"%d\" D=\"%d\" C=\"%d\" Pb=\"%d\" ER=\"%d\"/>\n",
                riesgos[i].I, riesgos[i].D, riesgos[i].C,
                riesgos[i].Pb, riesgos[i].ER);
        fprintf(f, "    <clasificacion>%s</clasificacion>\n", riesgos[i].clasificacion);
        fprintf(f, "  </riesgo>\n");
    }

    fprintf(f, "</analisis>\n");
    fclose(f);
    return 1;
}

/* Guarda usando el formato que indique la extension del archivo */
void guardarArchivo(const char *ruta)
{
    const char *ext = extensionDe(ruta);
    int ok = 0;

    if      (strcmp(ext, ".json") == 0) ok = guardarJSON(ruta);
    else if (strcmp(ext, ".xml")  == 0) ok = guardarXML(ruta);
    else if (strcmp(ext, ".csv")  == 0) ok = guardarCSV(ruta);
    else                                ok = guardarTXT(ruta);

    if (ok) printf("Datos guardados en: %s\n", ruta);
    else    printf("Error: no se pudo guardar en %s\n", ruta);
}

/* ============================================================
   CARGAR DESDE DISCO (con memoria dinamica)
   ============================================================ */

/* Carga desde un archivo JSON usando cJSON */
int cargarJSON(const char *ruta)
{
    FILE  *f;
    long   tamano;
    char  *contenido;
    cJSON *raiz, *arreglo, *item;
    int    i, cantidad;

    f = fopen(ruta, "r");
    if (f == NULL) return 0;

    /* Se mide el tamano del archivo para reservar memoria exacta */
    fseek(f, 0, SEEK_END);
    tamano = ftell(f);
    rewind(f);

    contenido = (char *) malloc(tamano + 1);
    if (contenido == NULL) { fclose(f); return 0; }

    fread(contenido, 1, tamano, f);
    contenido[tamano] = '\0';
    fclose(f);

    raiz = cJSON_Parse(contenido);
    free(contenido);

    if (raiz == NULL) return 0;

    arreglo = cJSON_GetObjectItem(raiz, "riesgos");
    if (arreglo == NULL) { cJSON_Delete(raiz); return 0; }

    cantidad = cJSON_GetArraySize(arreglo);
    total = 0;

    for (i = 0; i < cantidad; i++)
    {
        item = cJSON_GetArrayItem(arreglo, i);
        asegurarEspacio();

        riesgos[total].id = cJSON_GetObjectItem(item, "id")->valueint;
        strncpy(riesgos[total].nombre,
                cJSON_GetObjectItem(item, "nombre")->valuestring, MAX_NOMBRE - 1);
        strncpy(riesgos[total].descripcion,
                cJSON_GetObjectItem(item, "descripcion")->valuestring, MAX_DESC - 1);
        strncpy(riesgos[total].area,
                cJSON_GetObjectItem(item, "area")->valuestring, MAX_AREA - 1);

        riesgos[total].F = cJSON_GetObjectItem(item, "F")->valueint;
        riesgos[total].S = cJSON_GetObjectItem(item, "S")->valueint;
        riesgos[total].P = cJSON_GetObjectItem(item, "P")->valueint;
        riesgos[total].E = cJSON_GetObjectItem(item, "E")->valueint;
        riesgos[total].A = cJSON_GetObjectItem(item, "A")->valueint;
        riesgos[total].V = cJSON_GetObjectItem(item, "V")->valueint;

        calcularRiesgo(&riesgos[total]);
        total++;
    }

    cJSON_Delete(raiz);
    return 1;
}

/* Carga desde un archivo CSV */
int cargarCSV(const char *ruta)
{
    FILE *f = fopen(ruta, "r");
    char linea[600];

    if (f == NULL) return 0;

    /* Se salta la linea de encabezados */
    if (fgets(linea, sizeof(linea), f) == NULL) { fclose(f); return 0; }

    total = 0;

    while (fgets(linea, sizeof(linea), f) != NULL)
    {
        Riesgo r;
        int leidos;

        leidos = sscanf(linea,
            "%d,\"%79[^\"]\",\"%199[^\"]\",\"%59[^\"]\",%d,%d,%d,%d,%d,%d",
            &r.id, r.nombre, r.descripcion, r.area,
            &r.F, &r.S, &r.P, &r.E, &r.A, &r.V);

        if (leidos == 10)
        {
            asegurarEspacio();
            calcularRiesgo(&r);
            riesgos[total] = r;
            total++;
        }
    }

    fclose(f);
    return 1;
}

/* Carga desde el archivo de texto plano propio */
int cargarTXT(const char *ruta)
{
    FILE *f = fopen(ruta, "r");
    char linea[400];
    Riesgo r;
    int leyendo = 0;

    if (f == NULL) return 0;

    total = 0;
    memset(&r, 0, sizeof(Riesgo));

    while (fgets(linea, sizeof(linea), f) != NULL)
    {
        quitarSalto(linea);

        if (strncmp(linea, "ID          : ", 14) == 0)
        {
            sscanf(linea + 14, "%d", &r.id);
            leyendo = 1;
        }
        else if (strncmp(linea, "NOMBRE      : ", 14) == 0)
            strncpy(r.nombre, linea + 14, MAX_NOMBRE - 1);
        else if (strncmp(linea, "DESCRIPCION : ", 14) == 0)
            strncpy(r.descripcion, linea + 14, MAX_DESC - 1);
        else if (strncmp(linea, "AREA        : ", 14) == 0)
            strncpy(r.area, linea + 14, MAX_AREA - 1);
        else if (linea[0] == 'F' && linea[1] == '=')
        {
            sscanf(linea, "F=%d S=%d P=%d E=%d A=%d V=%d",
                   &r.F, &r.S, &r.P, &r.E, &r.A, &r.V);
        }
        else if (strncmp(linea, "CLASIFICACION:", 14) == 0 && leyendo)
        {
            asegurarEspacio();
            calcularRiesgo(&r);
            riesgos[total] = r;
            total++;
            memset(&r, 0, sizeof(Riesgo));
            leyendo = 0;
        }
    }

    fclose(f);
    return 1;
}

void cargarArchivo(const char *ruta)
{
    const char *ext = extensionDe(ruta);
    int ok = 0;

    if      (strcmp(ext, ".json") == 0) ok = cargarJSON(ruta);
    else if (strcmp(ext, ".csv")  == 0) ok = cargarCSV(ruta);
    else                                ok = cargarTXT(ruta);

    if (ok) printf("Se cargaron %d riesgo(s) desde %s\n", total, ruta);
    else    printf("Aviso: no se pudo leer %s (se empieza vacio).\n", ruta);
}

/* ============================================================
   OPCION: CREAR RIESGO
   ============================================================ */
void crearRiesgo(void)
{
    Riesgo r;

    asegurarEspacio();

    printf("\n--- REGISTRAR NUEVO RIESGO ---\n");
    r.id = generarId();

    leerTexto("Nombre del riesgo: ", r.nombre, MAX_NOMBRE);
    leerTexto("Descripcion: ",       r.descripcion, MAX_DESC);
    leerTexto("Area afectada: ",     r.area, MAX_AREA);

    printf("\n=== CRITERIOS DEL METODO MOSLER (1 a 5) ===");

    r.F = leerCriterio("F - FUNCION",
        "Que tanto afecta a la actividad principal de la empresa?");
    r.S = leerCriterio("S - SUSTITUCION",
        "Que tan dificil es reponer los bienes danados?");
    r.P = leerCriterio("P - PROFUNDIDAD",
        "Que tanto afecta la imagen y el animo de la empresa?");
    r.E = leerCriterio("E - EXTENSION",
        "Que tan amplio es el alcance del dano? (local, nacional...)");
    r.A = leerCriterio("A - AGRESION",
        "Que tan probable es que el riesgo se presente?");
    r.V = leerCriterio("V - VULNERABILIDAD",
        "Que tan probable es que realmente cause dano?");

    calcularRiesgo(&r);

    riesgos[total] = r;
    total++;

    printf("\nRiesgo registrado con el ID %d\n", r.id);
    printf("Evaluacion del Riesgo (ER) = %d  ->  %s\n", r.ER, r.clasificacion);

    guardarArchivo(archivoActual);
}

/* ============================================================
   OPCION: MODIFICAR RIESGO
   ============================================================ */
void modificarRiesgo(void)
{
    int id, pos, opcion;

    if (total == 0) { printf("\nNo hay riesgos registrados.\n"); return; }

    id  = leerEntero("\nID del riesgo a modificar: ");
    pos = buscarPorId(id);

    if (pos == -1) { printf("No existe un riesgo con ese ID.\n"); return; }

    printf("\nRiesgo: %s (ER actual = %d)\n", riesgos[pos].nombre, riesgos[pos].ER);
    printf("\nQue desea modificar?\n");
    printf("  1. Nombre           2. Descripcion      3. Area\n");
    printf("  4. F (Funcion)      5. S (Sustitucion)  6. P (Profundidad)\n");
    printf("  7. E (Extension)    8. A (Agresion)     9. V (Vulnerabilidad)\n");
    printf("  0. Cancelar\n");

    opcion = leerEntero("Opcion: ");

    switch (opcion)
    {
        case 1: leerTexto("Nuevo nombre: ", riesgos[pos].nombre, MAX_NOMBRE); break;
        case 2: leerTexto("Nueva descripcion: ", riesgos[pos].descripcion, MAX_DESC); break;
        case 3: leerTexto("Nueva area: ", riesgos[pos].area, MAX_AREA); break;
        case 4: riesgos[pos].F = leerCriterio("F - FUNCION", "Nuevo valor"); break;
        case 5: riesgos[pos].S = leerCriterio("S - SUSTITUCION", "Nuevo valor"); break;
        case 6: riesgos[pos].P = leerCriterio("P - PROFUNDIDAD", "Nuevo valor"); break;
        case 7: riesgos[pos].E = leerCriterio("E - EXTENSION", "Nuevo valor"); break;
        case 8: riesgos[pos].A = leerCriterio("A - AGRESION", "Nuevo valor"); break;
        case 9: riesgos[pos].V = leerCriterio("V - VULNERABILIDAD", "Nuevo valor"); break;
        case 0: printf("Operacion cancelada.\n"); return;
        default: printf("Opcion invalida.\n"); return;
    }

    calcularRiesgo(&riesgos[pos]);
    printf("\nModificado. Nuevo ER = %d  ->  %s\n",
           riesgos[pos].ER, riesgos[pos].clasificacion);

    guardarArchivo(archivoActual);
}

/* ============================================================
   OPCION: BORRAR RIESGO
   ============================================================ */
void borrarRiesgo(void)
{
    int id, pos, i;
    char confirmar[10];

    if (total == 0) { printf("\nNo hay riesgos registrados.\n"); return; }

    id  = leerEntero("\nID del riesgo a borrar: ");
    pos = buscarPorId(id);

    if (pos == -1) { printf("No existe un riesgo con ese ID.\n"); return; }

    printf("Seguro que desea borrar \"%s\"? (s/n): ", riesgos[pos].nombre);
    if (fgets(confirmar, sizeof(confirmar), stdin) == NULL) return;

    if (confirmar[0] != 's' && confirmar[0] != 'S')
    {
        printf("Operacion cancelada.\n");
        return;
    }

    /* Se corren los siguientes una posicion hacia atras */
    for (i = pos; i < total - 1; i++)
        riesgos[i] = riesgos[i + 1];

    total--;
    printf("Riesgo borrado correctamente.\n");

    guardarArchivo(archivoActual);
}

/* ============================================================
   OPCION: IMPRIMIR RIESGOS
   ============================================================ */
void imprimirTabla(void)
{
    int i;

    if (total == 0) { printf("\nNo hay riesgos registrados.\n"); return; }

    printf("\n===============================================================================\n");
    printf(" ID  NOMBRE                    AREA            C    Pb    ER    CLASIFICACION\n");
    printf("===============================================================================\n");

    for (i = 0; i < total; i++)
    {
        printf(" %-3d %-25.25s %-15.15s %-4d %-5d %-5d %s\n",
               riesgos[i].id, riesgos[i].nombre, riesgos[i].area,
               riesgos[i].C, riesgos[i].Pb, riesgos[i].ER,
               riesgos[i].clasificacion);
    }
    printf("===============================================================================\n");
    printf("Total: %d riesgo(s)   |   Memoria reservada para: %d\n", total, capacidad);
}

void imprimirDetalle(void)
{
    int id, pos;
    Riesgo r;

    if (total == 0) { printf("\nNo hay riesgos registrados.\n"); return; }

    imprimirTabla();
    id  = leerEntero("\nID del riesgo a detallar: ");
    pos = buscarPorId(id);

    if (pos == -1) { printf("No existe un riesgo con ese ID.\n"); return; }

    r = riesgos[pos];

    printf("\n===============================================================\n");
    printf("  RIESGO #%d: %s\n", r.id, r.nombre);
    printf("===============================================================\n");
    printf("  Descripcion : %s\n", r.descripcion);
    printf("  Area        : %s\n", r.area);

    printf("\n  --- CRITERIOS ---\n");
    printf("  F (Funcion)        = %d\n", r.F);
    printf("  S (Sustitucion)    = %d\n", r.S);
    printf("  P (Profundidad)    = %d\n", r.P);
    printf("  E (Extension)      = %d\n", r.E);
    printf("  A (Agresion)       = %d\n", r.A);
    printf("  V (Vulnerabilidad) = %d\n", r.V);

    printf("\n  --- CALCULOS ---\n");
    printf("  I  = F x S  = %d x %d = %d   (Importancia)\n", r.F, r.S, r.I);
    printf("  D  = P x E  = %d x %d = %d   (Danos)\n",       r.P, r.E, r.D);
    printf("  C  = I + D  = %d + %d = %d   (Caracter)\n",    r.I, r.D, r.C);
    printf("  Pb = A x V  = %d x %d = %d   (Probabilidad)\n",r.A, r.V, r.Pb);
    printf("  ER = C x Pb = %d x %d = %d   (Evaluacion)\n",  r.C, r.Pb, r.ER);

    printf("\n  RESULTADO: RIESGO %s  (ER = %d de 1250)\n", r.clasificacion, r.ER);
    printf("===============================================================\n");
}

/* ============================================================
   MODULO DE IA (SISTEMA EXPERTO)
   ------------------------------------------------------------
   Analiza los riesgos aplicando reglas de decision y genera
   recomendaciones automaticas de tratamiento.
   ============================================================ */
void moduloIA(void)
{
    int i, pos, id;
    Riesgo r;
    int criticoMayor;
    const char *nombreCritico;

    if (total == 0) { printf("\nNo hay riesgos para analizar.\n"); return; }

    imprimirTabla();
    id  = leerEntero("\nID del riesgo a analizar con IA: ");
    pos = buscarPorId(id);

    if (pos == -1) { printf("No existe un riesgo con ese ID.\n"); return; }

    r = riesgos[pos];

    printf("\n===============================================================\n");
    printf("   ANALISIS AUTOMATICO (SISTEMA EXPERTO)\n");
    printf("   Riesgo: %s\n", r.nombre);
    printf("===============================================================\n");

    /* --- Regla 1: identificar cual criterio pesa mas --- */
    criticoMayor  = r.F;  nombreCritico = "FUNCION";
    if (r.S > criticoMayor) { criticoMayor = r.S; nombreCritico = "SUSTITUCION"; }
    if (r.P > criticoMayor) { criticoMayor = r.P; nombreCritico = "PROFUNDIDAD"; }
    if (r.E > criticoMayor) { criticoMayor = r.E; nombreCritico = "EXTENSION"; }
    if (r.A > criticoMayor) { criticoMayor = r.A; nombreCritico = "AGRESION"; }
    if (r.V > criticoMayor) { criticoMayor = r.V; nombreCritico = "VULNERABILIDAD"; }

    printf("\n  [Diagnostico]\n");
    printf("  Factor mas critico detectado: %s (valor %d de 5)\n",
           nombreCritico, criticoMayor);

    /* --- Regla 2: comparar caracter contra probabilidad ---
       Se convierten a porcentaje porque tienen escalas distintas:
       C va de 2 a 50, mientras que Pb va de 1 a 25.            */
    {
        int impactoPct = r.C  * 100 / 50;
        int probPct    = r.Pb * 100 / 25;

        printf("  Nivel de impacto      : %d%%\n", impactoPct);
        printf("  Nivel de probabilidad : %d%%\n", probPct);

        if (impactoPct - probPct > 25)
            printf("  El riesgo es de ALTO IMPACTO pero POCO PROBABLE.\n");
        else if (probPct - impactoPct > 25)
            printf("  El riesgo es MUY PROBABLE aunque de impacto moderado.\n");
        else
            printf("  El impacto y la probabilidad estan equilibrados.\n");
    }

    /* --- Regla 3: recomendacion segun clasificacion --- */
    printf("\n  [Recomendaciones]\n");

    if (r.ER > 1000)
    {
        printf("  * PRIORIDAD MAXIMA: atender de inmediato.\n");
        printf("  * Detener o aislar la actividad afectada si es posible.\n");
        printf("  * Asignar presupuesto de emergencia y un responsable directo.\n");
        printf("  * Elaborar un plan de contingencia formal esta semana.\n");
    }
    else if (r.ER > 750)
    {
        printf("  * PRIORIDAD ALTA: atender en el corto plazo.\n");
        printf("  * Implementar controles preventivos y de deteccion.\n");
        printf("  * Evaluar la contratacion de un seguro que cubra el dano.\n");
        printf("  * Revisar el avance cada mes.\n");
    }
    else if (r.ER > 500)
    {
        printf("  * PRIORIDAD MEDIA: incluir en el plan anual.\n");
        printf("  * Reforzar los procedimientos existentes.\n");
        printf("  * Capacitar al personal del area afectada.\n");
    }
    else if (r.ER > 250)
    {
        printf("  * PRIORIDAD BAJA: mantener bajo vigilancia.\n");
        printf("  * Revisar la evaluacion cada seis meses.\n");
    }
    else
    {
        printf("  * RIESGO ACEPTABLE: no requiere accion inmediata.\n");
        printf("  * Documentar y revisar una vez al ano.\n");
    }

    /* --- Regla 4: sugerencia especifica segun el factor critico --- */
    printf("\n  [Accion sugerida sobre el factor %s]\n", nombreCritico);

    if (strcmp(nombreCritico, "FUNCION") == 0)
        printf("  Diversificar procesos para que la empresa no dependa\n"
               "  de una sola actividad o recurso.\n");
    else if (strcmp(nombreCritico, "SUSTITUCION") == 0)
        printf("  Mantener repuestos, respaldos o proveedores alternos\n"
               "  para reponer rapidamente lo danado.\n");
    else if (strcmp(nombreCritico, "PROFUNDIDAD") == 0)
        printf("  Preparar un plan de comunicacion y manejo de imagen\n"
               "  para responder ante la opinion publica.\n");
    else if (strcmp(nombreCritico, "EXTENSION") == 0)
        printf("  Sectorizar las operaciones para que un incidente\n"
               "  no se propague a toda la organizacion.\n");
    else if (strcmp(nombreCritico, "AGRESION") == 0)
        printf("  Reforzar la seguridad fisica y los controles de acceso\n"
               "  para reducir la probabilidad de ocurrencia.\n");
    else
        printf("  Corregir las debilidades internas detectadas mediante\n"
               "  auditorias y mantenimiento preventivo.\n");

    /* --- Regla 5: comparacion con el resto de riesgos --- */
    {
        int mayores = 0;
        for (i = 0; i < total; i++)
            if (riesgos[i].ER > r.ER) mayores++;

        printf("\n  [Posicion en la empresa]\n");
        printf("  Este riesgo ocupa el lugar %d de %d en gravedad.\n",
               mayores + 1, total);
    }

    printf("===============================================================\n");
}

/* ============================================================
   MANEJO DE ARCHIVOS ESPECIFICOS
   ============================================================ */
void seleccionarArchivo(void)
{
    char ruta[MAX_RUTA];
    FILE *prueba;

    printf("\nArchivo actual: %s\n", archivoActual);
    leerTexto("Nombre del archivo a abrir (ej: datos.json): ", ruta, MAX_RUTA);

    if (strlen(ruta) == 0) { printf("Operacion cancelada.\n"); return; }

    prueba = fopen(ruta, "r");
    if (prueba == NULL)
    {
        printf("Ese archivo no existe. Use la opcion de crear archivo.\n");
        return;
    }
    fclose(prueba);

    strcpy(archivoActual, ruta);
    total = 0;                 /* se limpia lo que habia en memoria */
    cargarArchivo(archivoActual);
}

void crearArchivo(void)
{
    char ruta[MAX_RUTA];
    char respuesta[10];

    printf("\nFormatos disponibles segun la extension:\n");
    printf("  .txt  = texto plano    .csv  = separado por comas\n");
    printf("  .json = formato JSON   .xml  = formato XML\n");

    leerTexto("Nombre del nuevo archivo: ", ruta, MAX_RUTA);
    if (strlen(ruta) == 0) { printf("Operacion cancelada.\n"); return; }

    printf("Desea vaciar los riesgos actuales? (s/n): ");
    if (fgets(respuesta, sizeof(respuesta), stdin) == NULL) return;

    if (respuesta[0] == 's' || respuesta[0] == 'S')
        total = 0;

    strcpy(archivoActual, ruta);
    guardarArchivo(archivoActual);
    printf("Archivo creado y seleccionado como archivo activo.\n");
}

void exportarTodos(void)
{
    char base[MAX_RUTA];
    char ruta[MAX_RUTA + 10];

    if (total == 0) { printf("\nNo hay riesgos para exportar.\n"); return; }

    leerTexto("\nNombre base (sin extension): ", base, MAX_RUTA);
    if (strlen(base) == 0) { printf("Operacion cancelada.\n"); return; }

    sprintf(ruta, "%s.txt",  base);  guardarTXT(ruta);  printf("  - %s\n", ruta);
    sprintf(ruta, "%s.csv",  base);  guardarCSV(ruta);  printf("  - %s\n", ruta);
    sprintf(ruta, "%s.json", base);  guardarJSON(ruta); printf("  - %s\n", ruta);
    sprintf(ruta, "%s.xml",  base);  guardarXML(ruta);  printf("  - %s\n", ruta);

    printf("Exportacion completada en los 4 formatos.\n");
}

/* ============================================================
   PROGRAMA PRINCIPAL
   ============================================================ */
int main(void)
{
    int opcion;

    iniciarMemoria();
    cargarArchivo(archivoActual);

    do
    {
        printf("\n===============================================================\n");
        printf("     ANALISIS DE RIESGOS - METODO MOSLER\n");
        printf("===============================================================\n");
        printf("  Archivo activo: %s   |   Riesgos: %d\n", archivoActual, total);
        printf("---------------------------------------------------------------\n");
        printf("   1. Crear riesgo\n");
        printf("   2. Modificar parametros de un riesgo\n");
        printf("   3. Recalcular todos los riesgos\n");
        printf("   4. Imprimir riesgos (tabla)\n");
        printf("   5. Ver detalle de un riesgo\n");
        printf("   6. Borrar riesgo\n");
        printf("   7. Seleccionar un archivo existente\n");
        printf("   8. Crear un archivo nuevo\n");
        printf("   9. Exportar a los 4 formatos (txt, csv, json, xml)\n");
        printf("  10. Analisis con IA (sistema experto)\n");
        printf("   0. Salir\n");
        printf("---------------------------------------------------------------\n");

        opcion = leerEntero("  Seleccione una opcion: ");

        switch (opcion)
        {
            case 1:  crearRiesgo();        break;
            case 2:  modificarRiesgo();    break;
            case 3:  calcularTodos();
                     guardarArchivo(archivoActual); break;
            case 4:  imprimirTabla();      break;
            case 5:  imprimirDetalle();    break;
            case 6:  borrarRiesgo();       break;
            case 7:  seleccionarArchivo(); break;
            case 8:  crearArchivo();       break;
            case 9:  exportarTodos();      break;
            case 10: moduloIA();           break;
            case 0:
                guardarArchivo(archivoActual);
                printf("\nDatos guardados. Programa finalizado.\n");
                break;
            default:
                printf("\nOpcion invalida.\n");
        }

    } while (opcion != 0);

    liberarMemoria();   /* muy importante: se libera la memoria dinamica */
    return 0;
}
