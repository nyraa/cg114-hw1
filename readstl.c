#include "readstl.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t readBinSTL(const char* filename, struct Triangle** triangles)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }
    // Skip the 80-byte header
    fseek(file, 80, SEEK_SET);
    
    uint32_t numTriangles;
    fread(&numTriangles, sizeof(uint32_t), 1, file);
    *triangles = malloc(numTriangles * sizeof(struct Triangle));
    if (!*triangles) {
        perror("Failed to allocate memory");
        fclose(file);
        return 0;
    }

    fread(*triangles, sizeof(struct Triangle), numTriangles, file);
    fclose(file);
    return numTriangles;
}