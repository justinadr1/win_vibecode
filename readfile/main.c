#include <stdio.h>
#include <stdlib.h>

char* readFile(const char* filename)
{
    FILE* file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    int size = ftell(file);

    rewind(file);

    char* buffer = (char*)malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    return buffer;
}

int main()
{
    char* lyrics = readFile("lyrics.txt");

    printf(lyrics);

    free(lyrics);
}