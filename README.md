# Compilador Musical

Este proyecto implementa un compilador para un lenguaje de notación musical personalizado que traduce a formato ABC estándar. El compilador está desarrollado en C++ y utiliza las herramientas Flex y Bison para el análisis léxico y sintáctico.

## Lenguaje de Entrada y Traducción de Salida

### Lenguaje de Entrada (.mus)

Nuestro lenguaje de entrada es una notación musical intuitiva con las siguientes características:

1. **Declaraciones Obligatorias** (encabezado):
   - **Tempo**: Definido como "Tempo <valor>" donde el valor es el número de pulsaciones por minuto (BPM).
   - **Compás**: Definido como "Compas <numerador>/<denominador>" (por ejemplo, "Compas 4/4").
   - **Tonalidad**: Definida como "Tonalidad <nota> <modo>" donde:
     - <nota> puede ser en notación latina (Do, Re, Mi, Fa, Sol, La, Si) o inglesa (C, D, E, F, G, A, B).
     - <modo> puede ser "M" (mayor) o "m" (menor).

2. **Notas musicales**:
   - Formato: <nombre_nota><octava> <duración>
   - Ejemplos: "Do4 Negra", "Sol5 Corchea", "Fa#3 Blanca"
   - Soporta notas en notación latina e inglesa
   - Soporta alteraciones (sostenidos # y bemoles b)
   - Incluye octavas (0-9)

3. **Duraciones**:
   - Soporta "Blanca", "Negra", "Corchea" y "Semicorchea"

4. **Comentarios**:
   - Se pueden incluir comentarios con // (estilo C++)

### Formato de Salida (ABC)

La salida es un archivo en formato ABC, que es una notación textual estándar para música. Características principales:

1. **Cabecera ABC**:
   - X:1 (número de referencia)
   - T:LJ CERTIFIED TRANSLATION (título)
   - Q:1/4=<tempo> (tempo, donde 1/4 representa una negra)
   - M:<numerador>/<denominador> (compás)
   - K:<tonalidad> (tonalidad)

2. **Notación de notas**:
   - Las notas se representan con letras (C, D, E, F, G, A, B)
   - Duración: 1 (redonda), 2 (blanca), 1 (negra, por defecto), /2 (corchea), /4 (semicorchea)
   - Octavas: con apóstrofes (') para octavas superiores y comas (,) para inferiores
   - Alteraciones: ^ (sostenido), _ (bemol)

3. **Barras de compás**:
   - | (barra de compás)

## Estructura del Compilador

### 1. Scanner (Analizador Léxico)

Implementado con Flex en `scanner.flex`, el scanner convierte el texto de entrada en tokens.

```
Tokens principales:
- TOKEN_TEMPO, TOKEN_COMPAS, TOKEN_TONALIDAD (para declaraciones)
- TOKEN_BLANCA, TOKEN_NEGRA, TOKEN_CORCHEA, TOKEN_SEMICORCHEA (duraciones)
- TOKEN_NOTA_DO, TOKEN_NOTA_RE, etc. (notas musicales)
- TOKEN_MAYOR, TOKEN_MENOR (modos)
- TOKEN_NOTA_COMPLETA (formato nota+octava)
```

El scanner reconoce espacios, comentarios, identificadores y valores numéricos. Cada vez que encuentra un patrón válido, genera el token correspondiente que será procesado por el parser.

### 2. Parser y Construcción del AST

Implementado con Bison en `parser.bison`, el parser define la gramática del lenguaje y construye el Árbol de Sintaxis Abstracta (AST).

#### Gramática

```
programa: encabezado lista_notas
encabezado: tempo_decl compas_decl key_decl
nota_stmt: pitch_expr dur_expr
```

#### Estructura del AST

El AST está formado por diferentes tipos de nodos:

- **MusicProgram**: Nodo raíz que contiene todas las declaraciones y notas.
- **Declaraciones**:
  - `TempoDeclaration`: Almacena el valor del tempo.
  - `TimeSignatureDeclaration`: Almacena numerador y denominador del compás.
  - `KeyDeclaration`: Almacena la nota base y el modo (mayor/menor).
- **Expresiones**:
  - `NoteExpression`: Representa una nota musical con su octava.
  - `DurationExpression`: Representa la duración de una nota.
- **Statements**:
  - `NoteStatement`: Combina una nota y su duración.

El parser utiliza acciones semánticas que son instrucciones fijas en c++ para construir los nodos del AST mientras analiza la entrada. Utiliza la estructura auxiliar `Encabezado` para agrupar las tres declaraciones obligatorias.

### 3. Análisis Semántico

Implementado principalmente a través del método `resolve_names()`(que es una verificacion semantica exclusivamente) en cada clase del AST. Este análisis verifica:

#### Verificaciones Generales:
- Que todas las declaraciones obligatorias (tempo, compás, tonalidad) estén presentes.
- Que no haya declaraciones duplicadas.

#### Verificaciones Específicas:
- **Tempo**: Valor entre 20 y 200 BPM (rango válido musical).
- **Compás**: 
  - Numerador > 1 y ≤ 12
  - Denominador debe ser 2, 4, 8 o 16
- **Tonalidad**: 
  - Nota raíz debe ser válida
  - Modo debe ser mayor o menor
- **Notas**: 
  - Deben tener un nombre válido
  - La octava debe estar en un rango apropiado

El método `resolve_names()` utiliza una tabla de símbolos (`SymbolTable`) para registrar las declaraciones ya procesadas y evitar duplicaciones.

### 4. Generación de Código (Traducción a ABC)

Implementado a través del método `to_abc()` en cada clase del AST. Este proceso:

1. Genera la cabecera ABC (X, T, Q, M, K).
2. Traduce cada nota según las reglas de conversión de formato.
3. Inserta barras de compás automáticamente según el compás declarado.

#### Reglas de traducción:
- **Tempo**: `Q:1/4=<valor_tempo>`
- **Compás**: `M:<numerador>/<denominador>`
- **Tonalidad**: `K:<nota_base><modo>` donde modo es "maj" (mayor) o "min" (menor)
- **Notas**: 
  - Conversión de nombres (Do→C, Re→D, etc.)
  - Conversión de duración (Negra→1, Blanca→2, etc.)
  - Manejo de alteraciones (sostenidos, bemoles)
  - Manejo de octavas

## Uso del Compilador

1. Escribir el código musical en un archivo con extensión `.mus` encontraras un ejemplo ya elaborado que representa la melodia verano de vivaldi.
2. Compilar el proyecto: `make all`
3. Ejecutar el compilador: `./compilador_musical archivo.mus`
4. Se generará un archivo de salida `archivo.abc`

### Uso del Makefile en la carpeta lj_to_abc

La carpeta `lj_to_abc` contiene un Makefile especializado que automatiza todo el proceso de compilación, traducción y generación de archivos musicales. Con un solo comando, puedes:

1. Compilar el código fuente si es necesario
2. Traducir el archivo musical `.mus` a formato ABC
3. Generar automáticamente:
   - Un archivo PDF con la partitura musical
   - Un archivo OGG para reproducción de audio
   - Un archivo MIDI intermedio

Para usar esta funcionalidad:

1. Coloca tu archivo musical con extensión `.mus` en la carpeta `lj_to_abc` con el nombre `melodia.mus`
2. Desde la carpeta `lj_to_abc`, ejecuta:
   ```bash
   make
   ```
3. El proceso generará automáticamente:
   - `output.pdf`: La partitura musical en formato PDF
   - `output.ogg`: El archivo de audio en formato OGG
   - `output.mid`: El archivo MIDI intermedio

Para limpiar todos los archivos generados:
```bash
make clean
```

## Estructura de Archivos

- **Analizadores**:
  - `scanner.flex`: Análisis léxico (Flex)
  - `parser.bison`: Análisis sintáctico (Bison)

- **AST**:
  - `ast_node_interface.hpp/cpp`: Interfaz base para todos los nodos del AST
  - `declaration.hpp/cpp`: Declaraciones (tempo, compás, tonalidad)
  - `expression.hpp/cpp`: Expresiones (notas, duraciones)
  - `statement.hpp/cpp`: Statements (notas con duración)

- **Semántica y traducción**:
  - `symbol_table.hpp/cpp`: Tabla de símbolos para análisis semántico

- **Otros**:
  - `main.cpp`: Punto de entrada del programa
  - `Makefile`: Reglas de compilación

## Ejemplo

### Entrada (test.mus)
```
// Caso válido: Fragmento de Vivaldi - El Verano (Presto)
Tempo 180
Compas 3/4
Tonalidad Sol m

// Fragmento principal en semicorcheas
Re5 Semicorchea
Do5 Semicorchea
Si4 Semicorchea
La4 Semicorchea
Sol4 Semicorchea
Fa#4 Semicorchea
Mi4 Semicorchea
Re4 Semicorchea
Do4 Semicorchea
Si3  Semicorchea
La3 Semicorchea
Sol3 Semicorchea
Sol3 Semicorchea
Sol3 Semicorchea
// ... continua ...
```

### Salida (test.abc)
```
X:1
T:LJ CERTIFIED TRANSLATION
Q:1/4=180
M:3/4
L:1/8
K:Gmin
d/ c/ B/ A/ G/ ^F/ E/ D/ C/ B,/ A,/ G,/ G,/ G,/ | 
// ... continua ...
```

## Conceptos Teóricos Aplicados

Este compilador implementa las fases clásicas de compilación:

1. **Análisis léxico**: Convierte el texto fuente en tokens mediante Flex.
2. **Análisis sintáctico**: Organiza los tokens según la gramática mediante Bison.
3. **Análisis semántico**: Verifica que el programa sea musicalmente coherente.
4. **Generación de código**: Traduce el AST al formato ABC.

El diseño se basa en el patrón Visitor para recorrer y procesar el AST.

## Reproductor Musical

El compilador incluye una funcionalidad adicional de reproducción musical, que permite al usuario visualizar la composición realizada en una partitura en formato PDF y escucharla en audio con formato .OGG. Esta funcionalidad utiliza herramientas externas como:

- **abcm2ps**: Convierte el archivo ABC a formato PostScript
- **ps2pdf**: Convierte el archivo PostScript (GRAFICO VECTORIAL) a PDF
- **abc2midi**: Adapta el archivo ABC generado interfaz MIDI
- **timidity**: Reproduce el archivo ABC-MIDI generado

Porfavor descarga las siguientes bibliotecas para poder usar el script:

```
sudo apt update
sudo apt install abcmidi timidity abcm2ps ghostscript
```

Esta característica proporciona una experiencia completa al usuario, permitiéndole crear composiciones musicales a través del compilador y visualizarlas en una partitura profesional y escucharlas inmediatamente.

Diseñado Por:

Lorena Fernandez V-28.440.154
Julio Vasquez v-27.777.893

Gracias Totales...