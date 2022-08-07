#include <stdio.h>

int main() {
  int floor = 0;
  int basement_index = 0;
  int position = 0;
  for (int c = fgetc(stdin); c != EOF; c = fgetc(stdin)) {
    position++;
    switch (c) {
      case '(':
        floor++;
        break;
      case ')':
        floor--;
        break;
    }
    if (floor == -1 && basement_index == 0) {
      basement_index = position;
    }
  }
  printf("%d\n%d\n", floor, basement_index);
}
