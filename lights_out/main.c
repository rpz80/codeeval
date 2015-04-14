#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <string.h>

struct stringref {
  char* begin;
  uint32_t size;
};

struct stringref new_stringref() {
  return (struct stringref){.begin = NULL, .size = 0};
}

struct stringref copy_stringref(const struct stringref* other) {
  struct stringref ret;

  ret.size = other->size;
  ret.begin = calloc(ret.size, sizeof(char));
  memcpy(ret.begin, other->begin, ret.size);

  return ret;
}

void free_stringref(struct stringref* s) {
  free(s->begin);
  s->size = 0;
}
//struct stringref new_stringref_s(char* b, uint32_t size) {
//  return (struct stringref){.begin = b, .size = size};
//}
//
//struct stringref new_stringref_p(char* b, char* e) {
//  return (struct stringref){.begin = b, .size = (e - b)};
//}


struct board {
  struct stringref* rows;
  int8_t* visited;
  uint32_t size;
};

struct board new_board() {
  return (struct board){NULL, NULL, 0};
}

struct board copy_board(const struct board* other) {
  struct board b = new_board();

  if (other->size == 0)
    return b;

  b.size = other->size;
  uint32_t row_size = other->rows[0].size;
  b.visited = calloc(row_size*b.size, sizeof(int8_t));
  memcpy(b.visited, other->visited, row_size*b.size*sizeof(int8_t));

  b.rows = malloc(b.size*sizeof(struct stringref));
  for (uint32_t i = 0; i < b.size; ++i) {
    b.rows[i] = copy_stringref(&other->rows[i]);
  }
  return b;
}

struct board* add_row_from_s(struct board* b, char** s) {
  struct stringref new_row = new_stringref();

  while (**s == ' ' || **s == '\n' || **s == '\r' || **s == '|')
    ++*s;

  char* start = *s;

  while (**s == 'O' || **s == '.')
    ++*s;

  new_row.size = *s - start;
  new_row.begin = calloc(new_row.size, sizeof(char));
  memcpy(new_row.begin, start, new_row.size);

  b->rows = realloc(b->rows, (b->size + 1)*sizeof(struct stringref));
  b->rows[b->size++] = new_row;

  b->visited = realloc(b->visited, b->size*new_row.size*sizeof(int8_t));
  memset(b->visited, 0, b->size*new_row.size);
  return b;
}

void free_board(struct board* b) {
  for (uint32_t i = 0; i < b->size; ++i)
    free_stringref(&b->rows[i]);
  free(b->rows);
  free(b->visited);
  b->size = 0;
}

void print_board(struct board* b) {
  assert(b);
  for (uint32_t i = 0; i < b->size; ++i) {
    for (uint32_t j = 0; j < b->rows[i].size; ++j) {
      printf("%2c", b->rows[i].begin[j]);
    }
    printf("\n");
  }
}

char* at(struct board* b, uint32_t row, uint32_t col) {
  return &b->rows[row].begin[col];
}

int8_t *v_at(struct board* b, uint32_t row, uint32_t col) {
  return &b->visited[b->rows[0].size*row + col];
}

#define Toggle(b, row, col) \
  if (*at(b, row, col) == '.') { \
    *at(b, row, col) = 'O'; \
  } else { \
    *at(b, row, col) = '.'; \
  }

void toggle_light(struct board* b, uint32_t row, uint32_t col) {
  Toggle(b, row, col);
  if (col > 0) {
    Toggle(b, row, col - 1);
  }

  if (col < b->rows[0].size - 1) {
    Toggle(b, row, col + 1);
  }

  if (row > 0) {
    Toggle(b, row - 1, col);
  }

  if (row < b->size-1) {
    Toggle(b, row + 1, col);
  }
}


int check_lights_off(const struct board* b) {
  for (uint32_t i = 0; i < b->size; ++i) {
    for (uint32_t j = 0; j < b->rows[i].size; ++j) {
      if (b->rows[i].begin[j] == 'O')
        return 0;
    }
  }
  return 1;
}


uint32_t do_calc(struct board* b, uint32_t steps, int* found) {
  uint32_t num_toggles = UINT32_MAX;

  if (!check_lights_off(b)) {
    for (uint32_t i = 0; i < b->size; ++i) {
      for (uint32_t j = 0; j < b->rows[i].size; ++j) {
        if (!*v_at(b, i, j)) {
          toggle_light(b, i, j);
          struct board b2 = copy_board(b);
          *v_at(&b2, i, j) = 1;
          uint32_t toggles_tmp = do_calc(&b2, steps + 1, found);
          if (*found && toggles_tmp < num_toggles) {
            num_toggles = toggles_tmp;
          }
          free_board(&b2);
        }
      }
    }
    if (num_toggles != UINT32_MAX)
      return num_toggles;
  }
  else {
   *found = 1;
    return steps;
  }

  return num_toggles;
}

void calc_turnoffs(struct board* b) {
  uint32_t num_toggles = UINT32_MAX;
  int found = 0;


  if (!check_lights_off(b)) {
    for (uint32_t i = 0; i < b->size; ++i) {
      for (uint32_t j = 0; j < b->rows[i].size; ++j) {
        if (!found && !*v_at(b, i, j)) {
          toggle_light(b, i, j);
          struct board b2 = copy_board(b);
          *v_at(&b2, i, j) = 1;
          uint32_t toggles_tmp = do_calc(&b2, 1, &found);
          if (found && toggles_tmp < num_toggles) {
            num_toggles = toggles_tmp;
          }
          free_board(&b2);
        }
      }
    }
    if (num_toggles != UINT32_MAX) {
      printf("toggles: %u\n", num_toggles);
      return;
    }
  }
  else {
    printf("toggles: 0\n");
    return;
  }
  printf("toggles: -1\n");
}

void calc2(struct board* b) {
  uint32_t num_toggles = UINT32_MAX;
  int found = 0;

  if(!check_lights_off(b)) {

  }
  else {
    printf("toggles: 0\n");
    return;
  }
}

void process(char* s) {
  enum pstate {
    row,
    num,
    data
  } state = row;

  int rows;
  int cols;

  while (s && *s) {
    switch (state) {
      case row:
        while (*s == ' ' || *s == '\n' || *s == '\r') {
          ++s;
        }
        rows = strtol(s, &s, 10);
        state = num;
        break;
      case num:
        while (*s == ' ' || *s == '\n' || *s == '\r') {
          ++s;
        }
        cols = strtol(s, &s, 10);
        state = data;
        break;
      case data: {
        struct board b = new_board();
        while (*s != '\n' && *s != '\r' && *s)
          add_row_from_s(&b, &s);
        state = row;
        calc_turnoffs(&b);
        free_board(&b);
      }
    }
  }
}

int main(int argc, const char* argv[]) {
  double start = clock();

  if (argc < 2) {
    printf("Usage lo <file>\n");
    return 1;
  }

  FILE* f = fopen(argv[1], "rb");

  if (!f) {
    printf("File open error\n");
    return 1;
  }

  fseek(f, 0, SEEK_END);
  uint32_t fsize = (uint32_t)ftell(f);
  fseek(f, 0, SEEK_SET);

  char* fdata = malloc(fsize + 1);
  fread(fdata, 1, fsize, f);
  fclose(f);
  fdata[fsize] = '\0';

  process(fdata);
  free(fdata);

  printf("elapsed: %.1f ms\n", (clock() - start)*1000/CLOCKS_PER_SEC);
}