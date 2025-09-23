#ifndef RC
#define RC

void hex_to_byte(int hex, byte* b);

void read_package(FILE* fstream, byte* pkg, int* zf, int* c1, int* interrupt_flag);

void data_parity(byte* pkg, int* interrupt_flag);

void simple_data_parity(byte* pkg, int* interrupt_flag);

#endif
