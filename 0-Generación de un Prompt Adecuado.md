necesito un prompt para gemini, que me ayude a resolver este ejercicio:

II.	Ejercicio
Me pide el dueño de una farmacia que le haga un sistema para administrarla. En la farmacia se venden diferentes tipos de productos y servicios como: medicamentos bajo receta, medicamentos de venta libre, perfumería, se colocan inyecciones, se toma la presión, etc.
Él necesita poder llevar el control de stock y la facturación de los medicamentos. Cada medicamento tiene un código, un nombre, un precio, una cantidad en stock y un ítem que indica si es de venta libre o no, ya que el procedimiento de venta es diferente en cada caso.
También nos dice que por cuestiones de seguridad no podemos utilizar archivos ni registros (estructuras, sean o no definidas con typedef), dice que él entiende que si se corta la luz perdería todas la información, pero que no nos preocupemos ya que dispone de UPs y un generador de electricidad a gas y que es a gas y no a nafta ya que se lo recomendaron por tener menor probabilidad de falla.
Nuestro sistema debe permitirle registrar cada venta durante el mes en curso, al finalizar cada mes se debe emitir un informe, con el total de las ventas en pesos y en cantidad de ventas por día como así también el total mensual, este informe se guarda en una carpeta.
Al comenzar un nuevo mes el sistema debe permitir borrar todos los datos de ventas y comenzar nuevamente.
También deberá permitir emitir informes a demanda, en cualquier momento, sobre las ventas de cualquier día y el estado de stock de los medicamentos.
Respecto al stock, el sistema debe disponer de una opción que permita saber cuáles medicamentos están por debajo del stock crítico, para poder encargar su compra, el stock crítico varía según cada medicamento y ese es un dato a tener en cuenta.
Estas últimas dos opciones deben permitirse ejecutar solo si quien lo solicita es el dueño de la farmacia.
Además, cuando se vende un medicamento bajo receta se debe guardar el nombre del medicamento, la cantidad vendida y el DNI del comprador, porque en caso de que algún inspector o el comprador lo solicite se deben informar estos datos. Debe ser con codigo en C, solo utilizando: 📓 [Cap. 2] Vectores
📓 [Cap. 2] Algoritmos de ordenamiento y búsqueda, costo computacional
📓 [Cap. 3] Manejo de memoria: Punteros
📓 [Cap. 4] Matrices: Introducción a Arreglos N-dimensionales
 

 Respuesta:

 Perfecto — te dejo un **prompt listo para pegar en Gemini** (en español) y pedirle que diseñe y entregue la solución completa al enunciado. Incluye restricciones, formato de salida requerido y criterios de evaluación. Copialo tal cual:

---

Eres un asistente experto en programación en C. Te daré el enunciado de un ejercicio y quiero que me devuelvas **una solución completa y didáctica** que cumpla estrictamente las restricciones indicadas.

**Enunciado (resumido):**
Desarrollar un sistema en C para administrar una farmacia. Debe controlar stock y facturación de medicamentos. Cada medicamento tiene: código, nombre, precio, cantidad en stock y un indicador si es de venta libre (OTC) o requiere receta. El sistema registra las ventas del mes en curso; al finalizar el mes debe emitir un informe con: total de ventas en pesos, cantidad de ventas por día y total mensual; ese informe "se guarda en una carpeta". Al iniciar nuevo mes se deben borrar los datos de ventas. Debe permitir emitir informes a demanda sobre las ventas de cualquier día y el estado de stock. Debe disponer de una opción para listar medicamentos por debajo del stock crítico (cada medicamento tiene su stock crítico). Estas dos últimas opciones (informes a demanda y listado stock crítico) sólo pueden ejecutar el dueño de la farmacia. Además, cuando se vende un medicamento con receta se debe guardar: nombre del medicamento, cantidad vendida y DNI del comprador (para poder informar ante requerimiento). **NO** se pueden utilizar archivos ni registros/structs/typedefs.
El programa debe estar escrito **en lenguaje C** y sólo usar las técnicas y estructuras indicadas más abajo.

**Restricciones técnicas obligatorias (solo se permite usar):**

* Cap.2: Vectores (arrays unidimensionales).
* Cap.2: Algoritmos de ordenamiento y búsqueda (implementados manualmente). Indica costo computacional (notación O).
* Cap.3: Manejo de memoria: punteros (usar punteros para acceso y manipulación cuando corresponda).
* Cap.4: Matrices (arreglos n-dimensionales) para almacenar datos tabulares (ej.: ventas por día).
* **Prohibido:** usar `struct`, `typedef` o cualquier forma de registros/objetos; **prohibido** usar archivos / file I/O (`fopen`, `fprintf`, etc.).
* El código debe compilar con `gcc` estándar (c99 o c11). Evitar dependencias externas.

**Requisitos funcionales y de interfaz:**

1. Menú interactivo por consola con opciones claras: registrar venta, consultar stock, informe diario, informe mensual, listar stock crítico (owner only), emitir informe a demanda por día (owner only), finalizar mes (genera informe mensual y "guarda" informe), reiniciar mes (borrar ventas), salir.
2. Registro de medicamentos (puede inicializarse con un conjunto fijo en arrays). Cada medicamento: código (entero), nombre (cadena), precio (float/double), stock (int), esOTC (int 0/1), stockCrítico (int).
3. Al registrar una venta: elegir medicamento por código o búsqueda por nombre; verificar stock; reducir stock; almacenar la venta en las estructuras de venta. Para ventas con receta: pedir DNI comprador y guardar registro asociado (nombre del medicamento, cantidad vendida, DNI).
4. Registro de ventas por día: modelo el mes con 31 días máximo (usar matriz/arrays para ventas por día). Poder registrar múltiples ventas en un mismo día (usar matrices/arrays y contadores).
5. Informes:

   * Informe diario: total en pesos y cantidad de ventas del día (detalle opcional de ítems).
   * Informe mensual: total por día y total del mes; además cantidad de ventas por día; **al finalizar mes** ese informe debe "guardarse en una carpeta" — dado que no se permiten archivos, **simular** el guardado manteniendo el informe en memoria (ej.: guardarlo como cadena en una matriz de strings o en una estructura equivalente con arrays) e imprimir instrucciones claras en comentarios sobre cómo habilitar file I/O si se permite en otra versión.
6. Restricción de propietario: para las opciones "listar stock crítico" y "emitir informe a demanda", solicitar autenticación por contraseña (por ejemplo, un PIN simple que se inicializa al comienzo). No usar archivos para persistir el PIN.
7. Proveer opción para borrar todos los datos de ventas al comenzar nuevo mes (reset arrays correspondientes).
8. Funciones separadas y comentadas: búsqueda (lineal y binaria si ordenas), ordenamiento (ej.: quicksort o mergesort o inserción si el dataset es pequeño), registro de venta, cálculo de totales, generación de informes, autenticación, etc. En cada función indica el costo en notación O().

**Formato de salida requerido de Gemini (ordenado, explícito):**

1. **Código C completo** listo para compilar (`gcc -std=c11 archivo.c -o programa`). Comentarios explicativos en cada bloque. No uses `struct` ni archivos. Usa arrays, matrices y punteros.
2. **Explicación paso a paso** de la arquitectura elegida (cómo representaste medicamentos, ventas por día, ventas con receta, cómo simulaste “guardar” informes sin archivos).
3. **Lista de funciones** con firma, propósito y costo computacional (O(...)).
4. **Ejemplo de ejecución (input/output)**: mostrar un run de consola que incluya: registrar 3 medicamentos, varias ventas (incluyendo receta con DNI), consulta de informe diario, informe mensual final y reinicio de mes. Incluye la salida esperada.
5. **Pruebas unitarias simples** o pruebas manuales sugeridas (casos borde: venta sin stock suficiente, búsqueda fallo, intento de opción owner con PIN incorrecto, mes con 30/31 días).
6. **Sugerencia breve** (3-5 pasos) indicando los cambios mínimos necesarios para que la versión que **sí** use archivos guarde los informes en una carpeta (qué funciones tocar, dónde usar `fopen`, formato del nombre del archivo con fecha).
7. **Nota final** que confirme que el código cumple las restricciones (no structs, no files), y una advertencia clara sobre la limitación de persistencia (si se corta la energía se pierde todo), y cómo mitigarlo en producción (UPS/generador — respetar lo indicado en el enunciado).

**Criterios de evaluación / requisitos de calidad:**

* Código legible y comentado.
* Cumplimiento estricto de las restricciones (sin `struct`, sin archivos).
* Uso demostrable de algoritmos de búsqueda/ordenamiento y punteros.
* Complejidad computacional indicada donde aplique.
* Menú y UX de consola claros para un usuario no técnico.
* Código que compile sin warnings con `-Wall -Wextra` preferible.

---

Pega ese prompt en Gemini y pídele que te devuelva exactamente lo pedido (1 a 7 del “Formato de salida requerido”). Si querés, yo también puedo escribirte ese código C aquí mismo — decime si querés que lo haga yo ahora (lo entregaré siguiendo exactamente las mismas condiciones).
------------------------------------------------------------------------------------------

// Se lo paso a Gemini y me devuelve el Prompt 1, adjunto en esta carpeta. En un nuevo chat, utilizo el prompt 1 y luego itero sobre ese mismo chat para ajustar el codigo. //
