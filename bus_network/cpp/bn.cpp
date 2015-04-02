#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    printf("Usage: bn <file>\n");
    return 1;
  }
  
  FILE* f = fopen(argv[1], "rb");
  if (f == NULL) {
    printf("File open error\n");
    return 1;
  }
  fseek(f, 0, SEEK_END);
  uint32_t fsize = ftell(f);
  fseek(f, 0 , SEEK_SET);

  char* buf = (char*)malloc(fsize + 1);
  free(buf);
  return 0;
}