#include <stdlib.h>
#include <stdio.h>

// YOU: Allocate these global variables, using these names
int  d, h;
int* c;
int  z[10];

int main (int argv, char** argc) {
  // Ignore this block of code
  if (argv != 11) {
    fprintf (stderr, "usage: z[0] ... z[9]\n");
    exit (EXIT_FAILURE);
  }
  for (int a=0; a<10; a++)
    z[a] = atol (argc[1 + a]);

  // YOU: Implement this code
  d  = z[3];
  d  = z[d];
  c  = &h;
  *c = 7;
  c  = &z[z[9]];
  *c = *c + z[5];

  // Ignore this block of code
  printf ("d=%d h=%d c=&z[%d] z={", d, h, c - z);
  for (int a=0; a<10; a++)
    printf("%d%s", z[a], a<9? ", ": "}\n");
  }
