#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int num_vowels(const char* word) {
  int count = 0;
  const unsigned vowels =
      1 << ('a' - 'a') |
      1 << ('e' - 'a') |
      1 << ('i' - 'a') |
      1 << ('o' - 'a') |
      1 << ('u' - 'a');
  for (const char* i = word; *i; i++) {
    const unsigned x = 1 << (*i - 'a');
    if (x & vowels) count++;
  }
  return count;
}

bool has_repeat(const char* word) {
  for (const char* i = word + 1; *i; i++) {
    if (i[-1] == i[0]) return true;
  }
  return false;
}

bool has_forbidden(const char* word) {
  for (const char* i = word + 1; *i; i++) {
    if (i[-1] == 'a' && i[0] == 'b') return true;
    if (i[-1] == 'c' && i[0] == 'd') return true;
    if (i[-1] == 'p' && i[0] == 'q') return true;
    if (i[-1] == 'x' && i[0] == 'y') return true;
  }
  return false;
}

bool part1_nice(const char* word) {
  return num_vowels(word) >= 3 &&
         has_repeat(word) &&
         !has_forbidden(word);
}

bool has_repeated_sequence(const char* word) {
  const int n = strlen(word);
  for (int i = 0; i < n - 3; i++) {
    for (int j = i + 2; j < n - 1; j++) {
      if (word[i] == word[j] && word[i + 1] == word[j + 1]) return true;
    }
  }
  return false;
}

bool has_sandwich(const char* word) {
  const int n = strlen(word);
  for (int i = 0; i < n - 2; i++) {
    if (word[i] == word[i + 2]) return true;
  }
  return false;
}

bool part2_nice(const char* word) {
  return has_repeated_sequence(word) && has_sandwich(word);
}

int main() {
  int part1 = 0, part2 = 0;
  while (1) {
    char word[17];
    const int result = scanf("%16[a-z] ", word);
    if (result == EOF) break;
    if (result != 1) abort();
    if (part1_nice(word)) part1++;
    if (part2_nice(word)) part2++;
  }
  printf("%d\n%d\n", part1, part2);
}
