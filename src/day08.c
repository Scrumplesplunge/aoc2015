#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int measure_decoded(const char* s) {
  if (s[0] != '"') abort();
  s++;
  int length = 0;
  while (*s != '"') {
    if (*s == '\\') {
      ++s;
      switch (*s) {
        case '\\':
        case '"':
          s++;
          length++;
          break;
        case 'x':
          if (!s[1] || !s[2]) abort();
          s += 3;
          length++;
          break;
        default:
          abort();
      }
    } else {
      if (!s) abort();
      s++;
      length++;
    }
  }
  return length;
}

int measure_encoded(const char* s) {
  // Count the enclosing brackets.
  int length = 2;
  // Count the contents.
  for (const char* i = s; *i; i++) {
    switch (*i) {
      case '\\':
      case '"':
        length += 2;
        break;
      default:
        length++;
        break;
    }
  }
  return length;
}

int main() {
  int source_length = 0;
  int decoded_length = 0;
  int encoded_length = 0;
  while (true) {
    char line[80];
    if (!fgets(line, sizeof(line), stdin)) break;
    int n = strlen(line);
    if (line[n - 1] == '\n') {
      n--;
      line[n] = '\0';
    }
    source_length += n;
    decoded_length += measure_decoded(line);
    encoded_length += measure_encoded(line);
  }
  const int part1 = source_length - decoded_length;
  const int part2 = encoded_length - source_length;
  printf("%d\n%d\n", part1, part2);
}
