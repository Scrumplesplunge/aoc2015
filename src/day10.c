#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stream {
  void* data;
  int (*peek)(void*);
  void (*next)(void*);
};

struct noop_stream {
  const char* i;
  const char* end;
};

int noop_stream_peek(void* data) {
  struct noop_stream* stream = data;
  return stream->i == stream->end ? EOF : *stream->i;
}

void noop_stream_next(void* data) {
  struct noop_stream* stream = data;
  stream->i++;
}

struct stream noop_stream(struct noop_stream* s) {
  return (struct stream){
    .data = s,
    .peek = noop_stream_peek,
    .next = noop_stream_next,
  };
}

struct look_and_say {
  struct stream input;
  int offset;
  bool is_count;
  char c;
  char repeated_char;
};

int look_and_say_peek(void* data) {
  struct look_and_say* stream = data;
  return stream->c;
}

void look_and_say_next(void* data) {
  struct look_and_say* stream = data;
  if (stream->is_count) {
    stream->is_count = false;
    stream->offset++;
    stream->c = stream->repeated_char;
  } else {
    const int x = stream->input.peek(stream->input.data);
    if (x == EOF) {
      stream->c = EOF;
      return;
    }
    int count = 1;
    while (true) {
      stream->input.next(stream->input.data);
      if (stream->input.peek(stream->input.data) != x) break;
      count++;
    }
    if (count > 9) abort();
    stream->is_count = true;
    stream->offset++;
    stream->c = '0' + count;
    stream->repeated_char = x;
  }
}

struct stream look_and_say(struct look_and_say* s) {
  look_and_say_next(s);
  return (struct stream){
    .data = s,
    .peek = look_and_say_peek,
    .next = look_and_say_next,
  };
}

enum { part1_steps = 40, part2_steps = 50 };

void solve(const char* seed, int* part1, int* part2) {
  struct noop_stream input = {
    .i = seed,
    .end = seed + strlen(seed),
  };
  struct look_and_say steps[part2_steps] = {0};
  steps[0].input = noop_stream(&input);
  for (int i = 1; i < part2_steps; i++) {
    steps[i].input = look_and_say(&steps[i - 1]);
  }
  struct stream result = look_and_say(&steps[part2_steps - 1]);
  while (result.peek(result.data) != EOF) result.next(result.data);
  *part1 = steps[part1_steps - 1].offset;
  *part2 = steps[part2_steps - 1].offset;
}

int main() {
  char input[12];
  if (!fgets(input, sizeof(input), stdin)) abort();
  {
    const int n = strlen(input);
    if (input[n - 1] == '\n') input[n - 1] = '\0';
  }
  int part1, part2;
  solve(input, &part1, &part2);
  printf("%d\n%d\n", part1, part2);
}
