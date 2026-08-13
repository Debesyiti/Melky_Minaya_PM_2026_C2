/* ============================================================
   CALCULADORA DE COSTO REAL DE UN VEHICULO
   ------------------------------------------------------------
   El programa permite registrar vehiculos, guardarlos en un
   archivo, y calcular cuanto cuesta realmente usarlos por
   kilometro, tomando en cuenta:
     - Amortizacion (depreciacion) del vehiculo
     - Mantenimiento
     - Seguro
     - Neumaticos
     - Combustible (diferente en ciudad y en autopista)

   Moneda utilizada: RD$ (pesos dominicanos)

   Nombre: Melky Minaya
   Matricula: 2026-0327
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VEHICULOS 50
#define ARCHIVO_VEHICULOS "vehiculos.txt"
#define ARCHIVO_CONFIG    "config.txt"

/* ------------------------------------------------------------
   ESTRUCTURA: guarda todos los datos de un vehiculo
   ------------------------------------------------------------ */
typedef struct {
    int    id;
    char   marca[40];
    char   modelo[40];
    int    anio;
    double precioCompra;          /* RD$ cuanto costo el vehiculo   */
    double valorResidual;         /* RD$ en cuanto se vendera al final */
    int    vidaUtil;              /* anios que se piensa usar        */
    double kmPorAnio;             /* km que se recorren cada anio    */
    double kmPorGalonCiudad;      /* rendimiento en ciudad           */
    double kmPorGalonAutopista;   /* rendimiento en autopista        */
    double seguroAnual;           /* RD$ por anio                    */
    double mantenimientoAnual;    /* RD$ por anio                    */
    double precioNeumaticos;      /* RD$ el juego completo           */
    double duracionNeumaticos;    /* km que duran los neumaticos     */
} Vehiculo;

/* ------------------------------------------------------------
   ESTRUCTURA: guarda los resultados de todos los calculos
   ------------------------------------------------------------ */
typedef struct {
    double amortizacionAnual;
    double amortizacionKm;
    double mantenimientoVidaUtil;
    double mantenimientoKm;
    double seguroKm;
    double neumaticosKm;
    double costoFijoKm;             /* todo lo que no es combustible */
    double combustibleKmCiudad;
    double combustibleKmAutopista;
    double costoTotalKmCiudad;
    double costoTotalKmAutopista;
} Costos;

/* ------------------------------------------------------------
   VARIABLES GLOBALES
   ------------------------------------------------------------ */
Vehiculo vehiculos[MAX_VEHICULOS];
int      totalVehiculos    = 0;
double   precioCombustible = 290.00;   /* RD$ por galon */

/* ============================================================
   FUNCIONES AUXILIARES DE ENTRADA
   ============================================================ */

/* Quita el salto de linea '\n' que deja fgets al final */
void quitarSalto(char *texto)
{
    int largo = strlen(texto);
    if (largo > 0 && texto[largo - 1] == '\n')
        texto[largo - 1] = '\0';
}

/* Pide un texto al usuario (puede tener espacios) */
void leerTexto(const char *mensaje, char *destino, int tamano)
{
    printf("%s", mensaje);
    fgets(destino, tamano, stdin);
    quitarSalto(destino);
}

/* Pide un numero entero, validando que sea correcto */
int leerEntero(const char *mensaje)
{
    char linea[100];
    int  valor;

    while (1)
    {
        printf("%s", mensaje);
        fgets(linea, sizeof(linea), stdin);
        if (sscanf(linea, "%d", &valor) == 1)
            return valor;
        printf("  >> Dato invalido, escriba un numero entero.\n");
    }
}

/* Pide un numero con decimales, validando que sea mayor que cero */
double leerDouble(const char *mensaje)
{
    char   linea[100];
    double valor;

    while (1)
    {
        printf("%s", mensaje);
        fgets(linea, sizeof(linea), stdin);
        if (sscanf(linea, "%lf", &valor) == 1 && valor > 0)
            return valor;
        printf("  >> Dato invalido, escriba un numero mayor que cero.\n");
    }
}

/* ============================================================
   MANEJO DE ARCHIVOS
   ============================================================ */

/* Guarda todos los vehiculos en el archivo de texto */
void guardarVehiculos(void)
{
    FILE *archivo;
    int i;

    archivo = fopen(ARCHIVO_VEHICULOS, "w");
    if (archivo == NULL)
    {
        printf("Error: no se pudo abrir el archivo para guardar.\n");
        return;
    }

    /* Primero se escribe cuantos vehiculos hay */
    fprintf(archivo, "%d\n", totalVehiculos);

    /* Luego cada dato en su propia linea */
    for (i = 0; i < totalVehiculos; i++)
    {
        fprintf(archivo, "%d\n",   vehiculos[i].id);
        fprintf(archivo, "%s\n",   vehiculos[i].marca);
        fprintf(archivo, "%s\n",   vehiculos[i].modelo);
        fprintf(archivo, "%d\n",   vehiculos[i].anio);
        fprintf(archivo, "%.2f\n", vehiculos[i].precioCompra);
        fprintf(archivo, "%.2f\n", vehiculos[i].valorResidual);
        fprintf(archivo, "%d\n",   vehiculos[i].vidaUtil);
        fprintf(archivo, "%.2f\n", vehiculos[i].kmPorAnio);
        fprintf(archivo, "%.2f\n", vehiculos[i].kmPorGalonCiudad);
        fprintf(archivo, "%.2f\n", vehiculos[i].kmPorGalonAutopista);
        fprintf(archivo, "%.2f\n", vehiculos[i].seguroAnual);
        fprintf(archivo, "%.2f\n", vehiculos[i].mantenimientoAnual);
        fprintf(archivo, "%.2f\n", vehiculos[i].precioNeumaticos);
        fprintf(archivo, "%.2f\n", vehiculos[i].duracionNeumaticos);
    }

    fclose(archivo);
}

/* Lee los vehiculos desde el archivo al iniciar el programa */
void cargarVehiculos(void)
{
    FILE *archivo;
    char linea[100];
    int i;

    archivo = fopen(ARCHIVO_VEHICULOS, "r");
    if (archivo == NULL)
    {
        /* Si el archivo no existe todavia, se empieza sin vehiculos */
        totalVehiculos = 0;
        return;
    }

    fgets(linea, sizeof(linea), archivo);
    sscanf(linea, "%d", &totalVehiculos);

    for (i = 0; i < totalVehiculos; i++)
    {
        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%d", &vehiculos[i].id);

        fgets(vehiculos[i].marca, sizeof(vehiculos[i].marca), archivo);
        quitarSalto(vehiculos[i].marca);

        fgets(vehiculos[i].modelo, sizeof(vehiculos[i].modelo), archivo);
        quitarSalto(vehiculos[i].modelo);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%d", &vehiculos[i].anio);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%lf", &vehiculos[i].precioCompra);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%lf", &vehiculos[i].valorResidual);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%d", &vehiculos[i].vidaUtil);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%lf", &vehiculos[i].kmPorAnio);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%lf", &vehiculos[i].kmPorGalonCiudad);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%lf", &vehiculos[i].kmPorGalonAutopista);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%lf", &vehiculos[i].seguroAnual);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%lf", &vehiculos[i].mantenimientoAnual);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%lf", &vehiculos[i].precioNeumaticos);

        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%lf", &vehiculos[i].duracionNeumaticos);
    }

    fclose(archivo);
}

/* Guarda el precio del combustible en su propio archivo */
void guardarConfig(void)
{
    FILE *archivo = fopen(ARCHIVO_CONFIG, "w");
    if (archivo != NULL)
    {
        fprintf(archivo, "%.2f\n", precioCombustible);
        fclose(archivo);
    }
}

/* Lee el precio del combustible al iniciar */
void cargarConfig(void)
{
    FILE *archivo = fopen(ARCHIVO_CONFIG, "r");
    char linea[100];

    if (archivo != NULL)
    {
        if (fgets(linea, sizeof(linea), archivo) != NULL)
            sscanf(linea, "%lf", &precioCombustible);
        fclose(archivo);
    }
}

/* ============================================================
   BUSQUEDA
   ============================================================ */

/* Devuelve la posicion del vehiculo con ese id, o -1 si no existe */
int buscarPorId(int id)
{
    int i;
    for (i = 0; i < totalVehiculos; i++)
        if (vehiculos[i].id == id)
            return i;
    return -1;
}

/* Genera un id nuevo (el mayor existente + 1) */
int generarId(void)
{
    int i, mayor = 0;
    for (i = 0; i < totalVehiculos; i++)
        if (vehiculos[i].id > mayor)
            mayor = vehiculos[i].id;
    return mayor + 1;
}

/* ============================================================
   CALCULOS DE COSTOS  (el corazon del programa)
   ============================================================ */
Costos calcularCostos(Vehiculo v)
{
    Costos c;

    /* 1) AMORTIZACION: cuanto pierde de valor el vehiculo cada anio.
          Se le resta el valor residual porque al final se puede vender. */
    c.amortizacionAnual = (v.precioCompra - v.valorResidual) / v.vidaUtil;
    c.amortizacionKm    = c.amortizacionAnual / v.kmPorAnio;

    /* 2) MANTENIMIENTO durante toda la vida util y por kilometro */
    c.mantenimientoVidaUtil = v.mantenimientoAnual * v.vidaUtil;
    c.mantenimientoKm       = v.mantenimientoAnual / v.kmPorAnio;

    /* 3) SEGURO por kilometro */
    c.seguroKm = v.seguroAnual / v.kmPorAnio;

    /* 4) NEUMATICOS por kilometro */
    c.neumaticosKm = v.precioNeumaticos / v.duracionNeumaticos;

    /* 5) COSTO FIJO: todo lo que se paga aunque no se gaste gasolina */
    c.costoFijoKm = c.amortizacionKm + c.mantenimientoKm +
                    c.seguroKm + c.neumaticosKm;

    /* 6) COMBUSTIBLE por kilometro (distinto en ciudad y autopista) */
    c.combustibleKmCiudad    = precioCombustible / v.kmPorGalonCiudad;
    c.combustibleKmAutopista = precioCombustible / v.kmPorGalonAutopista;

    /* 7) COSTO REAL por kilometro = costo fijo + combustible */
    c.costoTotalKmCiudad    = c.costoFijoKm + c.combustibleKmCiudad;
    c.costoTotalKmAutopista = c.costoFijoKm + c.combustibleKmAutopista;

    return c;
}

/* ============================================================
   OPCION 1: CREAR VEHICULO
   ============================================================ */
void crearVehiculo(void)
{
    Vehiculo v;

    if (totalVehiculos >= MAX_VEHICULOS)
    {
        printf("\nNo hay espacio para mas vehiculos.\n");
        return;
    }

    printf("\n--- REGISTRAR NUEVO VEHICULO ---\n");

    v.id = generarId();

    leerTexto("Marca: ", v.marca, sizeof(v.marca));
    leerTexto("Modelo: ", v.modelo, sizeof(v.modelo));

    v.anio                = leerEntero("Anio: ");
    v.precioCompra        = leerDouble("Precio de compra (RD$): ");
    v.valorResidual       = leerDouble("Valor de reventa al final (RD$): ");
    v.vidaUtil            = leerEntero("Vida util (anios): ");
    v.kmPorAnio           = leerDouble("Kilometros que recorre por anio: ");
    v.kmPorGalonCiudad    = leerDouble("Rendimiento en CIUDAD (km por galon): ");
    v.kmPorGalonAutopista = leerDouble("Rendimiento en AUTOPISTA (km por galon): ");
    v.seguroAnual         = leerDouble("Seguro anual (RD$): ");
    v.mantenimientoAnual  = leerDouble("Mantenimiento anual (RD$): ");
    v.precioNeumaticos    = leerDouble("Precio del juego de neumaticos (RD$): ");
    v.duracionNeumaticos  = leerDouble("Duracion de los neumaticos (km): ");

    vehiculos[totalVehiculos] = v;
    totalVehiculos++;

    guardarVehiculos();

    printf("\nVehiculo registrado correctamente con el ID %d\n", v.id);
}

/* ============================================================
   OPCION 2: LISTAR VEHICULOS
   ============================================================ */
void listarVehiculos(void)
{
    int i;

    if (totalVehiculos == 0)
    {
        printf("\nNo hay vehiculos registrados.\n");
        return;
    }

    printf("\n----------------------------------------------------------\n");
    printf(" ID  MARCA           MODELO          ANIO   PRECIO (RD$)\n");
    printf("----------------------------------------------------------\n");

    for (i = 0; i < totalVehiculos; i++)
    {
        printf(" %-3d %-15s %-15s %-6d %12.2f\n",
               vehiculos[i].id,
               vehiculos[i].marca,
               vehiculos[i].modelo,
               vehiculos[i].anio,
               vehiculos[i].precioCompra);
    }
    printf("----------------------------------------------------------\n");
    printf("Total de vehiculos: %d\n", totalVehiculos);
}

/* ============================================================
   OPCION 3: MODIFICAR VEHICULO
   ============================================================ */
void modificarVehiculo(void)
{
    int id, pos, opcion;

    listarVehiculos();
    if (totalVehiculos == 0) return;

    id  = leerEntero("\nID del vehiculo a modificar: ");
    pos = buscarPorId(id);

    if (pos == -1)
    {
        printf("No existe un vehiculo con ese ID.\n");
        return;
    }

    printf("\nVehiculo: %s %s (%d)\n", vehiculos[pos].marca,
           vehiculos[pos].modelo, vehiculos[pos].anio);
    printf("\nQue desea modificar?\n");
    printf(" 1. Marca                     2. Modelo\n");
    printf(" 3. Anio                      4. Precio de compra\n");
    printf(" 5. Valor de reventa          6. Vida util\n");
    printf(" 7. Km por anio               8. Rendimiento ciudad\n");
    printf(" 9. Rendimiento autopista    10. Seguro anual\n");
    printf("11. Mantenimiento anual      12. Precio neumaticos\n");
    printf("13. Duracion neumaticos       0. Cancelar\n");

    opcion = leerEntero("Opcion: ");

    switch (opcion)
    {
        case 1: leerTexto("Nueva marca: ", vehiculos[pos].marca, 40); break;
        case 2: leerTexto("Nuevo modelo: ", vehiculos[pos].modelo, 40); break;
        case 3: vehiculos[pos].anio = leerEntero("Nuevo anio: "); break;
        case 4: vehiculos[pos].precioCompra = leerDouble("Nuevo precio (RD$): "); break;
        case 5: vehiculos[pos].valorResidual = leerDouble("Nuevo valor de reventa (RD$): "); break;
        case 6: vehiculos[pos].vidaUtil = leerEntero("Nueva vida util (anios): "); break;
        case 7: vehiculos[pos].kmPorAnio = leerDouble("Nuevos km por anio: "); break;
        case 8: vehiculos[pos].kmPorGalonCiudad = leerDouble("Nuevo rendimiento ciudad: "); break;
        case 9: vehiculos[pos].kmPorGalonAutopista = leerDouble("Nuevo rendimiento autopista: "); break;
        case 10: vehiculos[pos].seguroAnual = leerDouble("Nuevo seguro anual (RD$): "); break;
        case 11: vehiculos[pos].mantenimientoAnual = leerDouble("Nuevo mantenimiento anual (RD$): "); break;
        case 12: vehiculos[pos].precioNeumaticos = leerDouble("Nuevo precio neumaticos (RD$): "); break;
        case 13: vehiculos[pos].duracionNeumaticos = leerDouble("Nueva duracion neumaticos (km): "); break;
        case 0: printf("Operacion cancelada.\n"); return;
        default: printf("Opcion invalida.\n"); return;
    }

    guardarVehiculos();
    printf("\nDato modificado correctamente.\n");
}

/* ============================================================
   OPCION 4: BORRAR VEHICULO
   ============================================================ */
void borrarVehiculo(void)
{
    int id, pos, i;
    char confirmar[10];

    listarVehiculos();
    if (totalVehiculos == 0) return;

    id  = leerEntero("\nID del vehiculo a borrar: ");
    pos = buscarPorId(id);

    if (pos == -1)
    {
        printf("No existe un vehiculo con ese ID.\n");
        return;
    }

    printf("Seguro que desea borrar %s %s? (s/n): ",
           vehiculos[pos].marca, vehiculos[pos].modelo);
    fgets(confirmar, sizeof(confirmar), stdin);

    if (confirmar[0] != 's' && confirmar[0] != 'S')
    {
        printf("Operacion cancelada.\n");
        return;
    }

    /* Se corren todos los vehiculos una posicion hacia atras */
    for (i = pos; i < totalVehiculos - 1; i++)
        vehiculos[i] = vehiculos[i + 1];

    totalVehiculos--;
    guardarVehiculos();

    printf("Vehiculo borrado correctamente.\n");
}

/* ============================================================
   OPCION 5: ANALISIS COMPLETO DE COSTOS
   ============================================================ */
void analisisCostos(void)
{
    int id, pos;
    Vehiculo v;
    Costos c;

    listarVehiculos();
    if (totalVehiculos == 0) return;

    id  = leerEntero("\nID del vehiculo a analizar: ");
    pos = buscarPorId(id);

    if (pos == -1)
    {
        printf("No existe un vehiculo con ese ID.\n");
        return;
    }

    v = vehiculos[pos];
    c = calcularCostos(v);

    printf("\n==========================================================\n");
    printf("  ANALISIS DE COSTOS: %s %s (%d)\n", v.marca, v.modelo, v.anio);
    printf("==========================================================\n");
    printf("  Precio de combustible actual: RD$ %.2f por galon\n", precioCombustible);

    printf("\n--- 1. AMORTIZACION (depreciacion) ---\n");
    printf("  Precio de compra          : RD$ %12.2f\n", v.precioCompra);
    printf("  Valor de reventa          : RD$ %12.2f\n", v.valorResidual);
    printf("  Perdida total de valor    : RD$ %12.2f\n", v.precioCompra - v.valorResidual);
    printf("  Vida util                 :     %9d anios\n", v.vidaUtil);
    printf("  Amortizacion por anio     : RD$ %12.2f\n", c.amortizacionAnual);
    printf("  Amortizacion por km       : RD$ %12.2f\n", c.amortizacionKm);

    printf("\n--- 2. MANTENIMIENTO ---\n");
    printf("  Mantenimiento por anio    : RD$ %12.2f\n", v.mantenimientoAnual);
    printf("  Total en la vida util     : RD$ %12.2f\n", c.mantenimientoVidaUtil);
    printf("  Mantenimiento por km      : RD$ %12.2f\n", c.mantenimientoKm);

    printf("\n--- 3. SEGURO Y NEUMATICOS ---\n");
    printf("  Seguro por anio           : RD$ %12.2f\n", v.seguroAnual);
    printf("  Seguro por km             : RD$ %12.2f\n", c.seguroKm);
    printf("  Juego de neumaticos       : RD$ %12.2f  (duran %.0f km)\n",
           v.precioNeumaticos, v.duracionNeumaticos);
    printf("  Neumaticos por km         : RD$ %12.2f\n", c.neumaticosKm);

    printf("\n--- 4. COMBUSTIBLE ---\n");
    printf("  Rendimiento en ciudad     :     %9.2f km/galon\n", v.kmPorGalonCiudad);
    printf("  Rendimiento en autopista  :     %9.2f km/galon\n", v.kmPorGalonAutopista);
    printf("  Combustible km ciudad     : RD$ %12.2f\n", c.combustibleKmCiudad);
    printf("  Combustible km autopista  : RD$ %12.2f\n", c.combustibleKmAutopista);

    printf("\n--- 5. COSTO REAL POR KILOMETRO ---\n");
    printf("  Costo fijo por km         : RD$ %12.2f\n", c.costoFijoKm);
    printf("     (amortizacion + mantenimiento + seguro + neumaticos)\n");
    printf("  ------------------------------------------------------\n");
    printf("  COSTO REAL EN CIUDAD      : RD$ %12.2f por km\n", c.costoTotalKmCiudad);
    printf("  COSTO REAL EN AUTOPISTA   : RD$ %12.2f por km\n", c.costoTotalKmAutopista);
    printf("==========================================================\n");
}

/* ============================================================
   OPCION 6: CALCULAR UN VIAJE
   ============================================================ */
void calcularViaje(void)
{
    int id, pos;
    double kmCiudad, kmAutopista, kmTotal;
    double galonesCiudad, galonesAutopista, galonesTotal;
    double costoCombustible, costoRealViaje;
    Vehiculo v;
    Costos c;

    listarVehiculos();
    if (totalVehiculos == 0) return;

    id  = leerEntero("\nID del vehiculo para el viaje: ");
    pos = buscarPorId(id);

    if (pos == -1)
    {
        printf("No existe un vehiculo con ese ID.\n");
        return;
    }

    v = vehiculos[pos];
    c = calcularCostos(v);

    printf("\n--- DATOS DEL VIAJE ---\n");
    kmCiudad    = leerDouble("Kilometros en CIUDAD: ");
    kmAutopista = leerDouble("Kilometros en AUTOPISTA: ");
    kmTotal     = kmCiudad + kmAutopista;

    /* Galones que se consumen en cada tipo de via */
    galonesCiudad    = kmCiudad    / v.kmPorGalonCiudad;
    galonesAutopista = kmAutopista / v.kmPorGalonAutopista;
    galonesTotal     = galonesCiudad + galonesAutopista;

    /* Costo solo de la gasolina */
    costoCombustible = galonesTotal * precioCombustible;

    /* Costo real: incluye el desgaste del vehiculo */
    costoRealViaje = (kmCiudad    * c.costoTotalKmCiudad) +
                     (kmAutopista * c.costoTotalKmAutopista);

    printf("\n==========================================================\n");
    printf("  RESULTADO DEL VIAJE: %s %s\n", v.marca, v.modelo);
    printf("==========================================================\n");
    printf("  Km en ciudad              :     %9.2f km\n", kmCiudad);
    printf("  Km en autopista           :     %9.2f km\n", kmAutopista);
    printf("  Distancia total           :     %9.2f km\n", kmTotal);

    printf("\n--- CONSUMO DE COMBUSTIBLE ---\n");
    printf("  Galones en ciudad         :     %9.2f gal\n", galonesCiudad);
    printf("  Galones en autopista      :     %9.2f gal\n", galonesAutopista);
    printf("  Galones totales           :     %9.2f gal\n", galonesTotal);
    printf("  COSTO EN COMBUSTIBLE      : RD$ %12.2f\n", costoCombustible);

    printf("\n--- COSTO REAL DEL VIAJE ---\n");
    printf("  (incluye combustible + desgaste del vehiculo)\n");
    printf("  Tramo ciudad              : RD$ %12.2f\n", kmCiudad * c.costoTotalKmCiudad);
    printf("  Tramo autopista           : RD$ %12.2f\n", kmAutopista * c.costoTotalKmAutopista);
    printf("  ------------------------------------------------------\n");
    printf("  COSTO REAL TOTAL          : RD$ %12.2f\n", costoRealViaje);
    if (kmTotal > 0)
        printf("  Promedio por kilometro    : RD$ %12.2f\n", costoRealViaje / kmTotal);
    printf("\n  Diferencia oculta         : RD$ %12.2f\n", costoRealViaje - costoCombustible);
    printf("  (esto es lo que el vehiculo cuesta ademas de la gasolina)\n");
    printf("==========================================================\n");
}

/* ============================================================
   OPCION 7: CAMBIAR PRECIO DEL COMBUSTIBLE
   ============================================================ */
void cambiarPrecioCombustible(void)
{
    printf("\nPrecio actual del combustible: RD$ %.2f por galon\n", precioCombustible);
    precioCombustible = leerDouble("Nuevo precio (RD$ por galon): ");
    guardarConfig();
    printf("Precio actualizado a RD$ %.2f por galon.\n", precioCombustible);
}

/* ============================================================
   MENU PRINCIPAL
   ============================================================ */
int main(void)
{
    int opcion;

    /* Al iniciar se leen los datos guardados anteriormente */
    cargarVehiculos();
    cargarConfig();

    do
    {
        printf("\n==========================================================\n");
        printf("     CALCULADORA DE COSTO REAL DE VEHICULOS  (RD$)\n");
        printf("==========================================================\n");
        printf("  Combustible: RD$ %.2f/galon   |   Vehiculos: %d\n",
               precioCombustible, totalVehiculos);
        printf("----------------------------------------------------------\n");
        printf("  1. Registrar un vehiculo nuevo\n");
        printf("  2. Ver lista de vehiculos\n");
        printf("  3. Modificar un vehiculo\n");
        printf("  4. Borrar un vehiculo\n");
        printf("  5. Analisis completo de costos\n");
        printf("  6. Calcular un viaje\n");
        printf("  7. Cambiar precio del combustible\n");
        printf("  0. Salir\n");
        printf("----------------------------------------------------------\n");

        opcion = leerEntero("  Seleccione una opcion: ");

        switch (opcion)
        {
            case 1: crearVehiculo();            break;
            case 2: listarVehiculos();          break;
            case 3: modificarVehiculo();        break;
            case 4: borrarVehiculo();           break;
            case 5: analisisCostos();           break;
            case 6: calcularViaje();            break;
            case 7: cambiarPrecioCombustible(); break;
            case 0: printf("\nPrograma finalizado. Datos guardados.\n"); break;
            default: printf("\nOpcion invalida, intente de nuevo.\n");
        }

    } while (opcion != 0);

    return 0;
}
