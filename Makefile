# Minimal Makefile for OnlineCRpg with Raylib

CC = gcc
CFLAGS = -Wall -Wextra -g3
LDFLAGS = -lraylib -lwinmm -lgdi32 -lopengl32
SRC = src/main.c src/level/level.c src/global.c src/network/network.c
OUT = output/main.exe

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
