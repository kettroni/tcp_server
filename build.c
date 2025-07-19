#include <stdio.h>
#include <stdlib.h>

const char *cmd = "cc -Wall -Wextra main.c -o ./output/main";

int main(int argc, char **argv) {
  return system(cmd);
}
