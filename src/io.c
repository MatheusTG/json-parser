#include <stdio.h>
#include <stdlib.h>

char* readFile(const char *path) {
  FILE *file = fopen(path, "r");
  if (!file) return NULL;

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *buffer = malloc(size + 1);
  if (!buffer) {
      fclose(file);
      return NULL;
  }

  fread(buffer, 1, size, file);
  buffer[size] = '\0';

  fclose(file);
  return buffer;
}