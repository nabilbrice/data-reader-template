#ifndef DRIVER_TABLE_READER_H_
#define DRIVER_TABLE_READER_H_

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
  double *data;
  int stride_energy;
  int stride_log_Rs;
  int stride_log_Ts;
  double energies[DIM_R];
  double log_Ts[DIM_S];
  double log_Rs[DIM_F];
};
typedef struct Table Table;

double *get_table_row(Table *, int, int, int);

void initialise_table_from_file(Table *, char *);

void free_table(Table *);

int closest_indices(double *, int, double);

double interpolated_value(Table *, double, double, double, int);

#endif // DRIVER_TABLE_READER_H_
