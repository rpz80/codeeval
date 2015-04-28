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

#define CHANGE_LIMIT 10

enum direction_type {
  ascending,
  descending,
  no
};

static
void process(char* data, const uint32_t size) {
  uint32_t count = 0;
  int parsing = 1;
  int first_hit = 1;
  int tmp, local_peak=0, change_count=0;
  enum direction_type dir = no;

  for (uint32_t i = 0; i < size; ++i) {
    if (isdigit(*data) || *data == '-') {
      if (parsing) {
        parsing = 0;
        tmp = strtoul(data, NULL, 10);
        if (first_hit) {
          first_hit = 0;
          local_peak = tmp;
        }
        else {
          if (tmp - local_peak > 10) {
            //printf("ascening\n");
            if (dir == no) {
              dir = ascending;
            }
            else if (dir == descending) {
              dir = ascending;
              ++change_count;
            }
            local_peak = tmp;
          }
          else if (local_peak - tmp > 10) {
            //printf("descending\n");
            if (dir == no) {
              dir = descending;
            }
            else if (dir == ascending) {
              dir = descending;
              ++change_count;
            }
            local_peak = tmp;
          }
        }
        ++count;
        if (count % 2000 == 0) {
          printf("change count: %u\n", 10*change_count/2);
          first_hit = 1;
          change_count = 0;
        }
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
