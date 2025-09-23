#include <stdio.h>
#include "transmitter.h"

int main() {
  byte pkg1[12] = {0};
  FILE* ifs = fopen("./transmitter_data", "r");
  create_package(pkg1, ifs);
  fclose(ifs);
  printf("%3s %3s %3s %3s %3s %3s %3s %3s\n", "d0", "d1", "d2",
      "pd0", "pd1", "ps0", "ps1", "s");
  print_package(pkg1);
  for (int i = 0; i < 12; i++) {
    char hex[3];
    byte_to_hex(pkg1[i], hex);
    printf("%s\n", hex);
  }
  FILE* ofs = fopen("./receiver_data", "a");
  write_package(ofs, pkg1);

  return 0;
} 

