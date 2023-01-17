#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "list.h"
#include "list.c"

void printInt (element_t element) {
  int* e = (int*) element;
  
  printf ("%d\n", e);
}

void printStr (element_t element) { //set a char pointer then print the element 
  char* charE = element;
  
  if (charE == NULL) {
      printf("NULL\n");
  } else {
      printf("%s\n", charE);
  }
}

void strToInt (element_t* point, element_t element) {
    char *e1 = NULL;
    char *e2 = (char*) element;

    intptr_t *p = (intptr_t*) point;
    *p = strtol(e2, &e1, 10);

    if (e1 == e2)
      *p = -1;
}

void strSet (element_t* pointer, element_t element1, element_t element2) {
    char **p = (char**) pointer;

    if ((intptr_t)element2 < 0) {
        *p = (char *)element1;
    } else {
        *p = NULL;
    }
}

//CHECK STUFF MIDWAY FOR FILTERS 100% sure this is right lmao
int isPos (element_t element) {
    return ((intptr_t) element) >= 0;
}

int isntNull (element_t element) {
    return ((char*) element) != NULL;
}

void truncate (element_t* pointer, element_t element1, element_t element2) {
    char **p = (char**) pointer;
    char *e1 = (char*) element1;
    
    if (strlen(e1) < ((intptr_t) element2)) {
        *p = e1;
        return;
    }

    intptr_t x = (intptr_t)element2;
    e1[(int) x] = 0;
    *p = e1;
}

void max (element_t* pointer, element_t element1, element_t element2) {
    intptr_t *p = (intptr_t*) pointer;

    if (((intptr_t)element1) >= ((intptr_t)element2)) {
        *p = ((intptr_t)element1);
    } else {
        *p = ((intptr_t)element2);
    }
}

void list_printall(struct list *l) {
    for (int i = 0; i < list_len(l); i++) {
        printf("%s", l->data[i]);
        if ((i + 1) != list_len(l)) {
            printf(" ");
        }
    }

    printf("\n");
}

int main(int argc, char **argv){
    struct list* list = list_create();
    for(int i=1; i<argc; i++){
        list_append(list, (element_t)argv[i]);
    }
    
    struct list* l1 = list_create();
    struct list* l2 = list_create(); 
    struct list* l3 = list_create();
    struct list* l4 = list_create();
    struct list* l5 = list_create();
    
    list_map1 (strToInt, l1, list);
    list_map2 (strSet, l2, list, l1);
    list_filter (isPos, l3, l1);
    list_filter(isntNull, l4, l2);
    list_map2(truncate, l5, l4, l3);
    list_foreach(printStr, l5);
    list_printall(l5);

    intptr_t x = -1;
    list_foldl (max, (element_t*) &x, l3);
    printf("%d\n", x);

    list_destroy (list);
    list_destroy (l1);
    list_destroy (l2);
    list_destroy (l3);
    list_destroy (l4);
    list_destroy (l5);
}