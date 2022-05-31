all:
	gcc -g -Wall -I/usr/include/libxml2 -o main main.c common.c loader.c commands/*.c -lm -lxml2

CC=gcc
CFLAGS=-g -Wall $(shell xml2-config --cflags)
LFLAGS=$(shell xml2-config --libs) -lm

SRC=./
OBJ=obj
SRCS=$(shell find $(SRC) -name "*.c")
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
HDRS=$(wildcard $(SRC)/*.h)
BIN=main

all: $(BIN)

$(BIN): $(OBJS) $(OBJ)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.c $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $@

clean:
	rm -r $(OBJ)
	rm $(BIN)
