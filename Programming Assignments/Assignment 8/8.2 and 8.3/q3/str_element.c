#include "str_element.h"

/* TODO: Implement all public str_element functions, including element interface functions. */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "refcount.h"
#include <string.h>

struct str_element {
    struct str_element_class *class;
    char * value;
};

struct str_element_class {
    void (*print)(struct element *this);
    int (*compare)(struct element *e1, struct element *e2);
    char * (*str_element_get_value)(struct str_element *this);
    int (*is_str_element)(struct element *this);
};

char * str_element_get_value(struct str_element *this) {
    struct str_element *se = this;

    if (se == NULL) {
        return NULL;
    }

    return this->value;
}

void se_print(struct element *this) {
    struct str_element *e = (struct str_element *)this;

    for (int i = 0; i < strlen(e->value); i++)
        printf("%c", e->value[i]);
}

int se_compare(struct element *e1, struct element *e2) {
    if (is_str_element(e1) == 1 && is_str_element(e2) == 0) { // int_element is always less than str element
        return 1;
    } else if (is_str_element(e1) == 0 && is_str_element(e2) == 1) {
        return -1;
    }

    if (is_str_element(e1) == 1) {
        struct str_element *this_e1 = (struct str_element *)e1;
        struct str_element *this_e2 = (struct str_element *)e2;

        return strcmp(this_e1->value, this_e2->value);
    } else {
        return e1->class->compare(e1, e2);
    }
}

struct str_element_class str_element_class_obj = {se_print, se_compare, str_element_get_value, is_str_element};

void str_element_finalizer(void *this) {
    struct str_element * se = (struct str_element *)this;

    free(se->value);
    se = NULL;
}

struct str_element *str_element_new(char *value) {
  struct str_element *se = rc_malloc(sizeof(struct str_element), str_element_finalizer);
  se->class = &str_element_class_obj;
  se->value = strdup(value);
  return se;
}

int is_str_element(struct element *this) {
    struct str_element * se = (struct str_element *)this;

    if (se->class == &str_element_class_obj) {
        return 1;
    } else {
        return 0;
    }
}