#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

struct buf_t {
  uint8_t buf[36];
  uint8_t count;
};

static struct buf_t primes;

int is_prime(uint8_t num) {
  for (uint8_t i = 0; i < primes.count; ++i) {
    if (num == primes.buf[i]) {
      return 1;
    }
  }
  return 0;
}

void fill_primes() {
  primes.count = 0;
  primes.buf[primes.count++] = 2;
  primes.buf[primes.count++] = 3;

  for (uint8_t i = 4; i < 36; ++i) {
    uint8_t found = 0;
    for (uint8_t j = 0; j < primes.count; ++j) {
      if (i % primes.buf[j] == 0) {
        found = 1;
        break;
      }
    }
    if (!found) {
      primes.buf[primes.count++] = i;
    }
  }
}

uint32_t calc(const struct buf_t* table, uint8_t row, uint32_t mask) {
  uint32_t ret = 0;
  if (mask == 1 && table[row].buf[0] == 1) {
    return 1;
  }
  for (uint8_t i = 0; i < table[row].count; ++i) {
    if (mask & (1 << (table[row].buf[i] - 1))) {
      ret += calc(
          table,
          table[row].buf[i],
          mask & ~(1 << (table[row].buf[i] - 1))
      );
    }
  }
  return ret;
}

void process(uint32_t num) {
  struct buf_t table[num + 1];
  for (uint8_t i = 0; i < num + 1; ++i) {
    table[i].count = 0;
  }
  for (uint8_t i = 1; i <= num; ++i) {
    for (uint8_t j = 1; j <= num; ++j) {
      if (i != j && is_prime(i + j)) {
        table[i].buf[table[i].count++] = j;
      }
    }
  }
  uint32_t mask = (uint32_t)(pow(2, num) - 1);
  printf("%u\n", calc(table, 1, mask));
}

int main(int argc, const char* argv[]) {
  time_t start = clock();
  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }

  FILE* f = fopen(argv[1], "r");
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
  char* pfc = fcontent;

  fill_primes();

  while (*pfc != 0) {
    if (isdigit(*pfc)) {
      uint32_t tmp = (uint32_t)strtol(pfc, &pfc, 10);
      if (tmp != 0)
        process(tmp);
      continue;
    }
    ++pfc;
  }
  printf("Elapsed: %.3fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
  return 0;
}

