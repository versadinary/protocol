#include <stdio.h>
#include "transmitter.h"
#include "receiver.h"

int main() {
  byte pkg1[12] = {0};
  FILE* ifs = fopen("./transmitter_data", "r");
  create_package(pkg1, ifs);
  fclose(ifs);
  print_package(pkg1);
  for (int i = 0; i < 12; i++) {
    char hex[3];
    byte_to_hex(pkg1[i], hex);
    printf("%s\n", hex);
  }
  FILE* ofs = fopen("./receiver_data", "a");
  write_package(ofs, pkg1);
  fclose(ofs);

  FILE* receiver = fopen("./receiver_data", "r");
  byte receiver_pkg[12];
  read_package(receiver, receiver_pkg);
  print_package(receiver_pkg);
  fclose(receiver);

  return 0;
} 

