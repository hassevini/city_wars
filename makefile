TARGET = teste
SRC = main.c character.c joystick.c gun.c bullet.c

PKG = allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5
CFLAGS = $(shell pkg-config $(PKG) --cflags)
LDFLAGS = $(shell pkg-config $(PKG) --libs)

all:
	gcc $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
