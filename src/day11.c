#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct password {
  char digits[8];
};

uint64_t little_endian(char* x) {
  return (uint64_t)x[0] |
         (uint64_t)x[1] << 8 |
         (uint64_t)x[2] << 16 |
         (uint64_t)x[3] << 24 |
         (uint64_t)x[4] << 32 |
         (uint64_t)x[5] << 40 |
         (uint64_t)x[6] << 48 |
         (uint64_t)x[7] << 56;
}

uint64_t big_endian(char* x) {
  return (uint64_t)x[0] << 56 |
         (uint64_t)x[1] << 48 |
         (uint64_t)x[2] << 40 |
         (uint64_t)x[3] << 32 |
         (uint64_t)x[4] << 24 |
         (uint64_t)x[5] << 16 |
         (uint64_t)x[6] << 8  |
         (uint64_t)x[7];
}

bool equal(struct password a, struct password b) {
  return little_endian(a.digits) == little_endian(b.digits);
}

int straight_index(struct password p) {
  for (int i = 0; i < 6; i++) {
    const char a = p.digits[i] + 2,
               b = p.digits[i + 1] + 1,
               c = p.digits[i + 2];
    if (a == b && b == c) return i;
  }
  return -1;
}

void inc(char* x, int size) {
  for (int i = size - 1; i >= 0; i--) {
    if (x[i] < 'z') {
      x[i]++;
      return;
    } else {
      x[i] = 'a';
    }
  }
  abort();
}

bool advance_straight(struct password* p) {
  const int i = straight_index(*p);
  if (i != -1) return false;
  // *****bba -> *****bcd (case 1)
  // ****cyza -> ****dabc (case 2)
  // ****cdab -> ****cdea (case 3)
  // ***bccxy -> ***bcdaa (case 4)
  {
    // Attempt case 4.
    char target = p->digits[3];
    if (target <= 'x' && p->digits[4] == target + 1 &&
        p->digits[5] < target + 2) {
      p->digits[5] = target + 2;
      p->digits[6] = 'a';
      p->digits[7] = 'a';
      return true;
    }
  }
  {
    // Attempt case 3.
    char target = p->digits[4];
    if (target <= 'x' &&
        (p->digits[5] < target + 1 ||
         (p->digits[5] == target + 1 && p->digits[6] < target + 2))) {
      p->digits[5] = target + 1;
      p->digits[6] = target + 2;
      p->digits[7] = 'a';
      return true;
    }
  }
  char target = p->digits[5];
  if (p->digits[6] > target + 1 || p->digits[7] > target + 2) target++;
  if (target > 'x') {
    // case 2
    inc(p->digits, 5);
    target = 'a';
  }
  p->digits[5] = target;
  p->digits[6] = target + 1;
  p->digits[7] = target + 2;
  return true;
}

bool advance_legal(struct password* p) {
  for (int i = 0; i < 8; i++) {
    const char c = p->digits[i];
    if (c == 'i' || c == 'o' || c == 'l') {
      p->digits[i]++;
      for (int j = i + 1; j < 8; j++) {
        p->digits[j] = 'a';
      }
      return true;
    }
  }
  return false;
}

int find_pair(char* value, int size) {
  const int end = size - 1;
  for (int i = 0; i < end; i++) {
    if (value[i] == value[i + 1]) return i;
  }
  return -1;
}

void make_pair(char* x) {
  // Form a pair.
  // adc -> add
  // abc -> acc
  // cbd -> cca
  if (x[2] > x[1]) x[1]++;
  x[2] = x[0] == x[1] ? 'a' : x[1];
}

bool advance_pairs(struct password* p) {
  const int first_pair = find_pair(p->digits, 8);
  if (first_pair == -1 || first_pair >= 4) {
    // Need to form two pairs at the end. The second pair can always be aa.
    // ****dcba -> ****ddaa
    // ***dbcda -> ***dccaa
    // ***cbcda -> ***ccaaa
    // ****baab -> ****bbaa
    // ****abba -> ****bbaa
    // ***babba -> ***bbaaa
    make_pair(p->digits + 3);
    p->digits[6] = 'a';
    p->digits[7] = 'a';
    return true;
  }
  const int second_pair =
      find_pair(p->digits + first_pair + 2, 8 - first_pair - 2);
  if (second_pair == -1) {
    // Need to form a single pair at the end.
    // *****aba -> *****abb
    // *****ace -> *****add
    // *****bab -> *****bba
    make_pair(p->digits + 5);
    return true;
  }
  return false;
}

struct password advance(struct password p) {
  inc(p.digits, 8);
  bool a, b, c;
  do {
    a = advance_straight(&p);
    b = advance_legal(&p);
    c = advance_pairs(&p);
  } while (a || b || c);
  return p;
}

int main() {
  struct password input;
  {
    char buffer[10];
    if (scanf("%8[a-z] ", buffer) != 1) abort();
    if (strlen(buffer) != 8) abort();
    memcpy(input.digits, buffer, 8);
  }
  const struct password part1 = advance(input);
  const struct password part2 = advance(part1);
  printf("%.8s\n%.8s\n", part1.digits, part2.digits);
}
