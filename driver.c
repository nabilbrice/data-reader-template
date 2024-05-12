#include <stdio.h>
#include <stdlib.h>

const int TABLE_SIZE_E = 10;
const int TABLE_SIZE_P = 2;

float *get_table_value(float *table, int i, int j, int k) {
  int stride_k = TABLE_SIZE_E * TABLE_SIZE_P;
  int stride_i = TABLE_SIZE_P;
  return &table[stride_k * k + stride_i * i + j];
}

void readtable(FILE *file, float *table, int k) {
  char buff[2048];

  int i;

  i = 0;
  while (fgets(buff, sizeof(buff), file)) {
    if (buff[0]=='#') {
      continue;
    }
    sscanf(buff, "%f %f", get_table_value(table, i, 0, k), get_table_value(table, i, 1, k));
    printf("subtable: %f %f \n", *get_table_value(table, i, 0, k), *get_table_value(table, i, 1, k));
    i++;
  };
}


int main() {

  // the table is actually bigger than necessary in entries
  float table[2][TABLE_SIZE_E][TABLE_SIZE_P];

  FILE *file;
  char buff[2048]; // maximum sized buffer

  int i, j, k, l;

  float *boxed_table = malloc( TABLE_SIZE_E * TABLE_SIZE_P * 2 * sizeof(boxed_table));

  file = fopen("example.dat", "r");
  j = 0;
  for (int j=0; (j < 2); j++) {
    readtable(file, boxed_table, j);
  };

  k = 4;
  l = 1;
  printf("For k = %d in table %d\n", k, l+1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", *get_table_value(boxed_table, k, 0, l), 
    *get_table_value(boxed_table, k, 1, l));


  free(boxed_table);
  return 0;

}
