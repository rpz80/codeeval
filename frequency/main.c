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

enum wave_type {
  square,
  maybe_square,
  other
};

static
void process(char* data, const uint32_t size) {
  uint32_t count = 0;
  int parsing = 1;
  int first_hit = 1;
  int tmp, local_peak=0, change_count=0;
  enum direction_type dir = no;
  int max_peaks = 0, min_peaks = 0;
  enum wave_type wt = other;

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
          if (tmp - local_peak > 3) {
            if (tmp - local_peak > 20) {
              if (wt == other)
                wt = maybe_square;
              else if (wt == maybe_square)
                wt = square;
            }
            else
              wt = other;
            //printf("ascening\n");
            if (dir == no) {
              dir = ascending;
            }
            else if (dir == descending) {
              ++min_peaks;
              dir = ascending;
              ++change_count;
            }
            local_peak = tmp;
          }
          else if (local_peak - tmp > 3) {
            if (local_peak - tmp > 20) {
              if (wt == other)
                wt = maybe_square;
              else if (wt == maybe_square)
                wt = square;
            }
            else
              wt = other;
            //printf("descending\n");
            if (dir == no) {
              dir = descending;
            }
            else if (dir == ascending) {
              dir = descending;
              ++max_peaks;
              ++change_count;
            }
            local_peak = tmp;
          }
        }
        ++count;
        if (count % 2000 == 0) {
          int total_peaks = max_peaks > min_peaks ? max_peaks : min_peaks;
          if (wt == square) {
            printf("%u\n", 10*(total_peaks + 1));
          }
          else {
            printf("%u\n", 10*(total_peaks));
          }
          first_hit = 1;
          change_count = 0;
          dir = no;
          min_peaks = max_peaks = 0;
          wt = other;
        }
      }
    }
    else {
      parsing = 1;
    }
    ++data;
  }
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
