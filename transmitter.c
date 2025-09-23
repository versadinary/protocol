#include <stdio.h>
#include "transmitter.h"

void read_sensor(FILE* fstream, byte* triad[3]) {
  int data[3];
  fscanf(fstream, "%1d%1d%1d", &data[0], &data[1], &data[2]);
  for (int i = 0; i < 3; i++) {
    triad[i]->d0 = data[i] / 4;
    triad[i]->d1 = data[i] / 2 % 2;
    triad[i]->d2 = data[i] % 2;
  }
}

void create_package(byte* pkg, FILE* fstream) {
  for (int i = 0; i < 4; i++) {
    byte* triad[] = {&pkg[3 * i], &pkg[3 * i + 1], &pkg[3 * i + 2]}; 
    read_sensor(fstream, triad);
    for (int k = 0; k < 3; k++) {
      triad[k]->s = 1;
      triad[k]->ps0 = 1;
      triad[k]->ps1 = 1;
      triad[k]->pd0 = triad[k]->d0 ^ triad[k]->d1 ^ triad[k]->d2;
    }
    triad[0]->pd1 = 0;
    for (int k = 0; k < 3; k++)
      triad[0]->pd1 ^= triad[k]->d2;
    triad[1]->pd1 = 0;
    for (int k = 0; k < 3; k++)
      triad[1]->pd1 ^= triad[k]->d1;
    triad[2]->pd1 = 0;
    for (int k = 0; k < 3; k++)
      triad[2]->pd1 ^= triad[k]->d0;
  }
  pkg[0].ps0 = 0;
  pkg[0].s = 0;
  pkg[0].ps1 = 0;
  pkg[11].ps0 = 0;
  pkg[11].s = 0;
  pkg[11].ps1 = 0;
}

void print_package(byte* pkg) {
  printf("%3s %3s %3s %3s %3s %3s %3s %3s\n", "d0", "d1", "d2",
        "pd0", "pd1", "ps0", "ps1", "s");
  for (int i = 0; i < 12; i++) {
    printf("%3d %3d %3d %3d %3d %3d %3d %3d\n", pkg[i].d0, pkg[i].d1, 
        pkg[i].d2, pkg[i].pd0, pkg[i].pd1, pkg[i].ps0, pkg[i].ps1, pkg[i].s);
  }
}
