#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <sys/mman.h>

static
void process(char* data, const uint32_t size) {
  uint32_t count = 0;
  int parsing = 1;

  for (uint32_t i = 0; i < size; ++i) {
    if (isdigit(*data) || *data == '-') {
      if (parsing) {
        parsing = 0;
        strtoul(data, NULL, 10);
        ++count;
      }
    }
    else {
      parsing = 1;
    }
    ++data;
  }
  //printf("%s\n", data);
  printf("Counted: %u\n", count);
}

int main(int argc, const char* argv[]) {
  double start = (double)clock();

  if (argc < 2) {
    printf("Usage: freq <file-name>\n");
    return 1;
  }

  struct stat s;
  int fd = open(argv[1], O_RDONLY);

  int status = fstat(fd, &s);
  if (status == -1) {
    strerror(errno);
    return 1;
  }

  process((char*)mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0), s.st_size);
  close(fd);

  printf("Elapsed: %.3fms\n", ((double)clock() - start)*1000/CLOCKS_PER_SEC);
  return 0;
}
