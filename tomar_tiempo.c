#include <stdio.h>
#include <string.h>

void main() {
  FILE *f = popen("date +%s", "r");
  char array[50];
  fgets(array, sizeof(array), f);
  printf("\nGot this: %s\n", array);
  pclose(f);
}
