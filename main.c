#include <stdio.h>
#include "transmitter.h"

int main() {
  byte pkg1[12] = {0};
  FILE* ifs = fopen("pack1", "r");
  create_package(pkg1, ifs);
  printf("%3s %3s %3s %3s %3s %3s %3s %3s\n", "d0", "d1", "d2",
      "pd0", "pd1", "ps0", "ps1", "s");
  print_package(pkg1);

  return 0;
} 

