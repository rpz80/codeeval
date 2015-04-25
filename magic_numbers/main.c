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

#define BUF_SIZE 8

static struct mn_type {
  uint32_t  size;
  uint32_t  cur;
  char      buf[BUF_SIZE];
  uint8_t   digit_count[10];
  uint8_t   visited;
} mn;

static
int utos(char* buf, uint32_t num) {
  int wcount = 0;
  uint32_t tmp = num;

  while ((tmp /= 10) != 0) {
    ++wcount;
  }

  int i = wcount;
  while (num != 0) {
    tmp = num % 10;
    num /= 10;
    *(buf + i--) = (char)(tmp + 48);
  }

  return wcount + 1;
}

static
void mn_reset(uint32_t num) {
  mn.size = utos(mn.buf, num);
  mn.cur = 0;
  mn.visited = 0;

  memset(mn.digit_count, 0, 10);
  for (int i = 0; i < mn.size; ++i) {
    ++mn.digit_count[(int)(mn.buf[i]-48)];
  }
}

enum parsing_state {
  first,
  second
};

static
int advance() {
  int steps = (int)(mn.buf[mn.cur]-48);

  for (int i = 0; i < steps; ++i) {
    if (mn.cur == mn.size - 1)
      mn.cur = 0;
    else
      ++mn.cur;
  }

  ++mn.visited;
  uint8_t vis_tmp = mn.digit_count[mn.buf[mn.cur]-48];

  if (vis_tmp == 1) {
    if (mn.visited < mn.size) {
      ++mn.digit_count[mn.buf[mn.cur]-48];
      return advance();
    }
    else if (mn.visited == mn.size) {
      return 1;
    }
  }

  return 0;
}

static
int is_magic() {
  if (mn.digit_count[0] != 0) {
    return 0;
  }

  for (int i = 0; i < 10; ++i) {
    if (mn.digit_count[i] > 1) {
      return 0;
    }
  }

  if (!advance())
    return 0;
  return 1;
}

static
void calc_magic_nums(uint32_t f, uint32_t s) {
  int found = 0;

  for (uint32_t i = f; i <= s; ++i) {
    mn_reset(i);
    if (is_magic()) {
      found = 1;
      fwrite(mn.buf, 1, mn.size, stdout);
      fwrite(" " , 1, 1, stdout);
    }
  }

  if (!found) {
    printf("-1");
  }

  printf("\n");
}

static
void process(char* data, const uint32_t size) {
  int parsing = 1;
  enum parsing_state st = first;
  uint32_t f, s;

  for (uint32_t i = 0; i < size; ++i) {
    if (isdigit(*data)) {
      if (parsing) {
        parsing = 0;
        if (st == first) {
          st = second;
          f = strtoul(data, NULL, 10);
        }
        else {
          st = first;
          s = strtoul(data, NULL, 10);
          calc_magic_nums(f, s);
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
    printf("Usage: mn <file-name>\n");
    return 1;
  }

  struct stat s;
  int fd = open(argv[1], O_RDONLY);

  if (fd == -1) {
    printf("File open error\n");
    return 1;
  }

  int status = fstat(fd, &s);
  if (status == -1) {
    strerror(errno);
    return 1;
  }

  process((char*)mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0), s.st_size);
  close(fd);

  printf("\nElapsed: %.3fms\n", ((double)clock() - start)*1000/CLOCKS_PER_SEC);
  return 0;
}
