#include <stdio.h>
#include <stdlib.h>

int x[] = {1,2,3,-1,-2,0,184,340057058};
int y[] = {0,0,0,0,0,0,0,0};

int f(int xcount) {
  int copy = xcount;
  int numAppearances = 0;

  while(copy != 0) {
    if((0x80000000 & copy) != 0)
      numAppearances++;
    copy *= 2;
  }

  return numAppearances;
}

int main() {
  int count = 8;

  while(count > 0) {
    count--;
    y[count] = f(x[count]);
  }

  for (int i = 0; i < 8; i++) {
    printf("%d\n", x[i]);
  }
  for (int i = 0; i < 8; i++) {
    printf("%d\n", y[i]);
  }
}