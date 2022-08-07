#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cell {
  unsigned short part1 : 1;
  unsigned short part2 : 15;
};

struct cell grid[1000][1000];

enum op {
  turn_on,
  turn_off,
  toggle,
};

struct position {
  short x, y;
};

void apply(enum op op, struct cell* x) {
  switch (op) {
    case turn_on:
      x->part1 = true;
      x->part2++;
      break;
    case turn_off:
      x->part1 = false;
      if (x->part2) x->part2--;
      break;
    case toggle:
      x->part1 = !x->part1;
      x->part2 += 2;
      break;
    default:
      abort();
  }
}

int main() {
  while (true) {
    char lookahead[8];
    if (scanf("%7s", lookahead) == EOF) break;
    enum op op;
    if (strcmp(lookahead, "turn") == 0) {
      char value[4];
      if (scanf("%3s", value) != 1) abort();
      if (strcmp(value, "on") == 0) {
        op = turn_on;
      } else if (strcmp(value, "off") == 0) {
        op = turn_off;
      } else {
        abort();
      }
    } else if (strcmp(lookahead, "toggle") == 0) {
      op = toggle;
    } else {
      abort();
    }
    struct position a, b;
    if (scanf(" %hd,%hd through %hd,%hd", &a.x, &a.y, &b.x, &b.y) != 4) abort();
    if (a.x > b.x || a.y > b.y) abort();
    for (int y = a.y; y <= b.y; y++) {
      for (int x = a.x; x <= b.x; x++) {
        apply(op, &grid[y][x]);
      }
    }
  }
  int part1 = 0, part2 = 0;
  for (int y = 0; y < 1000; y++) {
    for (int x = 0; x < 1000; x++) {
      part1 += grid[y][x].part1;
      part2 += grid[y][x].part2;
    }
  }
  printf("%d\n%d\n", part1, part2);
}
