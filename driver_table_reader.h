#ifndef DRIVER_TABLE_READER_H_
#define DRIVER_TABLE_READER_H_

// currently the information about dimensions of the data
// have to be given here:
// giving the incorrect dimensions results in a garbled table
#define DIM_ROWS 301
#define DIM_COLS 6
#define DIM_FAST 56
#define DIM_SLOW 22
// a row of data values
// with known compile-time size
typedef struct TableRow {
  double entry[DIM_COLS];
} TableRow;
// 2D subtable with known compile-time size
typedef struct SizedTable {
  TableRow row[DIM_ROWS];
} SizedTable;
// a data structure to hold all the tables,
// including the values of the headers
// dynamically allocated at run-time
typedef struct TableCollection {
  double row_labels[DIM_ROWS];
  SizedTable *table;
  double log_Ts[DIM_SLOW];
  double log_Rs[DIM_FAST];
} TableCollection;

int hash_label(double, double);

TableRow *get_table_row_from_key(TableCollection *, int, double, double);

TableRow *get_table_row(TableCollection *, int, int, int);

void initialise_tables_from_file(TableCollection *, char *);

void drop_tables(TableCollection *);

#endif // DRIVER_TABLE_READER_H_
