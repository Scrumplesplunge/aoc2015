#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { max_places = 16, max_distance = 999999, anywhere = 0 };
int num_places;
char places[max_places][16];
unsigned short distances[max_places][max_places];

int place(const char* name) {
  for (int i = 0; i < num_places; i++) {
    if (strcmp(places[i], name) == 0) return i;
  }
  if (strlen(name) > 15) abort();
  if (num_places == max_places) abort();
  const int i = num_places++;
  strcpy(places[i], name);
  return i;
}

// Travelling Salesman Problem
int tsp(int current_place, int current_distance, unsigned unvisited_mask) {
  if (!unvisited_mask) return current_distance;
  int best = max_distance;
  for (int i = 0; i < num_places; i++) {
    const unsigned x = 1 << i;
    if (unvisited_mask & x) {
      const int d = tsp(i, current_distance + distances[current_place][i],
                        unvisited_mask ^ x);
      if (d < best) best = d;
    }
  }
  return best;
}

int part1() {
  // Make a mask of the form 1111....110
  // This excludes "anywhere" (id 0), but includes all other places.
  const unsigned unvisited = (1 << num_places) - 2;
  return tsp(anywhere, 0, unvisited);
}

// WTF Salesman Problem: take the longest route
int wtfsp(int current_place, int current_distance, unsigned unvisited_mask) {
  if (!unvisited_mask) return current_distance;
  int best = -1;
  for (int i = 0; i < num_places; i++) {
    const unsigned x = 1 << i;
    if (unvisited_mask & x) {
      const int d = wtfsp(i, current_distance + distances[current_place][i],
                          unvisited_mask ^ x);
      if (d > best) best = d;
    }
  }
  return best;
}

int part2() {
  // Make a mask of the form 1111....110
  // This excludes "anywhere" (id 0), but includes all other places.
  const unsigned unvisited = (1 << num_places) - 2;
  return wtfsp(anywhere, 0, unvisited);
}

int main() {
  // Reserve ID 0 as "anywhere", which has a distance of 0 from each place.
  if (place("anywhere") != anywhere) abort();
  while (true) {
    char a_name[16], b_name[16];
    int distance;
    const int result = scanf("%15s to %15s = %d ", a_name, b_name, &distance);
    if (result == EOF) break;
    if (result != 3) abort();
    const int a = place(a_name);
    const int b = place(b_name);
    distances[a][b] = distances[b][a] = distance;
  }
  // 314 too low
  printf("%d\n%d\n", part1(), part2());
}
