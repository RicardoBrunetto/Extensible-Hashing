CC=gcc
CFLAGS=-w
LDFLAGS=
SOURCES= main.c ehashing.c
# HEADERS=$(SOURCES:.c=.h)
HEADERS=$(DIR)*.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE= main

all: Clear

main: $(OBJECTS)
	@$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)
	@echo "Sucesso incrivel!"

Clear: main
	@del *.o
