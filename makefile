CC = gcc
CFLAGS = -Wall
LDFLAGS = -lGL -lGLU -lglut -lm

# Cross-compile (MinGW) settings for Windows .exe
CROSS_CC = x86_64-w64-mingw32-gcc
CROSS_INCLUDES = -I/usr/local/x86_64-w64-mingw32/include
CROSS_LIBDIR = -L/usr/local/x86_64-w64-mingw32/lib
CROSS_CFLAGS = -DFREEGLUT_STATIC
CROSS_LDFLAGS = -Wl,-Bstatic -lfreeglut_static -Wl,-Bdynamic -lopengl32 -lglu32 -lgdi32 -luser32 -lkernel32 -lwinmm -static-libgcc -static-libstdc++

OBJ = displaystl.o readstl.o
WIN_OBJ = displaystl_win.o readstl_win.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%_win.o: %.c
	$(CROSS_CC) $(CFLAGS) $(CROSS_CFLAGS) $(CROSS_INCLUDES) -c $< -o $@

all: displaystl

displaystl: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

# Windows target using MinGW cross-compiler
displaystl.exe: $(WIN_OBJ)
	$(CROSS_CC) $(CFLAGS) $(CROSS_CFLAGS) $(CROSS_INCLUDES) -o $@ $(WIN_OBJ) $(CROSS_LIBDIR) $(CROSS_LDFLAGS)

clean:
	rm -f displaystl displaystl.exe $(OBJ) $(WIN_OBJ)

.PHONY: all clean