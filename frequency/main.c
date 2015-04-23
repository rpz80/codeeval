#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <sys/mman.h>

void process(const char* data) {
  printf("%s\n", data);
}

int main(int argc, const char* argv[]) {
  double start = (double)clock();

  if (argc < 2) {
    printf("Usage: freq <file-name>\n");
    return 1;
  }

  struct stat s;
  int fd = open (argv[1], O_RDONLY);

  int status = fstat (fd, &s);
  if (status == -1) {
    strerror(errno);
    return 1;
  }

  process((const char *)mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
  close(fd);

  printf("Elapsed: %.3fms\n", ((double)clock() - start)*1000/CLOCKS_PER_SEC);
  return 0;
}
