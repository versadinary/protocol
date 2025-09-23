#include <stdio.h>
#include "transmitter.h"
#include "receiver.h"

void hex_to_byte(int hex, byte* b) {
  b->s = hex % 2;
  b->ps1 = (hex >> 1) % 2;
  b->ps0 = (hex >> 2) % 2;
  b->pd1 = (hex >> 3) % 2;
  b->pd0 = (hex >> 4) % 2;
  b->d2 = (hex >> 5) % 2;
  b->d1 = (hex >> 6) % 2;
  b->d0 = (hex >> 7) % 2;
}

// zf - zero flag
// c1 - count ones
void read_package(FILE* fstream, byte* pkg, int* zf, int* c1, int* interrupt_flag) {
  int count = 0;
  int hex;
  while (count < 12 && fscanf(fstream, "%x", &hex) == 1) {
    hex_to_byte(hex, &pkg[count]);
    if (pkg[count].s == 0 && *zf == 1 && *c1 != 10) {
      *interrupt_flag = 1;
      return; 
    }
    *c1 += pkg[count].s;
    if (pkg[count].s == 0) *zf = 1;
    count++;
  }
  *zf = 0;
  *c1 = 0;
}
  
void data_parity(byte* pkg, int* interrupt_flag) {

}


