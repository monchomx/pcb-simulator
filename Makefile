# Directorios
SRC_DIR   = src
BUILD_DIR = build
BIN_DIR   = bin
TEST_DIR  = tests
INC_DIR   = include

# Compilador y Flags
CC       = gcc
CFLAGS   = -Wall -g -I$(INC_DIR)
#-fexec-charset=UTF-8 
LDFLAGS  = -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2

# Archivos del Proyecto
# Filtramos main.c de los objetos de prueba para evitar conflictos de múltiples "main"
SOURCES  = $(wildcard $(SRC_DIR)/*.c)
OBJECTS  = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET   = $(BIN_DIR)/MyApp

# Archivos de Tests
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS    = $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%,$(TEST_SOURCES))

# Fuentes para tests (todos los .c excepto main.c)
TEST_SUPPORT_SOURCES = $(filter-out $(SRC_DIR)/main.c,$(SOURCES))

# Regla por defecto: Compilar la app principal
all: $(TARGET)

# Linkeo de la aplicación principal
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilación de objetos (.c -> .o)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para compilar y ejecutar los tests
# Se compilan automáticamente todos los tests del directorio tests/
TESTS = $(TEST_BINS)

test: $(TESTS)
	@echo "Running all unit tests..."
	@for t in $(TESTS); do $$t || exit 1; done

# Regla genérica para compilar cualquier test
$(BIN_DIR)/%: $(TEST_DIR)/%.c $(TEST_SUPPORT_SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(TEST_DIR)/$*.c $(TEST_SUPPORT_SOURCES) $(LDFLAGS)

# Limpieza
clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN_DIR)/*

# Ejecutar la aplicación
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean test run
