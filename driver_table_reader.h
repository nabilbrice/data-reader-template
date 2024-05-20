#ifndef DRIVER_TABLE_READER_H_
#define DRIVER_TABLE_READER_H_

#include <stdio.h>

// currently the information about dimensions of the data
// have to be given here:
// giving the incorrect dimensions results in a garbled table
#define DIM_R 301
#define DIM_C 6
#define DIM_F 56
#define DIM_S 22
// a data structure to hold the table information,
// including the values of the headers
struct Table {
  float *data;
  int stride_energy;
  int stride_log_Rs;
  int stride_log_Ts;
  float energies[DIM_R];
  float log_Ts[DIM_S];
  float log_Rs[DIM_F];
};
typedef struct Table Table;

float *get_table_row(Table *, int, int, int);

void initialise_table_from_file(Table *, char *);

void free_table(Table *);

#endif // DRIVER_TABLE_READER_H_
