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
  if (*zf == 0 && pkg[count].s == 1) {
    *interrupt_flag = 1;
    printf("DELIVERY FAILED\n");
  }
  while (count < 12 && fscanf(fstream, "%x", &hex) == 1) {
    hex_to_byte(hex, &pkg[count]);
    if (pkg[count].s == 0) *zf = !(*zf);
    if (*zf == 0 && count != 11) {
      printf("DELIVERY FAILED\n");
      return;
    }
    if (count == 0) {
      count++;
      continue;
    }
    if (pkg[count].s == 0 && *zf == 1 && *c1 != 10) {
      *interrupt_flag = 1;
      printf("DELIVERY FAILED\n");
      return; 
    }
    *c1 += pkg[count].s;
    count++;
  }
  *c1 = 0;
}

void simple_data_parity(byte *pkg, int *interrupt_flag) {
  for (int i = 0; i < 4; i++) {
    int d[3][3] = {pkg[3 * i].d0, pkg[3 * i].d1, pkg[3 * i].d2,
                   pkg[3 * i + 1].d0, pkg[3 * i + 1].d1, pkg[3 * i + 1].d2,
                   pkg[3 * i + 2].d0, pkg[3 * i + 2].d1, pkg[3 * i + 2].d2};
    int par[6] = {0};
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        par[j] ^= d[j][k];
        par[j + 3] ^= d[k][2 - j];
      }
    }
    int par_data[] = {pkg[3 * i].pd0, pkg[3 * i + 1].pd0,
                      pkg[3 * i + 2].pd0, pkg[3 * i].pd1,
                      pkg[3 * i + 1].pd1, pkg[3 * i + 2].pd1};
    for (int k = 0; k < 6; k++) {
      if (par_data[k] != par[k]) {
        *interrupt_flag = 1;
        printf("DATA ERROR\n");
        return;
      }
    }
  }
  for (int i = 0; i < 12; i++) {
    int s = pkg[i].s + pkg[i].ps0 + pkg[i].ps1;
    if (s != 3 && s != 0) {
      printf("SYNC BYTE ERROR\n");
      *interrupt_flag = 1;
      return;
    }
  }
}
  
void data_parity(byte* pkg, int* interrupt_flag){
  if (interrupt_flag){
    printf("incorrect pkg");
    return;
  }
  int pkg_in[12][8];
  for(int i = 0; i<12; i++){
    pkg_in[i][0] = pkg[i].d0;
    pkg_in[i][1] = pkg[i].d1;
    pkg_in[i][2] = pkg[i].d2;
    pkg_in[i][3] = pkg[i].pd0;
    pkg_in[i][4] = pkg[i].pd1;
    pkg_in[i][5] = pkg[i].ps0;
    pkg_in[i][6] = pkg[i].ps1;
    pkg_in[i][7] = pkg[i].s;
  }
  int d[4][3][3];
  int pd[4][2][3];
  int s[12][3];
  
  for(int i = 0; i<4; i++){
    for(int j = 0; j<3; j++){
      int byte_idx = i*3 + j;
      d[i][j][0] = pkg_in[byte_idx][0];  
      d[i][j][1] = pkg_in[byte_idx][1];  
      d[i][j][2] = pkg_in[byte_idx][2];  
    }
    for(int j = 0; j<3; j++){
      int byte_idx = i*3 + j;
      pd[i][0][j] = pkg_in[byte_idx][3]; 
      pd[i][1][j] = pkg_in[byte_idx][4]; 
    }
  }
  for(int i = 0; i<12; i++){
    s[i][0] = pkg_in[i][5];  
    s[i][1] = pkg_in[i][6];  
    s[i][2] = pkg_in[i][7];  
  }


  for(int i = 0; i < 4; i++){          
    for(int j = 0; j < 3; j++){        
      int calc_pd0 = d[i][j][0] ^ d[i][j][1] ^ d[i][j][2];
      int received_pd0 = pd[i][0][j];
      
      int calc_pd1 = d[i][0][j] ^ d[i][1][j] ^ d[i][2][j];
      int received_pd1 = pd[i][1][j];
      
      if((calc_pd0 != received_pd0) && (calc_pd1 != received_pd1)){
        printf("Error detected in triad %d, byte %d, bit %d - correcting\n", i, j, j);
        d[i][j][j] = d[i][j][j] ^ 1;
        
        int byte_idx = i*3 + j;
        pkg[byte_idx].d0 = d[i][j][0];
        pkg[byte_idx].d1 = d[i][j][1];
        pkg[byte_idx].d2 = d[i][j][2];
      }
      else if(calc_pd0 != received_pd0){
        printf("PD0 error in triad %d, byte %d\n", i, j);
      }
      else if(calc_pd1 != received_pd1){
        printf("PD1 error in triad %d, bit pos %d\n", i, j);
      }
    }
  }

  int sync_errors = 0;
  
  if(s[0][0] != 0 || s[0][1] != 0 || s[0][2] != 0) {
    printf("Sync error: first byte should be 0-0-0, but got %d-%d-%d\n", 
           s[0][0], s[0][1], s[0][2]);
    sync_errors++;
  }
  
  if(s[11][0] != 0 || s[11][1] != 0 || s[11][2] != 0) {
    printf("Sync error: last byte should be 0-0-0, but got %d-%d-%d\n", 
           s[11][0], s[11][1], s[11][2]);
    sync_errors++;
  }
  
  for(int i = 1; i < 11; i++) {
    if(s[i][0] != 1 || s[i][1] != 1 || s[i][2] != 1) {
      printf("Sync error: byte %d should be 1-1-1, but got %d-%d-%d\n", 
             i, s[i][0], s[i][1], s[i][2]);
      sync_errors++;
    }
  }
  
  printf("Sync sequence: ");
  for(int i = 0; i < 12; i++) {
    printf("%d", s[i][2]);
  }
  printf("\n");
  
  for(int i = 0; i < 11; i++) {
    int current = s[i][2];
    int next = s[i+1][2];
    
    if(current == 1 && next == 1) {
      printf("Invalid sync transition: 1->1 between bytes %d and %d\n", i, i+1);
      sync_errors++;
    }
  }
  
  for(int i = 0; i < 12; i++) {
    if(s[i][0] != s[i][2] || s[i][1] != s[i][2]) {
      printf("Sync parity error in byte %d: ps0=%d, ps1=%d, s=%d\n", 
             i, s[i][0], s[i][1], s[i][2]);
      sync_errors++;
    }
  }
  
  if(sync_errors > 0) {
    printf("Total sync errors: %d\n", sync_errors);
    if(interrupt_flag) *interrupt_flag = 1;
  } else {
    printf("Sync sequence OK\n");
  }
}
