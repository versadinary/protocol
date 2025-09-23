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

void read_package(FILE* fstream, byte* pkg, int* zf, int* interrupt_flag) {
  int count = 0;
  int hex;
  while (count < 12 && fscanf(fstream, "%x", &hex) == 1) {
    hex_to_byte(hex, &pkg[count]);

    if ()
  }
}
  
void data_parity(byte* pkg, int interrupt_flag) {

}


