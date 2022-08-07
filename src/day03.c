#include <stdio.h>
#include <stdlib.h>

struct location {
  short x, y;
};

struct node {
  struct location location;
  short part1_visits;
  short part2_visits;
  short next_node;
};

enum { max_nodes = 8192, num_buckets = 1024 };
int num_nodes;
short buckets[num_buckets];
struct node nodes[max_nodes];

struct node* get(struct location location) {
  const int bucket =
      ((unsigned)location.x + 29 * (unsigned)location.y) % num_buckets;
  short* i = &buckets[bucket];
  while (1) {
    if (*i == -1) break;
    struct node* node = &nodes[*i];
    if (node->location.x == location.x && node->location.y == location.y) {
      return node;
    }
    i = &node->next_node;
  }
  if (num_nodes == max_nodes) abort();
  *i = num_nodes++;
  struct node* node = &nodes[*i];
  *node = (struct node){
    .location = location,
    .part1_visits = 0,
    .part2_visits = 0,
    .next_node = -1,
  };
  return node;
}

int step(struct location* location, char c) {
  switch (c) {
    case '<':
      location->x--;
      return 1;
    case '>':
      location->x++;
      return 1;
    case '^':
      location->y--;
      return 1;
    case 'v':
      location->y++;
      return 1;
    default:
      return 0;
  }
}

int main() {
  for (int i = 0; i < num_buckets; i++) buckets[i] = -1;
  struct location part1 = {0};
  struct location part2[2] = {0};
  int part2_index = 0;
  for (int c = fgetc(stdin); c != EOF; c = fgetc(stdin)) {
    if (step(&part1, c)) get(part1)->part1_visits++;
    part2_index++;
    struct location* l = &part2[part2_index % 2];
    if (step(l, c)) get(*l)->part2_visits++;
  }
  int part1_count = 0;
  int part2_count = 0;
  for (int i = 0; i < num_nodes; i++) {
    struct node* n = &nodes[i];
    if (n->part1_visits) part1_count++;
    if (n->part2_visits) part2_count++;
  }
  printf("%d\n%d\n", part1_count, part2_count);
}
