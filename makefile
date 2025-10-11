CC = gcc
CFLAGS = -Wall
LDFLAGS = -lGL -lGLU -lglut

displaystl: displaystl.c
	$(CC) $(CFLAGS) -o displaystl displaystl.c $(LDFLAGS)
clean:
	rm -f displaystl
.PHONY: clean