#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { max_people = 16 };
int num_people;
char people[max_people][8];

int person(const char* name) {
  for (int i = 0; i < num_people; i++) {
    if (strcmp(people[i], name) == 0) return i;
  }
  if (num_people == max_people) abort();
  if (strlen(name) > 7) abort();
  const int i = num_people++;
  strcpy(people[i], name);
  return i;
}

// deltas[a][b] is the impact on a's happiness for sitting next to b.
int deltas[max_people][max_people];

int seat(int* people, int seat_index, unsigned unseated_mask, int happiness) {
  if (seat_index == num_people) {
    return happiness + deltas[0][people[num_people - 1]]
                     + deltas[people[num_people - 1]][0];
  }
  int best_value = -999999;
  int best_index = -1;
  if (!unseated_mask) abort();
  for (int i = 1; i < num_people; i++) {
    if ((1 << i) & unseated_mask) {
      people[seat_index] = i;
      const int x = seat(people, seat_index + 1, unseated_mask ^ (1 << i),
                         happiness + deltas[i][people[seat_index - 1]]
                                   + deltas[people[seat_index - 1]][i]);
      if (x > best_value) {
        best_value = x;
        best_index = i;
      }
    }
  }
  people[seat_index] = best_index;
  return best_value;
}

int main() {
  while (true) {
    char a[8], b[8], direction[5];
    int magnitude;
    const int result = scanf(
        "%7s would %4s %d happiness units by sitting next to %7[A-Za-z]. ",
        a, direction, &magnitude, b);
    if (result == EOF) break;
    if (result != 4) abort();
    const bool is_gain = strcmp(direction, "gain") == 0;
    if (!is_gain && !strcmp(direction, "lose") == 0) abort();
    deltas[person(a)][person(b)] = is_gain ? magnitude : -magnitude;
  }
  int seats[max_people];
  seats[0] = 0;
  const int part1 = seat(seats, 1, (1 << num_people) - 2, 0);
  person("Me");
  const int part2 = seat(seats, 1, (1 << num_people) - 2, 0);
  printf("%d\n%d\n", part1, part2);
}
