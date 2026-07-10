TARGET = prog
LIBS = -lm -lSDL3 -lSDL3_image
CC = gcc
CFLAGS = -g -Wall -std=c99 -lSDL3 -DSDL_MAIN_HANDLED -mwindows

.PHONY: default all clean

default: $(TARGET)
all: default

SOURCEDIR = ./src/
OBJECTS = $(patsubst $(SOURCEDIR)%.c, $(SOURCEDIR)%.o, $(wildcard $(SOURCEDIR)*.c))
HEADERS = $(wildcard $(SOURCEDIR)*.h)

%.o: %.c $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
		$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

run:
		./$(TARGET)

clean:
		-rm -f $(SOURCEDIR)*.o
		-rm -f $(TARGET)
