#ifndef RC
#define RC

void hex_to_byte(int hex, byte* b);

void read_package(FILE* fstream, byte* pkg, char interrupt_flag);

#endif
