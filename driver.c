#include <stdio.h>
#include <stdlib.h>

const int TABLE_SIZE_E = 10;
const int TABLE_SIZE_P = 2;

void readtable(FILE *file, float subtable[TABLE_SIZE_E][TABLE_SIZE_P]) {
  char buff[2048];

  int i;

  i = 0;
  while (fgets(buff, sizeof(buff), file)) {
    if (buff[0]=='#') {
      continue;
    }
    sscanf(buff, "%f %f", &subtable[i][0], &subtable[i][1]);
    printf("subtable: %f %f \n", subtable[i][0], subtable[i][1]);
    i++;
  };
}

int main() {

  // the table is actually bigger than necessary in entries
  float table[2][TABLE_SIZE_E][TABLE_SIZE_P];

  FILE *file;
  char buff[2048]; // maximum sized buffer

  int i, j, k, l;

  file = fopen("example.dat", "r");
  j = 0;
  for (int j=0; (j < 2); j++) {
    readtable(file, table[j]);
  };

  k = 4;
  l = 1;
  printf("For k = %d in table %d\n", k, l+1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", table[l][k][0], table[l][k][1]);


  return 0;

}
