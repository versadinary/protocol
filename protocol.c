#include <stdio.h>

typedef struct {
  unsigned d0:1;
  unsigned d1:1;
  unsigned d2:1;
  unsigned pd0:1;
  unsigned pd1:1;
  unsigned ps0:1;
  unsigned ps1:1;
  unsigned s:1;
} byte;

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
  for (int i = 0; i < 12; i++) {
    printf("%3d %3d %3d %3d %3d %3d %3d %3d\n", pkg[i].d0, pkg[i].d1, 
        pkg[i].d2, pkg[i].pd0, pkg[i].pd1, pkg[i].ps0, pkg[i].ps1, pkg[i].s);
  }
}

void byte_to_hex(byte b, char* dest) {
  char decimal = b.s + (b.ps1 << 1) + (b.ps0 << 2) + (b.pd1 << 3) +
    (b.pd0 << 4) + (b.d2 << 5) << (b.d1 << 6) + (b.d0 << 7);
  sprintf(dest, "%02X", decimal);
}

void write_package(FILE* dest, byte* pkg) {
  for (int i = 0; i < 12; i++) {
    char hex[3];
    byte_to_hex(pkg[i], hex);
    fprintf(dest, "%s\n", hex);
  }
}

int main() {
  byte pkg1[12] = {0};
  FILE* ifs = fopen("sensors", "r");
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
  FILE* ofs = fopen("pkg_tr", "a");
  write_package(ofs, pkg1);
  

  return 0;
} 
