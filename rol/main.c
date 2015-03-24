#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define ROOM_SIZE 10

void process(const char* room)
{
  for (uint8_t i = 0; i < ROOM_SIZE; ++i) {
    for (uint8_t j = 0; j < ROOM_SIZE; ++j) {
      printf("%-1c", room[i*ROOM_SIZE + j]);
    }
    printf("\n");
  }
}

int main(int argc, const char* argv[])
{
  time_t start = clock();
  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }

  FILE* f = fopen(argv[1], "rb");
  if (f == NULL) {
    printf("File open error\n");
    return 1;
  }

  fseek(f, 0, SEEK_END);
  uint32_t fsize = (uint32_t)ftell(f);
  fseek(f, 0, SEEK_SET);

  char fcontent[fsize + 1];
  fread(fcontent, 1, fsize, f);
  fcontent[fsize] = '\0';
  fclose(f);

  char* pfc = fcontent;
  while (*pfc != '\0') {
    process(pfc);
    pfc += 100;
    while (*pfc == '\n' || *pfc == '\r') {
      ++pfc;
    }
  }

  printf("Elapsed: %.3fs\n", (double)(clock() - start)/ CLOCKS_PER_SEC);

  return 0;
};