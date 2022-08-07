#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum input_type {
  input_literal,
  input_wire,
};

struct input {
  enum input_type type;
  union {
    int literal;
    char wire[4];
  };
};

enum op {
  op_mov,
  op_and,
  op_or,
  op_lshift,
  op_rshift,
  op_not,
};

struct wire {
  char name[4];
  enum op op;
  struct input inputs[2];
  bool evaluated;
  uint16_t value;
};

struct node {
  struct wire wire;
  struct node* next;
};

enum { max_nodes = 1024, num_buckets = 256 };
int num_nodes;
struct node nodes[max_nodes];
struct node* buckets[num_buckets];

struct node** find(const char* wire) {
  const int n = strlen(wire);
  if (n > 3) abort();
  unsigned h = 0xdeadbeef;
  for (int i = 0; i < n; i++) h = 33 * h + (unsigned char)wire[i];
  h %= num_buckets;
  struct node** node = &buckets[h];
  while (*node) {
    if (strcmp((*node)->wire.name, wire) == 0) return node;
    node = &(*node)->next;
  }
  return node;
}

struct node* add(const char* wire) {
  struct node** p = find(wire);
  if (*p) abort();
  if (num_nodes == max_nodes) abort();
  struct node* n = &nodes[num_nodes++];
  strcpy(n->wire.name, wire);
  *p = n;
  return n;
}

struct node* get(const char* wire) {
  struct node** p = find(wire);
  if (!*p) abort();
  return *p;
}

void parse_input(const char* text, struct input* input) {
  if ('0' <= text[0] && text[0] <= '9') {
    input->type = input_literal;
    if (sscanf(text, "%d", &input->literal) != 1) abort();
  } else {
    if (strlen(text) > 3) abort();
    input->type = input_wire;
    strcpy(input->wire, text);
  }
}

bool scan_def(struct wire* wire) {
  memset(wire, 0, sizeof(struct wire));
  char line[64];
  if (!fgets(line, sizeof(line), stdin)) return false;
  char a[8], b[8];
  if (sscanf(line, "%7s -> %3s ", a, wire->name) == 2) {
    wire->op = op_mov;
    parse_input(a, &wire->inputs[0]);
    return true;
  } else if (sscanf(line, "NOT %7s -> %3s", a, wire->name) == 2) {
    wire->op = op_not;
    parse_input(a, &wire->inputs[0]);
    return true;
  } else if (sscanf(line, "%7s AND %7s -> %3s", a, b, wire->name) == 3) {
    wire->op = op_and;
    parse_input(a, &wire->inputs[0]);
    parse_input(b, &wire->inputs[1]);
    return true;
  } else if (sscanf(line, "%7s OR %7s -> %3s", a, b, wire->name) == 3) {
    wire->op = op_or;
    parse_input(a, &wire->inputs[0]);
    parse_input(b, &wire->inputs[1]);
    return true;
  } else if (sscanf(line, "%7s LSHIFT %7s -> %3s", a, b, wire->name) == 3) {
    wire->op = op_lshift;
    parse_input(a, &wire->inputs[0]);
    parse_input(b, &wire->inputs[1]);
    return true;
  } else if (sscanf(line, "%7s RSHIFT %7s -> %3s", a, b, wire->name) == 3) {
    wire->op = op_rshift;
    parse_input(a, &wire->inputs[0]);
    parse_input(b, &wire->inputs[1]);
    return true;
  } else {
    fprintf(stderr, "bad gate: %s\n", line);
    abort();
  }
}

uint16_t raw_eval_wire(struct wire* wire);
uint16_t eval_wire(const char* name);
uint16_t eval_input(struct input input) {
  switch (input.type) {
    case input_literal:
      return input.literal;
    case input_wire:
      return eval_wire(input.wire);
  }
  abort();
}

uint16_t eval_wire(const char* wire_name) {
  struct wire* wire = &get(wire_name)->wire;
  if (!wire->evaluated) {
    wire->value = raw_eval_wire(wire);
    wire->evaluated = true;
  }
  return wire->value;
}

uint16_t raw_eval_wire(struct wire* wire) {
  switch (wire->op) {
    case op_mov:
      return eval_input(wire->inputs[0]);
    case op_and:
      return eval_input(wire->inputs[0]) & eval_input(wire->inputs[1]);
    case op_or:
      return eval_input(wire->inputs[0]) | eval_input(wire->inputs[1]);
    case op_lshift:
      return eval_input(wire->inputs[0]) << eval_input(wire->inputs[1]);
    case op_rshift:
      return eval_input(wire->inputs[0]) >> eval_input(wire->inputs[1]);
    case op_not:
      return ~eval_input(wire->inputs[0]);
    default:
      abort();
  }
}

int main() {
  struct wire wire;
  while (scan_def(&wire)) {
    add(wire.name)->wire = wire;
  }
  const uint16_t part1 = eval_wire("a");
  get("b")->wire = (struct wire){
    .name = "b",
    .op = op_mov,
    .inputs[0] = (struct input){
      .type = input_literal,
      .literal = part1,
    },
  };
  // Clear evaluation cache so that we re-evaluate.
  for (int i = 0; i < num_nodes; i++) {
    nodes[i].wire.evaluated = false;
  }
  const uint16_t part2 = eval_wire("a");
  printf("%d\n%d\n", part1, part2);
}
