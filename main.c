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
  int zero_flag = 0;
  int count1 = 0;
  int interrupt_flag = 0;
  read_package(receiver, receiver_pkg, &zero_flag, &count1, &interrupt_flag);
  print_package(receiver_pkg);
  simple_data_parity(receiver_pkg, &interrupt_flag);
  fclose(receiver);

  return 0;
} 

