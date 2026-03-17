# Minimal Makefile for OnlineCRpg with Raylib

CC = gcc
CFLAGS = -Wall -Wextra -g3
LDFLAGS = -lraylib -lwinmm -lgdi32 -lopengl32
SRC = \
	src/main.c \
	src/level/level.c \
	src/global.c \
	src/network/network.c \
	src/network/client.c \
	src/network/server.c \
	src/util/logger.c \
	src/network/packet.c \
	src/network/global_net.c \
	src/level/tile.c \
	src/util/assets.c \
	src/entites/entity.c
OUT = output/main.exe

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
