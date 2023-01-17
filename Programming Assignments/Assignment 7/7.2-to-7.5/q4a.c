#include <stdio.h>
#include <stdlib.h>

int* array;

void helperFunc(int a, int b) {
  array[b] += a;
}

int main(int argc, char **argv) {
  array = malloc(10*sizeof(int));

  int var1 = 1;
  int var2 = 2;

  helperFunc(3, 4);
  helperFunc(var1, var2);
  for (int i = 0; i < 10; i++) {
    printf("%d\n",array[i]);
  }
}
