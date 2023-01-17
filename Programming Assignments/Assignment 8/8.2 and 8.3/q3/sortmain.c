#include <stdio.h>
#include <stdlib.h>

#include "int_element.h"
#include "str_element.h"
#include "element.h"

/* If the string is numeric, return an int_element. Otherwise return a str_element. */
struct element *parse_string(char *str) {
  char *endp;
  /* strtol returns a pointer to the first non-numeric character in endp.
     If it gets to the end of the string, that character will be the null terminator. */
  int value = strtol(str, &endp, 10);
  if(str[0] != '\0' && endp[0] == '\0') {
    /* String was non-empty and strtol conversion succeeded - integer */
    return (struct element *)int_element_new(value);
  } else {
    return (struct element *)str_element_new(str);
  }
}

int compare(const void *this1, const void *this2) {
  struct element * e1 = *(struct element **)this1;
  struct element * e2 = *(struct element **)this2;
  
  return e1->class->compare(e1, e2);
}

int main(int argc, char **argv) {
  int size = argc - 1;
  /* TODO: Read elements into a new array using parse_string */
  struct element *list[size]; // each element in the list[i] is a pointer to an element

  for (int i = 1; i < argc; i++) {
    list[i - 1] = parse_string(argv[i]); // each pointer to an element points to a corresponding element pointer
  }

  qsort(list, size, sizeof(struct element *), compare);

  /* TODO: Sort elements with qsort */
  printf("Sorted: ");

  for (int i = 0; i < size; i++) {
    list[i]->class->print(list[i]);
    printf(" ");
    rc_free_ref(list[i]);
    /* TODO: Print elements, separated by a space */
  }

  printf("\n");
}
