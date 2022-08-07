#include <stdio.h>
#include <stdlib.h>

int main() {
  int paper = 0;
  int ribbon = 0;
  while (1) {
    int l, w, h;
    const int result = scanf("%dx%dx%d ", &l, &w, &h);
    if (result == EOF) break;
    if (result != 3) abort();
    // Calculate how much wrapping paper is needed.
    {
      int a = l * w, b = w * h, c = h * l;
      int smallest = a;
      if (b < smallest) smallest = b;
      if (c < smallest) smallest = c;
      paper += 2 * l * w + 2 * w * h + 2 * h * l + smallest;
    }
    // Calculate how much ribbon is needed.
    {
      int a = l + w, b = w + h, c = h + l;
      int smallest = a;
      if (b < smallest) smallest = b;
      if (c < smallest) smallest = c;
      ribbon += 2 * smallest + l * w * h;
    }
  }
  printf("%d\n%d\n", paper, ribbon);
}
