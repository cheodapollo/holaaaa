#include <stdio.h>
#include <string.h>

void main() {
  FILE *f = popen("/sbin/ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'", "r");
  char array[50];
  fgets(array, sizeof(array), f);
  printf("\nGot this: %s\n", array);
  pclose(f);
}

