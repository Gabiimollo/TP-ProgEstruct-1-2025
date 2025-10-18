# Documentación del Sistema de Farmacia

Este documento detalla cada una de las funciones utilizadas en el sistema de gestión de farmacia en C.

---

## Funciones de Utilidad Personalizadas

### `void mi_strcpy(char* destino, const char* origen)`
- **Propósito:** Copia el contenido de la cadena `origen` en la cadena `destino`, incluyendo el carácter nulo final. Reemplaza a `strcpy()` de `<string.h>`.
- **Parámetros:**
    - `char* destino`: Puntero al buffer donde se copiará la cadena.
    - `const char* origen`: Puntero a la cadena que se va a copiar.
- **Retorno:** Nada.
- **Costo Computacional:** O(N), donde N es la longitud de la cadena `origen`.

### `void mi_strcat(char* destino, const char* origen)`
- **Propósito:** Concatena (añade) una copia de la cadena `origen` al final de la cadena `destino`. Reemplaza a `strcat()` de `<string.h>`.
- **Parámetros:**
    - `char* destino`: Puntero a la cadena destino, que debe tener suficiente espacio para el contenido de ambas cadenas.
    - `const char* origen`: Puntero a la cadena que se añadirá al final.
- **Retorno:** Nada.
- **Costo Computacional:** O(N+M), donde N es la longitud de `destino` y M es la longitud de `origen`.

### `char mi_tolower(char c)`
- **Propósito:** Convierte un carácter de mayúscula a minúscula. Si el carácter no es una letra mayúscula, lo devuelve sin cambios. Reemplaza a `tolower()` de `<ctype.h>`.
- **Parámetros:**
    - `char c`: El carácter a convertir.
- **Retorno:** El carácter en minúscula.
- **Costo Computacional:** O(1).

### `void limpiar_buffer_stdin()`
- **Propósito:** Consume y descarta todos los caracteres restantes en el búfer de entrada estándar (`stdin`) hasta encontrar un salto de línea o el fin de archivo. Es crucial para prevenir errores de lectura después de usar `scanf`.
- **Parámetros:** Ninguno.
- **Retorno:** Nada.
- **Costo Computacional:** O(N), donde N son los caracteres restantes en el búfer.

### `void leer_linea(char* buffer, int tamano)`
- **Propósito:** Lee una línea de texto desde `stdin` de forma segura, evitando desbordamientos de búfer. Reemplaza a `fgets`.
- **Parámetros:**
    - `char* buffer`: El arreglo donde se almacenará la línea leída.
    - `int tamano`: El tamaño máximo del `buffer`.
- **Retorno:** Nada.
- **Costo Computacional:** O(N), donde N es el número de caracteres leídos o el tamaño del búfer.

### `void limpiarPantalla()`
- **Propósito:** Limpia la pantalla de la terminal. Utiliza comandos específicos del sistema operativo (`cls` para Windows, `clear` para Linux/macOS).
- **Parámetros:** Ninguno.
- **Retorno:** Nada.
- **Costo Computacional:** O(1), aunque depende de la implementación del sistema operativo.

### `void esperarEnter()`
- **Propósito:** Pausa la ejecución del programa y muestra un mensaje solicitando al usuario que presione la tecla 'Enter' para continuar.
- **Parámetros:** Ninguno.
- **Retorno:** Nada.
- **Costo Computacional:** O(1).

---

## Funciones Principales del Sistema

### `void inicializarDatos(...)`
- **Propósito:** Carga en memoria el conjunto inicial de medicamentos con sus datos. Esta función se ejecuta una sola vez al iniciar el programa para tener un inventario base.
- **Parámetros:** Punteros a todos los arrays paralelos que almacenan la información de los medicamentos y un puntero al contador de medicamentos.
- **Retorno:** Nada.
- **Costo Computacional:** O(1), ya que la cantidad de datos a inicializar es fija en el código.

### `void ordenarMedicamentosPorCodigo(...)`
- **Propósito:** Ordena todos los arrays paralelos de medicamentos basándose en el código de manera ascendente. Utiliza el algoritmo de **Ordenamiento por Inserción (Insertion Sort)**. Este paso es crucial para que la búsqueda binaria funcione.
- **Parámetros:** Punteros a todos los arrays de medicamentos (`codigos`, `nombres`, `precios`, etc.) y el número total de medicamentos (`n`).
- **Retorno:** Nada.
- **Costo Computacional:** O(n²), característico del algoritmo de Inserción.

### `int buscarMedicamentoPorCodigo(int codigo, const int codigos[], int n)`
- **Propósito:** Busca el índice de un medicamento en el array `codigos` utilizando el algoritmo de **búsqueda binaria**. El array de códigos debe estar previamente ordenado.
- **Parámetros:**
    - `int codigo`: El código del medicamento a buscar.
    - `const int codigos[]`: El array de códigos.
    - `int n`: El número de medicamentos en el sistema.
- **Retorno:** El índice del medicamento si se encuentra; `-1` en caso contrario.
- **Costo Computacional:** O(log n).

### `int buscarMedicamentoPorNombre(const char* nombre, ...)`
- **Propósito:** Busca el índice de un medicamento por su nombre utilizando **búsqueda lineal**. La búsqueda no distingue mayúsculas de minúsculas y busca subcadenas.
- **Parámetros:**
    - `const char* nombre`: El nombre (o parte del nombre) a buscar.
    - `const char nombres[][]`: La matriz de nombres de medicamentos.
    - `int n`: El número de medicamentos.
- **Retorno:** El índice del primer medicamento que coincida; `-1` si no hay coincidencias.
- **Costo Computacional:** O(n * M), donde M es la longitud promedio de los nombres.

### `void registrarVenta(...)`
- **Propósito:** Orquesta el proceso completo de una venta. Permite al usuario buscar un medicamento, especificar la cantidad, validar el stock, actualizar las estructuras de datos de ventas y registrar la información de la receta si es necesario. Incluye un bucle de validación para reintentar si el usuario ingresa un dato inválido.
- **Costo Computacional:** Dominado por la búsqueda del medicamento, O(log n) o O(n).

### `void generarInformeMensual(..., char* buffer)`
- **Propósito:** Genera un informe de texto formateado con el resumen de ventas de todo el mes. El informe se guarda en una cadena de caracteres (`buffer`) en lugar de imprimirse directamente.
- **Costo Computacional:** O(1), ya que siempre itera un número fijo de días (31).

### `void emitirInformeDiario(int dia, ...)`
- **Propósito:** Muestra en pantalla un resumen de las ventas para un día específico, incluyendo el total recaudado y la cantidad de transacciones.
- **Parámetros:**
    - `int dia`: El día del mes (1-31) para el cual se quiere el informe.
    - `const double ventasDiarias[][]`: La matriz que contiene los datos de ventas.
- **Retorno:** Nada.
- **Costo Computacional:** O(1), ya que accede directamente a una posición de la matriz.

### `void consultarStock(...)`
- **Propósito:** Muestra una tabla formateada con el código, nombre y stock actual de todos los medicamentos.
- **Parámetros:** Los arrays de `codigos`, `nombres`, `stocks` y el número de medicamentos.
- **Retorno:** Nada.
- **Costo Computacional:** O(n).

### `void listarMedicamentosDisponibles(...)`
- **Propósito:** Muestra una lista simple con el código y el nombre de todos los medicamentos. Se usa para ayudar al usuario cuando comete un error.
- **Parámetros:**
    - `const int codigos[]`: El array de códigos.
    - `const char nombres[][]`: La matriz de nombres.
    - `int n_medicamentos`: El número de medicamentos.
- **Retorno:** Nada.
- **Costo Computacional:** O(n).

### `void listarStockCritico(...)`
- **Propósito:** Imprime una lista de medicamentos cuyo stock actual está por debajo de su nivel crítico.
- **Parámetros:** Los arrays de `codigos`, `nombres`, `stocks`, `stocksCriticos` y el número de medicamentos.
- **Retorno:** Nada.
- **Costo Computacional:** O(n).

### `void finalizarMes(...)`
- **Propósito:** Genera el informe mensual final y lo "guarda" en un array en memoria, simulando un almacenamiento persistente.
- **Parámetros:**
    - `const double ventasDiarias[][]`: La matriz con los datos de ventas.
    - `char informesGuardados[][]`: La matriz de strings donde se almacenará el informe.
    - `int* contadorInformes`: Puntero al contador de informes guardados.
- **Retorno:** Nada.
- **Costo Computacional:** O(1), ya que el costo principal es el de `generarInformeMensual`, que es constante.

### `void reiniciarDatosVentas(...)`
- **Propósito:** Restablece a cero todas las estructuras de datos de ventas para comenzar un nuevo mes.
- **Parámetros:** Punteros a las estructuras de datos de ventas y sus contadores.
- **Retorno:** Nada.
- **Costo Computacional:** O(1), porque itera sobre arrays de tamaño fijo.

### `int autenticarDueno()`
- **Propósito:** Solicita un PIN y lo compara con una constante para autorizar el acceso a funciones restringidas.
- **Retorno:** `1` si el PIN es correcto, `0` en caso contrario.
- **Costo Computacional:** O(1).

### `void mostrarMenu()`
- **Propósito:** Imprime en pantalla el menú principal con todas las opciones disponibles.
- **Parámetros:** Ninguno.
- **Retorno:** Nada.
- **Costo Computacional:** O(1).