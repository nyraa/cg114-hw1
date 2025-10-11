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
    *triangles = (struct Triangle*)malloc(numTriangles * sizeof(struct Triangle));
    if (!*triangles) {
        perror("Failed to allocate memory");
        fclose(file);
        return 0;
    }

    char buffer[50];
    for (uint32_t i = 0; i < numTriangles; i++)
    {
        fread(buffer, 50, 1, file);
        struct Triangle* tri = &((*triangles)[i]);
        *tri = *(struct Triangle*)buffer;
    }
    fclose(file);
    return numTriangles;
}