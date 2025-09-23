#ifndef TR
#define TR
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

void read_sensor(FILE* fstream, byte* triad[3]);

void create_package(byte* pkg, FILE* fstream);

void print_package(byte* pkg);

void byte_to_hex(byte b, char* dest);

void write_package(FILE* dest, byte* pkg);

#endif
