CC = gcc
CFLAGS = -Wall -Wextra -Isrc

SRC = src/*.c
OUT = bin/json-parser

all:
	mkdir -p bin
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)