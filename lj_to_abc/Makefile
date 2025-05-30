CXX = g++
CXXFLAGS = -Wall -std=c++17
COMPILER_PATH = ..
REPRODUCTOR_PATH = ../reproductor_musical

# Nombre del archivo de entrada
INPUT_FILE = melodia.mus

# Nombre del archivo de salida ABC (se generará automáticamente)
ABC_FILE = test.abc

all: generate_music

# Compilar el compilador musical si no existe
$(COMPILER_PATH)/compilador_musical:
	$(MAKE) -C $(COMPILER_PATH)

# Generar archivo ABC
$(ABC_FILE): $(COMPILER_PATH)/compilador_musical $(INPUT_FILE)
	$(COMPILER_PATH)/compilador_musical $(INPUT_FILE)
#cambia el nombre del archivo abc a test.abc
	mv melodia.abc test.abc
	sleep 2  # Esperar 2 segundos para asegurar que el archivo se haya generado

# Compilar el script de reproducción
$(REPRODUCTOR_PATH)/abc2midi_pdf:
	$(MAKE) -C $(REPRODUCTOR_PATH)

# Generar archivos musicales (PDF y OGG)
generate_music: $(ABC_FILE) $(REPRODUCTOR_PATH)/abc2midi_pdf
#copia el archivo abc a la carpeta del reproductor
	cp $(ABC_FILE) $(REPRODUCTOR_PATH)/../test.abc 
	cd $(REPRODUCTOR_PATH) && ./abc2midi_pdf
	cp $(REPRODUCTOR_PATH)/*.pdf .
	cp $(REPRODUCTOR_PATH)/*.ogg .
	cp $(REPRODUCTOR_PATH)/*.mid .

clean:
	rm -f *.midi *.pdf *.ps *.mid *.abc *.ogg test.abc
	$(MAKE) -C $(COMPILER_PATH) clean
	$(MAKE) -C $(REPRODUCTOR_PATH) clean

.PHONY: all generate_music clean 