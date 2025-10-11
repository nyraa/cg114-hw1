CC = gcc
CFLAGS = -Wall
LDFLAGS = -lGL -lGLU -lglut -lm

OBJ = displaystl.o readstl.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: displaystl

displaystl: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f displaystl $(OBJ)

.PHONY: all clean