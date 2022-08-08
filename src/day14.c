#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum state {
  state_running,
  state_resting,
};

struct reindeer {
  int speed;
  int endurance;
  int rest;

  int points;
  int distance_covered;
  enum state state;
  int time_left;
};

enum { max_reindeers = 16 };
int num_reindeers;
struct reindeer reindeers[max_reindeers];

int main() {
  while (true) {
    if (num_reindeers == max_reindeers) abort();
    struct reindeer* r = &reindeers[num_reindeers++];
    const int result = scanf(
        "%*s can fly %d km/s for %d seconds, "
        "but then must rest for %d seconds. ",
        &r->speed, &r->endurance, &r->rest);
    r->points = 0;
    r->distance_covered = 0;
    r->state = state_running;
    r->time_left = r->endurance;
    if (result == EOF) break;
    if (result != 3) abort();
  }
  for (int i = 0; i < 2503; i++) {
    // Move all reindeers.
    for (int j = 0; j < num_reindeers; j++) {
      struct reindeer* r = &reindeers[j];
      switch (r->state) {
        case state_running:
          r->distance_covered += r->speed;
          r->time_left--;
          if (r->time_left == 0) {
            r->state = state_resting;
            r->time_left = r->rest;
          }
          break;
        case state_resting:
          r->time_left--;
          if (r->time_left == 0) {
            r->state = state_running;
            r->time_left = r->endurance;
          }
          break;
      }
    }
    // Find the reindeer(s) that are in the lead.
    int best = 0;
    for (int i = 1; i < num_reindeers; i++) {
      if (reindeers[i].distance_covered > reindeers[best].distance_covered) {
        best = i;
      }
    }
    // Award points to all reindeer(s) that are in the lead.
    for (int i = 0; i < num_reindeers; i++) {
      if (reindeers[i].distance_covered == reindeers[best].distance_covered) {
        reindeers[i].points++;
      }
    }
  }
  int furthest = 0, most_points = 0;
  for (int i = 0; i < num_reindeers; i++) {
    const struct reindeer* r = &reindeers[i];
    if (r->distance_covered > furthest) furthest = r->distance_covered;
    if (r->points > most_points) most_points = r->points;
  }
  printf("%d\n%d\n", furthest, most_points);
}
