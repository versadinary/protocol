#include <stdio.h>
#include "transmitter.h"

int main() {
  byte pkg1[12] = {0};
  FILE* ifs = fopen("pack1", "r");
  create_package(pkg1, ifs);
  print_package(pkg1);

  return 0;
} 

