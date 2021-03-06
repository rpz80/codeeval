#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define ROOM_SIZE 10

typedef enum {
  NW,
  SW,
  NE,
  SE
} direction;

struct cell {
  uint8_t row;
  uint8_t col;
};

struct ray {
  uint8_t       steps;
  direction     dir;
  cell   cur_cell;
};


#define Set_dir(coord, dir1, dir2) \
  switch (coord) \
  { \
    case 0: \
      r->dir = dir1; \
      break;   \
    case 9:  \
      r->dir = dir2; \
      break; \
    default: \
      break; \
  }


struct ray* init_ray(const char* room, struct ray* r) {
  assert(room);
  assert(r);

  r->steps = 1;

  for (uint8_t i = 0; i < ROOM_SIZE; ++i) {
    for(uint8_t j = 0; j < ROOM_SIZE; ++j) {
      switch (room[i*ROOM_SIZE + j]) {
        case '/': {
          cell c = {i, j};
          r->cur_cell = c;
          Set_dir(i, SW, NE);
          Set_dir(j, NE, SW);
          break;
                  }
        case '\\':{
          cell c = {i, j};
          r->cur_cell = c;
          Set_dir(i, SE, NW);
          Set_dir(j, SE, NW);
          break;
                  }
        default:
          break;
      }
    }
  }

  return r;
}

struct ray* init_ray2(struct ray* r, uint8_t steps, 
                      direction dir, struct cell c) {
  assert(r);

  r->dir = dir;
  r->cur_cell = c;
  r->steps = steps;

  return r;
}

char* at(char* room, uint8_t row, uint8_t cell) {
  assert(room);
  assert(row  >= 0 && row  < ROOM_SIZE);
  assert(cell >= 0 && cell < ROOM_SIZE);

  return &room[row*ROOM_SIZE + cell];
}

void advance(struct ray* r, char* room);

void try_reflect(struct ray* r, char* room) {
  switch (r->dir) {
    case NW: {
      if (r->cur_cell.row - 1 == 0 && r->cur_cell.col - 1 == 0)
        return;
      if (r->cur_cell.col - 1 == 0) {
        r->dir = NE;
        r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
        ++r->steps;
        if (*at(room, r->cur_cell.row, r->cur_cell.col) == '\\' || 
            *at(room, r->cur_cell.row, r->cur_cell.col) == 'X')
          *at(room, r->cur_cell.row, r->cur_cell.col) = 'X';
        else
          *at(room, r->cur_cell.row, r->cur_cell.col) = '/';

        //*at(room, r->cur_cell.row, r->cur_cell.col) = '/';
        advance(r, room);
      }
      else {
        r->dir = SW;
        r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
        ++r->steps;

        if (*at(room, r->cur_cell.row, r->cur_cell.col) == '\\' || 
            *at(room, r->cur_cell.row, r->cur_cell.col) == 'X')
          *at(room, r->cur_cell.row, r->cur_cell.col) = 'X';
        else
          *at(room, r->cur_cell.row, r->cur_cell.col) = '/';

        //*at(room, r->cur_cell.row, r->cur_cell.col) = '/';
        advance(r, room);
      }
      break;
    }
    
    case NE: {
      if (r->cur_cell.row - 1 == 0 && r->cur_cell.col + 1 == 9)
        return;
      if (r->cur_cell.col + 1 == 9) {
        r->dir = NW;
        r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
        ++r->steps;
        if (*at(room, r->cur_cell.row, r->cur_cell.col) == '/' || 
            *at(room, r->cur_cell.row, r->cur_cell.col) == 'X')
          *at(room, r->cur_cell.row, r->cur_cell.col) = 'X';
        else
          *at(room, r->cur_cell.row, r->cur_cell.col) = '\\';

        //*at(room, r->cur_cell.row, r->cur_cell.col) = '\\';
        advance(r, room);
      }
      else {
        r->dir = SE;
        r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
        ++r->steps;
        if (*at(room, r->cur_cell.row, r->cur_cell.col) == '/' ||
            *at(room, r->cur_cell.row, r->cur_cell.col) == 'X')
          *at(room, r->cur_cell.row, r->cur_cell.col) = 'X';
        else
          *at(room, r->cur_cell.row, r->cur_cell.col) = '\\';

        //*at(room, r->cur_cell.row, r->cur_cell.col) = '\\';
        advance(r, room);
      }
      break;
    }

    case SW: {
      if (r->cur_cell.row + 1 == 9 && r->cur_cell.col - 1 == 0)
        return;      
      if (r->cur_cell.col - 1 == 0) {
        r->dir = SE;
        r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
        ++r->steps;
        if (*at(room, r->cur_cell.row, r->cur_cell.col) == '/' || 
            *at(room, r->cur_cell.row, r->cur_cell.col) == 'X')
          *at(room, r->cur_cell.row, r->cur_cell.col) = 'X';
        else
          *at(room, r->cur_cell.row, r->cur_cell.col) = '\\';

        //*at(room, r->cur_cell.row, r->cur_cell.col) = '\\';
        advance(r, room);
      }
      else {
        r->dir = NW;
        r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
        ++r->steps;

        if (*at(room, r->cur_cell.row, r->cur_cell.col) == '/' || 
            *at(room, r->cur_cell.row, r->cur_cell.col) == 'X')
          *at(room, r->cur_cell.row, r->cur_cell.col) = 'X';
        else
          *at(room, r->cur_cell.row, r->cur_cell.col) = '\\';

        //*at(room, r->cur_cell.row, r->cur_cell.col) = '\\';
        advance(r, room);
      }
      break;
    }

    case SE: {
      if (r->cur_cell.row + 1 == 9 && r->cur_cell.col + 1 == 9)
        return;
      if (r->cur_cell.col + 1 == 9) {
        r->dir = SW;
        r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
        ++r->steps;

        if (*at(room, r->cur_cell.row, r->cur_cell.col) == '\\' ||
            *at(room, r->cur_cell.row, r->cur_cell.col) == 'X')
          *at(room, r->cur_cell.row, r->cur_cell.col) = 'X';
        else
          *at(room, r->cur_cell.row, r->cur_cell.col) = '/';

        //*at(room, r->cur_cell.row, r->cur_cell.col) = '/';
        advance(r, room);
      }
      else {
        r->dir = NE;
        r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
        ++r->steps;

        if (*at(room, r->cur_cell.row, r->cur_cell.col) == '\\' || 
            *at(room, r->cur_cell.row, r->cur_cell.col) == 'X')
          *at(room, r->cur_cell.row, r->cur_cell.col) = 'X';
        else
          *at(room, r->cur_cell.row, r->cur_cell.col) = '/';

        //*at(room, r->cur_cell.row, r->cur_cell.col) = '/';
        advance(r, room);
      }
      break;
    }

  };
}

void try_split(struct ray* r, char* room) {
  assert(r);
  assert(room);

  switch (r->dir) {
    case SE:
    case NW: {

      struct ray ray_SW;
      init_ray2(&ray_SW, r->steps, SW, r->cur_cell);
      advance(&ray_SW, room);

      struct ray ray_NE;
      init_ray2(&ray_NE, r->steps, NE, r->cur_cell);
      advance(&ray_NE, room);

    }
      break;

    case SW:
    case NE: {

      struct ray ray_SE;
      init_ray2(&ray_SE, r->steps, SE, r->cur_cell);
      advance(&ray_SE, room);

      struct ray ray_NW;
      init_ray2(&ray_NW, r->steps, NW, r->cur_cell);
      advance(&ray_NW, room);

    }
      break;

//    case SW: {
//      struct ray ray_NW;
//      init_ray2(&ray_NW, r->steps, NW, r->cur_cell);
//      advance(&ray_NW, room);
//
//      struct ray ray_SE;
//      init_ray2(&ray_SE, r->steps, SE, r->cur_cell);
//      advance(&ray_SE, room);
//    }

//    case SE: {
//      struct ray ray_NE;
//      init_ray2(&ray_NE, r->steps, NE, r->cur_cell);
//      advance(&ray_NE, room);
//
//      struct ray ray_SW;
//      init_ray2(&ray_SW, r->steps, SW, r->cur_cell);
//      advance(&ray_SW, room);
//    }
    default:
      break;
  };

  //r->steps = 0;
  advance(r, room);
}

void advance(struct ray* r, char* room) {
  assert(r);
  assert(room);

  if (r->steps == 20)
    return;
  
  uint8_t row = r->cur_cell.row;
  uint8_t col = r->cur_cell.col;

  switch (r->dir) {
    case NW: {
      if (row == 0 || col == 0)
        return;
      char* next = at(room,
                      (uint8_t)(r->cur_cell.row - 1),
                      (uint8_t)(r->cur_cell.col - 1));

      switch (*next) {
        case ' ':
        case '\\':
          *next = '\\';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
          ++r->steps;
          advance(r, room);
          break;
        case 'o':
          break;
        case 'X':
          *next = 'X';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
          ++r->steps;
          advance(r, room);
          break;

        case '#':
          try_reflect(r, room);
          break;
        case '*':
          r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
          try_split(r, room);
          break;
        case '/':
          *next = 'X';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
          ++r->steps;
          advance(r, room);
          break;
        default:
          break;
      }
      break;
    } // case NW
    
    case SW: {
      if (row == 9 || col == 0)
        return;

      char* next = at(room, (uint8_t)(r->cur_cell.row + 1),
                            (uint8_t)(r->cur_cell.col - 1));

      switch (*next) {
        case ' ':
        case '/':
          *next = '/';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
          ++r->steps;
          advance(r, room);
          break;
        case 'X':
          *next = 'X';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
          ++r->steps;
          advance(r, room);
          break;

        case '#':
          try_reflect(r, room);
          break;
        case '*':
          r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
          try_split(r, room);
          break;
        case '\\':
          *next = 'X';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col - 1);
          ++r->steps;
          advance(r, room);
          break;
        default:
          break;
      }
      break;
    } //case SW
    
    case NE: {
      if (row == 0 || col == 9)
        return;

      char* next = at(room,
                      (uint8_t)(r->cur_cell.row - 1),
                      (uint8_t)(r->cur_cell.col + 1));

      switch (*next) {
        case ' ':
        case '/':
          *next = '/';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
          ++r->steps;
          advance(r, room);
          break;
        case 'X':
          *next = 'X';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
          ++r->steps;
          advance(r, room);
          break;

        case '#':
          try_reflect(r, room);
          break;
        case '*':
          r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
          try_split(r, room);
          break;
        case '\\':
          *next = 'X';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row - 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
          ++r->steps;
          advance(r, room);
          break;
        default:
          break;
      }
      break;
    } //case NE
    
    case SE: {
      if (row == 9 || col == 9)
        return;

      char* next = at(room,
                      (uint8_t)(r->cur_cell.row + 1),
                      (uint8_t)(r->cur_cell.col + 1));

      switch (*next) {
        case ' ':
        case '\\':
          *next = '\\';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
          ++r->steps;
          advance(r, room);
          break;
        case 'X':
          *next = 'X';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
          ++r->steps;
          advance(r, room);
          break;

        case '#':
          try_reflect(r, room);
          break;
        case '*':
          r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
          try_split(r, room);
          break;
        case '/':
          *next = 'X';
          r->cur_cell.row = (uint8_t)(r->cur_cell.row + 1);
          r->cur_cell.col = (uint8_t)(r->cur_cell.col + 1);
          ++r->steps;
          advance(r, room);
          break;
        default:
          break;
      }
      break;
    } // case SE

    default:
      break;
  }; // switch (r->dir)
}

void process(char* room) {
  struct ray r;
  init_ray(room, &r);
  advance(&r, room);

  //for (uint8_t i = 0; i < ROOM_SIZE; ++i) {
  // for (uint8_t j = 0; j < ROOM_SIZE; ++j) {
  //   printf("%-1c", room[i*ROOM_SIZE + j]);
  // }
  // printf("\n");
  //}

  for (uint8_t i = 0; i < ROOM_SIZE*ROOM_SIZE; ++i) {
    printf("%c", room[i]);
  }
  printf("\n");
}

int main(int argc, const char* argv[]) {
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

  char* fcontent = (char*)malloc(fsize + 1);
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

  free(fcontent);
  printf("Elapsed: %.3fs\n", (double)(clock() - start)/ CLOCKS_PER_SEC);

  return 0;
};