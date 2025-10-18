#include <stdio.h>
#include <string.h> // Se mantiene para strstr, strcmp y strcspn
#include <stdlib.h> // Para system("clear") / system("cls")

// ======== Constantes Globales ========
#define MAX_MEDICAMENTOS 50
#define MAX_DIAS_MES 31
#define MAX_NOMBRE 50
#define MAX_VENTAS_RECETA 100
#define MAX_INFORMES 12
#define MAX_LEN_INFORME 4096
#define PIN_DUENO 1234

// ======== Prototipos de Funciones Personalizadas (NUEVO) ========
void mi_strcpy(char* destino, const char* origen);
void mi_strcat(char* destino, const char* origen);
char mi_tolower(char c);
void limpiar_buffer_stdin();
void leer_linea(char* buffer, int tamano);

// ======== Prototipos de Funciones del Sistema ========
void limpiarPantalla();
void esperarEnter();
int buscarMedicamentoPorCodigo(int codigo, const int codigos[], int n);
int buscarMedicamentoPorNombre(const char* nombre, const char nombres[][MAX_NOMBRE], int n);
void ordenarMedicamentosPorCodigo(int codigos[], char nombres[][MAX_NOMBRE], double precios[], int stocks[], int esVentaLibre[], int stocksCriticos[], int n);
void listarMedicamentosDisponibles(const int codigos[], const char nombres[][MAX_NOMBRE], int n_medicamentos);
void mostrarMenu();
void registrarVenta(int dia, int codigos[], const char nombres[][MAX_NOMBRE], double precios[], int stocks[], const int esVentaLibre[], double ventasDiarias[][2], char nombresReceta[][MAX_NOMBRE], int cantidadesReceta[], long long dnisReceta[], int* contadorVentasReceta, int n_medicamentos);
void consultarStock(const int codigos[], const char nombres[][MAX_NOMBRE], const int stocks[], int n_medicamentos);
void emitirInformeDiario(int dia, const double ventasDiarias[][2]);
void generarInformeMensual(const double ventasDiarias[][2], char* buffer);
void listarStockCritico(const int codigos[], const char nombres[][MAX_NOMBRE], const int stocks[], const int stocksCriticos[], int n_medicamentos);
void finalizarMes(const double ventasDiarias[][2], char informesGuardados[][MAX_LEN_INFORME], int* contadorInformes);
void reiniciarDatosVentas(double ventasDiarias[][2], char nombresReceta[][MAX_NOMBRE], int cantidadesReceta[], long long dnisReceta[], int* contadorVentasReceta);
int autenticarDueno();
void inicializarDatos(int codigos[], char nombres[][MAX_NOMBRE], double precios[], int stocks[], int esVentaLibre[], int stocksCriticos[], int* n_medicamentos);

// ======== Funcion Principal ========
int main() {
    // ... (El cuerpo de main() no cambia respecto a la version anterior) ...
    int codigos[MAX_MEDICAMENTOS];
    char nombres[MAX_MEDICAMENTOS][MAX_NOMBRE];
    double precios[MAX_MEDICAMENTOS];
    int stocks[MAX_MEDICAMENTOS];
    int esVentaLibre[MAX_MEDICAMENTOS];
    int stocksCriticos[MAX_MEDICAMENTOS];
    int n_medicamentos = 0;

    double ventasDiarias[MAX_DIAS_MES][2] = {0.0};
    char nombresReceta[MAX_VENTAS_RECETA][MAX_NOMBRE];
    int cantidadesReceta[MAX_VENTAS_RECETA];
    long long dnisReceta[MAX_VENTAS_RECETA];
    int contadorVentasReceta = 0;

    char informesGuardados[MAX_INFORMES][MAX_LEN_INFORME];
    int contadorInformes = 0;

    inicializarDatos(codigos, nombres, precios, stocks, esVentaLibre, stocksCriticos, &n_medicamentos);
    ordenarMedicamentosPorCodigo(codigos, nombres, precios, stocks, esVentaLibre, stocksCriticos, n_medicamentos);

    int opcion;
    do {
        limpiarPantalla();
        mostrarMenu();
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        limpiar_buffer_stdin(); // REEMPLAZO: de while(getchar...)

        switch (opcion) {
            case 1: {
                int dia;
                printf("Ingrese el dia del mes (1-31): ");
                scanf("%d", &dia);
                limpiar_buffer_stdin();
                if (dia >= 1 && dia <= MAX_DIAS_MES) {
                    registrarVenta(dia, codigos, nombres, precios, stocks, esVentaLibre, ventasDiarias, nombresReceta, cantidadesReceta, dnisReceta, &contadorVentasReceta, n_medicamentos);
                } else {
                    printf("Dia invalido.\n");
                }
                esperarEnter();
                break;
            }
            case 2:
                consultarStock(codigos, nombres, stocks, n_medicamentos);
                esperarEnter();
                break;
            case 3: {
                int dia;
                printf("Ingrese el dia del mes para el informe (1-31): ");
                scanf("%d", &dia);
                limpiar_buffer_stdin();
                 if (dia >= 1 && dia <= MAX_DIAS_MES) {
                    emitirInformeDiario(dia, ventasDiarias);
                } else {
                    printf("Dia invalido.\n");
                }
                esperarEnter();
                break;
            }
            case 4: {
                char buffer[MAX_LEN_INFORME];
                generarInformeMensual(ventasDiarias, buffer);
                printf("%s", buffer);
                esperarEnter();
                break;
            }
            case 5:
                if (autenticarDueno()) {
                    listarStockCritico(codigos, nombres, stocks, stocksCriticos, n_medicamentos);
                }
                esperarEnter();
                break;
            case 6:
                finalizarMes(ventasDiarias, informesGuardados, &contadorInformes);
                esperarEnter();
                break;
            case 7:
                reiniciarDatosVentas(ventasDiarias, nombresReceta, cantidadesReceta, dnisReceta, &contadorVentasReceta);
                esperarEnter();
                break;
            case 8:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
                esperarEnter();
        }
    } while (opcion != 8);
    return 0;
}


// ======== Implementacion de Funciones Personalizadas (NUEVO) ========

/**
 * Copia una cadena de origen a destino.
 */
void mi_strcpy(char* destino, const char* origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

/**
 * Concatena la cadena de origen al final de la de destino.
 */
void mi_strcat(char* destino, const char* origen) {
    int i = 0;
    while (destino[i] != '\0') {
        i++;
    }
    int j = 0;
    while (origen[j] != '\0') {
        destino[i + j] = origen[j];
        j++;
    }
    destino[i + j] = '\0';
}

/**
 * Convierte un caracter a minuscula.
 */
char mi_tolower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

/**
 * Limpia el buffer de entrada estandar (stdin).
 */
void limpiar_buffer_stdin() {
    int c;
    while ((c = fgetc(stdin)) != '\n' && c != EOF);
}

/**
 * Lee una linea de texto desde stdin de forma segura.
 */
void leer_linea(char* buffer, int tamano) {
    int i = 0;
    int c;
    while (i < tamano - 1 && (c = fgetc(stdin)) != '\n' && c != EOF) {
        buffer[i++] = (char)c;
    }
    buffer[i] = '\0';
    // Si la linea era mas larga que el buffer, limpiar el resto
    if (c != '\n' && c != EOF) {
        limpiar_buffer_stdin();
    }
}


// ======== Implementacion de Funciones del Sistema (MODIFICADAS) ========

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void esperarEnter() {
    printf("\nPresione Enter para continuar...");
    limpiar_buffer_stdin();
}

void inicializarDatos(int codigos[], char nombres[][MAX_NOMBRE], double precios[], int stocks[], int esVentaLibre[], int stocksCriticos[], int* n_medicamentos) {
    codigos[0] = 101; mi_strcpy(nombres[0], "Paracetamol 500mg"); precios[0] = 150.50; stocks[0] = 100; esVentaLibre[0] = 1; stocksCriticos[0] = 20;
    codigos[1] = 205; mi_strcpy(nombres[1], "Amoxicilina 875mg"); precios[1] = 850.00; stocks[1] = 50; esVentaLibre[1] = 0; stocksCriticos[1] = 10;
    codigos[2] = 103; mi_strcpy(nombres[2], "Ibuprofeno 400mg"); precios[2] = 250.75; stocks[2] = 120; esVentaLibre[2] = 1; stocksCriticos[2] = 25;
    codigos[3] = 310; mi_strcpy(nombres[3], "Losartan 50mg"); precios[3] = 1200.00; stocks[3] = 30; esVentaLibre[3] = 0; stocksCriticos[3] = 5;
    *n_medicamentos = 4;
}

void mostrarMenu() {
    printf("===== Farmacia 'El Buen C' =====\n");
    printf("1. Registrar Venta\n");
    printf("2. Consultar Stock General\n");
    printf("3. Emitir Informe de Ventas Diario\n");
    printf("4. Emitir Informe de Ventas Mensual (Vista Previa)\n");
    printf("5. Listar Stock Critico (Solo Dueno)\n");
    printf("6. Finalizar Mes (Genera y 'Guarda' Informe Mensual)\n");
    printf("7. Reiniciar Mes (Borra datos de ventas)\n");
    printf("8. Salir\n");
    printf("==================================\n");
}


void ordenarMedicamentosPorCodigo(int codigos[], char nombres[][MAX_NOMBRE], double precios[], int stocks[], int esVentaLibre[], int stocksCriticos[], int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        int codigoActual = codigos[i];
        char nombreActual[MAX_NOMBRE]; mi_strcpy(nombreActual, nombres[i]);
        double precioActual = precios[i];
        int stockActual = stocks[i];
        int esVentaLibreActual = esVentaLibre[i];
        int stockCriticoActual = stocksCriticos[i];
        j = i - 1;
        while (j >= 0 && codigos[j] > codigoActual) {
            codigos[j + 1] = codigos[j];
            mi_strcpy(nombres[j + 1], nombres[j]);
            precios[j + 1] = precios[j];
            stocks[j + 1] = stocks[j];
            esVentaLibre[j + 1] = esVentaLibre[j];
            stocksCriticos[j + 1] = stocksCriticos[j];
            j = j - 1;
        }
        codigos[j + 1] = codigoActual;
        mi_strcpy(nombres[j + 1], nombreActual);
        precios[j + 1] = precioActual;
        stocks[j + 1] = stockActual;
        esVentaLibre[j + 1] = esVentaLibreActual;
        stocksCriticos[j + 1] = stockCriticoActual;
    }
}


int buscarMedicamentoPorNombre(const char* nombre, const char nombres[][MAX_NOMBRE], int n) {
    char nombreBusqueda[MAX_NOMBRE];
    char nombreActual[MAX_NOMBRE];
    mi_strcpy(nombreBusqueda, nombre);
    for(int i = 0; nombreBusqueda[i]; i++) nombreBusqueda[i] = mi_tolower(nombreBusqueda[i]);
    for (int i = 0; i < n; i++) {
        mi_strcpy(nombreActual, nombres[i]);
        for(int j = 0; nombreActual[j]; j++) nombreActual[j] = mi_tolower(nombreActual[j]);
        if (strstr(nombreActual, nombreBusqueda) != NULL) return i;
    }
    return -1;
}

void registrarVenta(int dia, int codigos[], const char nombres[][MAX_NOMBRE], double precios[], int stocks[], const int esVentaLibre[], double ventasDiarias[][2], char nombresReceta[][MAX_NOMBRE], int cantidadesReceta[], long long dnisReceta[], int* contadorVentasReceta, int n_medicamentos) {
    int tipoBusqueda;
    int idx = -1;

    printf("Buscar medicamento por: 1. Codigo, 2. Nombre\nOpcion: ");
    scanf("%d", &tipoBusqueda);
    limpiar_buffer_stdin();

    while (1) {
        if (tipoBusqueda == 1) {
            int codigo;
            printf("Ingrese el codigo del medicamento (o 0 para volver): ");
            scanf("%d", &codigo);
            limpiar_buffer_stdin();
            if (codigo == 0) return;
            idx = buscarMedicamentoPorCodigo(codigo, codigos, n_medicamentos);
        } else if (tipoBusqueda == 2) {
            char nombre[MAX_NOMBRE];
            printf("Ingrese el nombre del medicamento (o 'salir' para volver): ");
            leer_linea(nombre, MAX_NOMBRE);
            if (strcmp(nombre, "salir") == 0) return;
            idx = buscarMedicamentoPorNombre(nombre, nombres, n_medicamentos);
        } else {
            printf("Opcion de busqueda no valida.\n");
            return;
        }

        if (idx != -1) break;
        else {
            printf("\n*** Medicamento no encontrado. ***\n");
            listarMedicamentosDisponibles(codigos, nombres, n_medicamentos);
            printf("Por favor, intente de nuevo.\n");
        }
    }

    int* p_stock = &stocks[idx];
    printf("\nMedicamento seleccionado: %s | Stock actual: %d\n", nombres[idx], *p_stock);
    
    int cantidad;
    printf("Ingrese la cantidad a vender: ");
    scanf("%d", &cantidad);
    limpiar_buffer_stdin();

    if (cantidad <= 0) {
        printf("Cantidad no valida.\n");
        return;
    }
    if (*p_stock >= cantidad) {
        *p_stock -= cantidad;
        double totalVenta = cantidad * precios[idx];
        ventasDiarias[dia - 1][0] += totalVenta;
        ventasDiarias[dia - 1][1]++;
        printf("Venta registrada exitosamente. Total: $%.2f\n", totalVenta);
        printf("Nuevo stock de %s: %d\n", nombres[idx], *p_stock);
        if (esVentaLibre[idx] == 0) {
            if (*contadorVentasReceta < MAX_VENTAS_RECETA) {
                printf("Este medicamento requiere receta. Ingrese DNI del comprador: ");
                scanf("%lld", &dnisReceta[*contadorVentasReceta]);
                limpiar_buffer_stdin();
                mi_strcpy(nombresReceta[*contadorVentasReceta], nombres[idx]);
                cantidadesReceta[*contadorVentasReceta] = cantidad;
                (*contadorVentasReceta)++;
                printf("Datos de receta guardados.\n");
            } else {
                printf("ADVERTENCIA: Limite de registro de ventas con receta alcanzado.\n");
            }
        }
    } else {
        printf("Stock insuficiente. Solo hay %d unidades disponibles.\n", *p_stock);
    }
}


void generarInformeMensual(const double ventasDiarias[][2], char* buffer) {
    double totalMes = 0;
    int totalVentasMes = 0;
    char linea[100];
    
    limpiarPantalla();
    mi_strcpy(buffer, "========= INFORME MENSUAL DE VENTAS =========\n");
    mi_strcat(buffer, "Dia       |  Ventas ($)  | Cant. Transacciones\n");
    mi_strcat(buffer, "---------------------------------------------\n");
    for (int i = 0; i < MAX_DIAS_MES; i++) {
        if (ventasDiarias[i][0] > 0 || ventasDiarias[i][1] > 0) {
            sprintf(linea, "Dia %-5d | $%11.2f | %-19d\n", i + 1, ventasDiarias[i][0], (int)ventasDiarias[i][1]);
            mi_strcat(buffer, linea);
            totalMes += ventasDiarias[i][0];
            totalVentasMes += (int)ventasDiarias[i][1];
        }
    }
    mi_strcat(buffer, "---------------------------------------------\n");
    sprintf(linea, "TOTAL MES | $%11.2f | %-19d\n", totalMes, totalVentasMes);
    mi_strcat(buffer, linea);
    mi_strcat(buffer, "=============================================\n");
}


void finalizarMes(const double ventasDiarias[][2], char informesGuardados[][MAX_LEN_INFORME], int* contadorInformes) {
    if (*contadorInformes < MAX_INFORMES) {
        printf("Generando informe mensual final...\n");
        generarInformeMensual(ventasDiarias, informesGuardados[*contadorInformes]);
        (*contadorInformes)++;
        printf("Informe del mes finalizado y 'guardado' en memoria.\n");
        printf("Se recomienda reiniciar los datos de ventas para el nuevo mes.\n");
    } else {
        printf("Error: No hay mas espacio para guardar informes en memoria.\n");
    }
}

int autenticarDueno() {
    int pin;
    printf("Esta opcion requiere autenticacion. Ingrese el PIN de dueno: ");
    scanf("%d", &pin);
    limpiar_buffer_stdin();
    if (pin == PIN_DUENO) {
        return 1;
    } else {
        printf("PIN incorrecto. Acceso denegado.\n");
        return 0;
    }
}

// Las funciones buscarMedicamentoPorCodigo, listarMedicamentosDisponibles, consultarStock,
// emitirInformeDiario, listarStockCritico y reiniciarDatosVentas no requieren cambios
// ya que no usaban las funciones prohibidas. Se omiten por brevedad pero deben
// estar en el archivo final. Aquí se incluye una de ellas como ejemplo:

int buscarMedicamentoPorCodigo(int codigo, const int codigos[], int n) {
    int izquierda = 0, derecha = n - 1;
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        if (codigos[medio] == codigo) return medio;
        if (codigos[medio] < codigo) izquierda = medio + 1;
        else derecha = medio - 1;
    }
    return -1;
}

void listarMedicamentosDisponibles(const int codigos[], const char nombres[][MAX_NOMBRE], int n_medicamentos) {
    printf("\n--- Medicamentos Disponibles ---\n");
    printf("%-10s %-30s\n", "Codigo", "Nombre");
    printf("----------------------------------------\n");
    for (int i = 0; i < n_medicamentos; i++) {
        printf("%-10d %-30s\n", codigos[i], nombres[i]);
    }
    printf("----------------------------------------\n");
}

void consultarStock(const int codigos[], const char nombres[][MAX_NOMBRE], const int stocks[], int n_medicamentos) {
    printf("\n--- Estado de Stock Actual ---\n");
    printf("%-10s %-30s %-10s\n", "Codigo", "Nombre", "Stock");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < n_medicamentos; i++) {
        printf("%-10d %-30s %-10d\n", codigos[i], nombres[i], stocks[i]);
    }
    printf("--------------------------------------------------\n");
}

void emitirInformeDiario(int dia, const double ventasDiarias[][2]) {
    limpiarPantalla();
    printf("\n--- Informe de Ventas del Dia %d ---\n", dia);
    printf("Total recaudado: $%.2f\n", ventasDiarias[dia - 1][0]);
    printf("Cantidad de ventas: %d\n", (int)ventasDiarias[dia - 1][1]);
    printf("-------------------------------------\n");
}

void listarStockCritico(const int codigos[], const char nombres[][MAX_NOMBRE], const int stocks[], const int stocksCriticos[], int n_medicamentos) {
    printf("\n--- Medicamentos con Stock Critico ---\n");
    printf("%-30s %-10s %-10s\n", "Nombre", "Stock", "Critico");
    printf("--------------------------------------------------\n");
    int encontrados = 0;
    for (int i = 0; i < n_medicamentos; i++) {
        if (stocks[i] < stocksCriticos[i]) {
            printf("%-30s %-10d %-10d\n", nombres[i], stocks[i], stocksCriticos[i]);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("No hay medicamentos con stock por debajo del critico.\n");
    }
    printf("--------------------------------------------------\n");
}

void reiniciarDatosVentas(double ventasDiarias[][2], char nombresReceta[][MAX_NOMBRE], int cantidadesReceta[], long long dnisReceta[], int* contadorVentasReceta) {
    printf("Reiniciando datos de ventas del mes...\n");
    for (int i = 0; i < MAX_DIAS_MES; i++) {
        ventasDiarias[i][0] = 0.0;
        ventasDiarias[i][1] = 0.0;
    }
    *contadorVentasReceta = 0;
    printf("Datos de ventas reiniciados. Listo para un nuevo mes.\n");
}