#ifndef RC
#define RC

void hex_to_byte(int hex, byte* b);

void read_package(FILE* fstream, byte* pkg, int zf, int interrupt_flag);

void data_parity(byte* pkg, int interrupt_flag);

#endif
