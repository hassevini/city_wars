# Nome do executável
TARGET = city_wars

# Diretórios
SRC_DIR = src
INC_DIR = include

# Lista de arquivos-fonte (todos os .c em src/)
SRC = $(wildcard $(SRC_DIR)/*.c)

# PKG-CONFIG
PKG = allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5
CFLAGS = -g -I$(INC_DIR) $(shell pkg-config $(PKG) --cflags)
LDFLAGS = $(shell pkg-config $(PKG) --libs)

# Regra padrão
all:
	gcc $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Limpeza
clean:
	rm -f $(TARGET)
