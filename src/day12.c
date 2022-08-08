#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct results {
  const char* rest;
  int part1;
  int part2;
};

struct results visit_json(const char* json);

struct results visit_string(const char* json) {
  json++;
  while (*json != '"') json++;
  return (struct results){.rest = json + 1};
}

struct results visit_int(const char* json) {
  bool negative = false;
  int value = 0;
  if (*json == '-') {
    negative = true;
    json++;
  }
  while ('0' <= *json && *json <= '9') {
    value = 10 * value + (*json - '0');
    json++;
  }
  const int x = negative ? -value : value;
  return (struct results){.rest = json, .part1 = x, .part2 = x};
}

struct results visit_array(const char* json) {
  struct results results = {0};
  while (true) {
    const struct results temp = visit_json(json + 1);
    json = temp.rest;
    results.part1 += temp.part1;
    results.part2 += temp.part2;
    if (*json == ']') break;
    if (*json != ',') abort();
  }
  results.rest = json + 1;
  return results;
}

struct results visit_object(const char* json) {
  struct results results = {0};
  bool is_red = false;
  while (true) {
    {
      const struct results temp = visit_string(json + 1);
      json = temp.rest;
    }
    if (*json != ':') abort();
    json++;
    if (strncmp(json, "\"red\"", 5) == 0) {
      is_red = true;
      json += 5;
    } else {
      const struct results temp = visit_json(json);
      json = temp.rest;
      results.part1 += temp.part1;
      results.part2 += temp.part2;
    }
    if (*json == '}') break;
    if (*json != ',') abort();
  }
  results.rest = json + 1;
  if (is_red) results.part2 = 0;
  return results;
}

struct results visit_json(const char* json) {
  if (*json == '"') {
    return visit_string(json);
  } else if (*json == '-' || ('0' <= *json && *json <= '9')) {
    return visit_int(json);
  } else if (*json == '[') {
    return visit_array(json);
  } else if (*json == '{') {
    return visit_object(json);
  } else {
    abort();
  }
}

char buffer[65536];

int main() {
  fread(buffer, 1, sizeof(buffer), stdin);
  const struct results results = visit_json(buffer);
  printf("%d\n%d\n", results.part1, results.part2);
}
