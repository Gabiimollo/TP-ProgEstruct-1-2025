#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para system("clear") / system("cls")
#include <ctype.h>  // Para tolower()

// ======== Constantes Globales ========
#define MAX_MEDICAMENTOS 50
#define MAX_DIAS_MES 31
#define MAX_NOMBRE 50
#define MAX_VENTAS_RECETA 100
#define MAX_INFORMES 12
#define MAX_LEN_INFORME 4096
#define PIN_DUEÑO 1234

// ======== Prototipos de Funciones ========

// --- Funciones de Utilidad y Búsqueda ---
void limpiarPantalla();
void esperarEnter();
int buscarMedicamentoPorCodigo(int codigo, const int codigos[], int n); // Búsqueda Binaria
int buscarMedicamentoPorNombre(const char* nombre, const char nombres[][MAX_NOMBRE], int n); // Búsqueda Lineal
void ordenarMedicamentosPorCodigo(int codigos[], char nombres[][MAX_NOMBRE], double precios[], int stocks[], int esVentaLibre[], int stocksCriticos[], int n); // Inserción

// --- Funciones del Menú Principal ---
void mostrarMenu(int esDueño);
void registrarVenta(int dia, int codigos[], const char nombres[][MAX_NOMBRE], double precios[], int stocks[], const int esVentaLibre[], double ventasDiarias[][2], char nombresReceta[][MAX_NOMBRE], int cantidadesReceta[], long long dnisReceta[], int* contadorVentasReceta, int n_medicamentos);
void consultarStock(const int codigos[], const char nombres[][MAX_NOMBRE], const int stocks[], int n_medicamentos);
void emitirInformeDiario(int dia, const double ventasDiarias[][2]);
void generarInformeMensual(const double ventasDiarias[][2], char* buffer);
void listarStockCritico(const int codigos[], const char nombres[][MAX_NOMBRE], const int stocks[], const int stocksCriticos[], int n_medicamentos);
void finalizarMes(const double ventasDiarias[][2], char informesGuardados[][MAX_LEN_INFORME], int* contadorInformes);
void reiniciarDatosVentas(double ventasDiarias[][2], char nombresReceta[][MAX_NOMBRE], int cantidadesReceta[], long long dnisReceta[], int* contadorVentasReceta);
int autenticarDueño();

// --- Funciones de Inicialización ---
void inicializarDatos(int codigos[], char nombres[][MAX_NOMBRE], double precios[], int stocks[], int esVentaLibre[], int stocksCriticos[], int* n_medicamentos);


// ======== Función Principal ========
int main() {
    // --- Estructuras de Datos (Arrays Paralelos) ---

    // Datos de Medicamentos
    int codigos[MAX_MEDICAMENTOS];
    char nombres[MAX_MEDICAMENTOS][MAX_NOMBRE];
    double precios[MAX_MEDICAMENTOS];
    int stocks[MAX_MEDICAMENTOS];
    int esVentaLibre[MAX_MEDICAMENTOS]; // 1 para Venta Libre (OTC), 0 para Receta
    int stocksCriticos[MAX_MEDICAMENTOS];
    int n_medicamentos = 0;

    // Datos de Ventas
    // Columna 0: Total $ del día. Columna 1: Cantidad de ventas del día.
    double ventasDiarias[MAX_DIAS_MES][2] = {0.0};

    // Datos de Ventas con Receta
    char nombresReceta[MAX_VENTAS_RECETA][MAX_NOMBRE];
    int cantidadesReceta[MAX_VENTAS_RECETA];
    long long dnisReceta[MAX_VENTAS_RECETA];
    int contadorVentasReceta = 0;

    // "Almacenamiento" de informes mensuales
    char informesGuardados[MAX_INFORMES][MAX_LEN_INFORME];
    int contadorInformes = 0;

    // --- Inicialización y Lógica Principal ---
    inicializarDatos(codigos, nombres, precios, stocks, esVentaLibre, stocksCriticos, &n_medicamentos);
    ordenarMedicamentosPorCodigo(codigos, nombres, precios, stocks, esVentaLibre, stocksCriticos, n_medicamentos); // Esencial para búsqueda binaria

    int opcion;
    do {
        limpiarPantalla();
        mostrarMenu(0); // Menú estándar
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        while(getchar() != '\n'); // Limpiar buffer de entrada

        switch (opcion) {
            case 1: {
                int dia;
                printf("Ingrese el día del mes (1-31): ");
                scanf("%d", &dia);
                if (dia >= 1 && dia <= MAX_DIAS_MES) {
                    registrarVenta(dia, codigos, nombres, precios, stocks, esVentaLibre, ventasDiarias, nombresReceta, cantidadesReceta, dnisReceta, &contadorVentasReceta, n_medicamentos);
                } else {
                    printf("Día inválido.\n");
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
                printf("Ingrese el día del mes para el informe (1-31): ");
                scanf("%d", &dia);
                 if (dia >= 1 && dia <= MAX_DIAS_MES) {
                    emitirInformeDiario(dia, ventasDiarias);
                } else {
                    printf("Día inválido.\n");
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
                if (autenticarDueño()) {
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
                printf("Opción no válida. Intente de nuevo.\n");
                esperarEnter();
        }
    } while (opcion != 8);

    return 0;
}

// ======== Implementación de Funciones ========

/**
 * @brief Limpia la pantalla de la consola.
 * @costo O(1)
 */
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * @brief Pausa la ejecución hasta que el usuario presione Enter.
 * @costo O(1)
 */
void esperarEnter() {
    printf("\nPresione Enter para continuar...");
    while(getchar() != '\n');
}

/**
 * @brief Inicializa los datos de los medicamentos en los arrays paralelos.
 * @costo O(1) - Porque los datos son fijos.
 */
void inicializarDatos(int codigos[], char nombres[][MAX_NOMBRE], double precios[], int stocks[], int esVentaLibre[], int stocksCriticos[], int* n_medicamentos) {
    // Medicamento 1
    codigos[0] = 101; strcpy(nombres[0], "Paracetamol 500mg"); precios[0] = 150.50; stocks[0] = 100; esVentaLibre[0] = 1; stocksCriticos[0] = 20;
    // Medicamento 2
    codigos[1] = 205; strcpy(nombres[1], "Amoxicilina 875mg"); precios[1] = 850.00; stocks[1] = 50; esVentaLibre[1] = 0; stocksCriticos[1] = 10;
    // Medicamento 3
    codigos[2] = 103; strcpy(nombres[2], "Ibuprofeno 400mg"); precios[2] = 250.75; stocks[2] = 120; esVentaLibre[2] = 1; stocksCriticos[2] = 25;
    // Medicamento 4
    codigos[3] = 310; strcpy(nombres[3], "Losartan 50mg"); precios[3] = 1200.00; stocks[3] = 30; esVentaLibre[3] = 0; stocksCriticos[3] = 5;

    *n_medicamentos = 4;
}

/**
 * @brief Muestra el menú de opciones.
 * @costo O(1)
 */
void mostrarMenu(int esDueño) {
    printf("===== Farmacia 'El Buen C' =====\n");
    printf("1. Registrar Venta\n");
    printf("2. Consultar Stock General\n");
    printf("3. Emitir Informe de Ventas Diario\n");
    printf("4. Emitir Informe de Ventas Mensual (Vista Previa)\n");
    printf("5. Listar Stock Crítico (Solo Dueño)\n");
    printf("6. Finalizar Mes (Genera y 'Guarda' Informe Mensual)\n");
    printf("7. Reiniciar Mes (Borra datos de ventas)\n");
    printf("8. Salir\n");
    printf("==================================\n");
}

/**
 * @brief Ordena los medicamentos por código usando Insertion Sort. Necesario para la búsqueda binaria.
 * @param ...punteros a los arrays paralelos.
 * @param n Cantidad de medicamentos.
 * @costo O(n^2) - Típico de Insertion Sort.
 */
void ordenarMedicamentosPorCodigo(int codigos[], char nombres[][MAX_NOMBRE], double precios[], int stocks[], int esVentaLibre[], int stocksCriticos[], int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        // Guardar el elemento actual a insertar
        int codigoActual = codigos[i];
        char nombreActual[MAX_NOMBRE];
        strcpy(nombreActual, nombres[i]);
        double precioActual = precios[i];
        int stockActual = stocks[i];
        int esVentaLibreActual = esVentaLibre[i];
        int stockCriticoActual = stocksCriticos[i];

        j = i - 1;
        // Mover los elementos mayores que el actual una posición hacia adelante
        while (j >= 0 && codigos[j] > codigoActual) {
            codigos[j + 1] = codigos[j];
            strcpy(nombres[j + 1], nombres[j]);
            precios[j + 1] = precios[j];
            stocks[j + 1] = stocks[j];
            esVentaLibre[j + 1] = esVentaLibre[j];
            stocksCriticos[j + 1] = stocksCriticos[j];
            j = j - 1;
        }
        // Insertar el elemento actual en su posición correcta
        codigos[j + 1] = codigoActual;
        strcpy(nombres[j + 1], nombreActual);
        precios[j + 1] = precioActual;
        stocks[j + 1] = stockActual;
        esVentaLibre[j + 1] = esVentaLibreActual;
        stocksCriticos[j + 1] = stockCriticoActual;
    }
}


/**
 * @brief Busca un medicamento por su código usando Búsqueda Binaria.
 * @param codigo El código a buscar.
 * @param codigos Array de códigos (debe estar ordenado).
 * @param n Tamaño del array.
 * @return El índice del medicamento si se encuentra, -1 en caso contrario.
 * @costo O(log n) - Eficiencia de la búsqueda binaria.
 */
int buscarMedicamentoPorCodigo(int codigo, const int codigos[], int n) {
    int izquierda = 0, derecha = n - 1;
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        if (codigos[medio] == codigo) {
            return medio; // Encontrado
        }
        if (codigos[medio] < codigo) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    return -1; // No encontrado
}

/**
 * @brief Busca un medicamento por nombre usando Búsqueda Lineal (case-insensitive).
 * @param nombre El nombre a buscar.
 * @param nombres Matriz de nombres.
 * @param n Cantidad de medicamentos.
 * @return El índice del medicamento si se encuentra, -1 en caso contrario.
 * @costo O(n) - Recorre el array secuencialmente.
 */
int buscarMedicamentoPorNombre(const char* nombre, const char nombres[][MAX_NOMBRE], int n) {
    char nombreBusqueda[MAX_NOMBRE];
    char nombreActual[MAX_NOMBRE];
    
    // Copiar para no modificar el original y convertir a minúsculas
    strcpy(nombreBusqueda, nombre);
    for(int i = 0; nombreBusqueda[i]; i++){
        nombreBusqueda[i] = tolower(nombreBusqueda[i]);
    }

    for (int i = 0; i < n; i++) {
        strcpy(nombreActual, nombres[i]);
        for(int j = 0; nombreActual[j]; j++){
            nombreActual[j] = tolower(nombreActual[j]);
        }
        // strstr busca una subcadena, útil si el usuario no pone el nombre completo
        if (strstr(nombreActual, nombreBusqueda) != NULL) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Registra una venta, actualizando stock y contadores de ventas.
 * @costo Peor caso O(n) si la búsqueda es por nombre. Mejor caso O(log n) si es por código.
 */
void registrarVenta(int dia, int codigos[], const char nombres[][MAX_NOMBRE], double precios[], int stocks[], const int esVentaLibre[], double ventasDiarias[][2], char nombresReceta[][MAX_NOMBRE], int cantidadesReceta[], long long dnisReceta[], int* contadorVentasReceta, int n_medicamentos) {
    int tipoBusqueda;
    int idx = -1;
    printf("Buscar medicamento por: 1. Código, 2. Nombre\nOpción: ");
    scanf("%d", &tipoBusqueda);
    while(getchar() != '\n');

    if (tipoBusqueda == 1) {
        int codigo;
        printf("Ingrese el código del medicamento: ");
        scanf("%d", &codigo);
        idx = buscarMedicamentoPorCodigo(codigo, codigos, n_medicamentos);
    } else if (tipoBusqueda == 2) {
        char nombre[MAX_NOMBRE];
        printf("Ingrese el nombre del medicamento: ");
        fgets(nombre, MAX_NOMBRE, stdin);
        nombre[strcspn(nombre, "\n")] = 0; // Eliminar el salto de línea
        idx = buscarMedicamentoPorNombre(nombre, nombres, n_medicamentos);
    } else {
        printf("Opción de búsqueda no válida.\n");
        return;
    }

    if (idx == -1) {
        printf("Medicamento no encontrado.\n");
        return;
    }

    // Usamos punteros para acceder a los datos del medicamento encontrado
    int* p_stock = &stocks[idx];
    double* p_precio = &precios[idx];
    const int* p_esVentaLibre = &esVentaLibre[idx];
    
    printf("Medicamento: %s | Stock actual: %d\n", nombres[idx], *p_stock);

    int cantidad;
    printf("Ingrese la cantidad a vender: ");
    scanf("%d", &cantidad);
    while(getchar() != '\n');

    if (cantidad <= 0) {
        printf("Cantidad no válida.\n");
        return;
    }

    if (*p_stock >= cantidad) {
        *p_stock -= cantidad; // Reducir stock usando el puntero
        double totalVenta = cantidad * (*p_precio);

        // Actualizar datos de ventas diarias
        ventasDiarias[dia - 1][0] += totalVenta;
        ventasDiarias[dia - 1][1]++;

        printf("Venta registrada exitosamente. Total: $%.2f\n", totalVenta);
        printf("Nuevo stock de %s: %d\n", nombres[idx], *p_stock);

        // Si requiere receta, guardar datos adicionales
        if (*p_esVentaLibre == 0) {
            if (*contadorVentasReceta < MAX_VENTAS_RECETA) {
                printf("Este medicamento requiere receta. Ingrese DNI del comprador: ");
                scanf("%lld", &dnisReceta[*contadorVentasReceta]);
                while(getchar() != '\n');

                strcpy(nombresReceta[*contadorVentasReceta], nombres[idx]);
                cantidadesReceta[*contadorVentasReceta] = cantidad;
                (*contadorVentasReceta)++;
                printf("Datos de receta guardados.\n");
            } else {
                printf("ADVERTENCIA: Límite de registro de ventas con receta alcanzado.\n");
            }
        }
    } else {
        printf("Stock insuficiente. Solo hay %d unidades disponibles.\n", *p_stock);
    }
}

/**
 * @brief Muestra el stock de todos los medicamentos.
 * @costo O(n) - Debe recorrer toda la lista de medicamentos.
 */
void consultarStock(const int codigos[], const char nombres[][MAX_NOMBRE], const int stocks[], int n_medicamentos) {
    printf("\n--- Estado de Stock Actual ---\n");
    printf("%-10s %-30s %-10s\n", "Código", "Nombre", "Stock");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < n_medicamentos; i++) {
        printf("%-10d %-30s %-10d\n", codigos[i], nombres[i], stocks[i]);
    }
    printf("--------------------------------------------------\n");
}

/**
 * @brief Muestra un informe de ventas para un día específico.
 * @costo O(1) - Acceso directo a la matriz.
 */
void emitirInformeDiario(int dia, const double ventasDiarias[][2]) {
    printf("\n--- Informe de Ventas del Día %d ---\n", dia);
    printf("Total recaudado: $%.2f\n", ventasDiarias[dia - 1][0]);
    printf("Cantidad de ventas: %d\n", (int)ventasDiarias[dia - 1][1]);
    printf("-------------------------------------\n");
}

/**
 * @brief Genera una cadena de texto con el informe mensual.
 * @param buffer Puntero a la cadena donde se guardará el informe.
 * @costo O(D) - Donde D es el número de días en el mes (constante, 31).
 */
void generarInformeMensual(const double ventasDiarias[][2], char* buffer) {
    double totalMes = 0;
    int totalVentasMes = 0;
    char linea[100];

    strcpy(buffer, "========= INFORME MENSUAL DE VENTAS =========\n");
    strcat(buffer, "Día       |  Ventas ($)  | Cant. Transacciones\n");
    strcat(buffer, "---------------------------------------------\n");

    for (int i = 0; i < MAX_DIAS_MES; i++) {
        if (ventasDiarias[i][0] > 0 || ventasDiarias[i][1] > 0) {
            sprintf(linea, "Día %-5d | $%11.2f | %-19d\n", i + 1, ventasDiarias[i][0], (int)ventasDiarias[i][1]);
            strcat(buffer, linea);
            totalMes += ventasDiarias[i][0];
            totalVentasMes += (int)ventasDiarias[i][1];
        }
    }
    strcat(buffer, "---------------------------------------------\n");
    sprintf(linea, "TOTAL MES | $%11.2f | %-19d\n", totalMes, totalVentasMes);
    strcat(buffer, linea);
    strcat(buffer, "=============================================\n");
}

/**
 * @brief Lista los medicamentos cuyo stock está por debajo del crítico.
 * @costo O(n) - Debe revisar cada medicamento.
 */
void listarStockCritico(const int codigos[], const char nombres[][MAX_NOMBRE], const int stocks[], const int stocksCriticos[], int n_medicamentos) {
    printf("\n--- Medicamentos con Stock Crítico ---\n");
    printf("%-30s %-10s %-10s\n", "Nombre", "Stock", "Crítico");
    printf("--------------------------------------------------\n");
    int encontrados = 0;
    for (int i = 0; i < n_medicamentos; i++) {
        // Uso de punteros para acceder a los valores
        const int* p_stock = &stocks[i];
        const int* p_stockCritico = &stocksCriticos[i];
        if (*p_stock < *p_stockCritico) {
            printf("%-30s %-10d %-10d\n", nombres[i], *p_stock, *p_stockCritico);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("No hay medicamentos con stock por debajo del crítico.\n");
    }
    printf("--------------------------------------------------\n");
}

/**
 * @brief "Guarda" el informe mensual en una matriz de strings en memoria.
 * @costo O(D) - Dominado por la generación del informe.
 */
void finalizarMes(const double ventasDiarias[][2], char informesGuardados[][MAX_LEN_INFORME], int* contadorInformes) {
    if (*contadorInformes < MAX_INFORMES) {
        printf("Generando informe mensual final...\n");
        generarInformeMensual(ventasDiarias, informesGuardados[*contadorInformes]);
        (*contadorInformes)++;
        printf("Informe del mes finalizado y 'guardado' en memoria.\n");
        printf("Se recomienda reiniciar los datos de ventas para el nuevo mes.\n");
    } else {
        printf("Error: No hay más espacio para guardar informes en memoria.\n");
    }
}

/**
 * @brief Reinicia a cero todas las estructuras de datos de ventas.
 * @costo O(D + R) - D=días, R=max ventas receta.
 */
void reiniciarDatosVentas(double ventasDiarias[][2], char nombresReceta[][MAX_NOMBRE], int cantidadesReceta[], long long dnisReceta[], int* contadorVentasReceta) {
    printf("Reiniciando datos de ventas del mes...\n");
    for (int i = 0; i < MAX_DIAS_MES; i++) {
        ventasDiarias[i][0] = 0.0;
        ventasDiarias[i][1] = 0.0;
    }
    for (int i = 0; i < *contadorVentasReceta; i++) {
        nombresReceta[i][0] = '\0';
        cantidadesReceta[i] = 0;
        dnisReceta[i] = 0;
    }
    *contadorVentasReceta = 0;
    printf("Datos de ventas reiniciados. Listo para un nuevo mes.\n");
}

/**
 * @brief Solicita un PIN y verifica si es correcto.
 * @return 1 si es correcto, 0 si no.
 * @costo O(1)
 */
int autenticarDueño() {
    int pin;
    printf("Esta opción requiere autenticación. Ingrese el PIN de dueño: ");
    scanf("%d", &pin);
    while(getchar() != '\n');
    if (pin == PIN_DUEÑO) {
        return 1;
    } else {
        printf("PIN incorrecto. Acceso denegado.\n");
        return 0;
    }
}