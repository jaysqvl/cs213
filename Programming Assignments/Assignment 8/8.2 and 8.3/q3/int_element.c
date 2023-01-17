#include "int_element.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "refcount.h"

/* TODO: Implement all public int_element functions, including element interface functions.

You may add your own private functions here too. */

struct int_element {
    struct int_element_class *class;
    int value;
};

struct int_element_class {
    void (*print)(struct element *this);
    int (*compare)(struct element *e1, struct element *e2);
    int (*int_element_get_value)(struct int_element *this);
    int (*is_int_element)(struct element *this);
};

int int_element_get_value(struct int_element *this) {
    struct int_element *ie = this;

    if (ie == NULL) {
        return -1;
    }

    return this->value;
}

void ie_print(struct element *this) {
    struct int_element *e = (struct int_element *)this;

    printf("%d", e->value);
}

int ie_compare(struct element *e1, struct element *e2) {
    if (is_int_element(e1) == 1 && is_int_element(e2) == 0) { // int_element is always less than str element
        return -1;
    } else if (is_int_element(e1) == 0 && is_int_element(e2) == 1) {
        return 1;
    }

    if (is_int_element(e1) == 1) {
        struct int_element *this_e1 = (struct int_element *)e1;
        struct int_element *this_e2 = (struct int_element *)e2;

        if (this_e1->value < this_e2->value) {
            return -1;
        } else if (this_e1->value > this_e2->value) {
            return 1;
        } else {
            return 0;
        }

    } else {
        return e1->class->compare(e1, e2);
    }
}

struct int_element_class int_element_class_obj = {ie_print, ie_compare, int_element_get_value, is_int_element};

void int_element_finalizer(void *this) {
    struct int_element * ie = (struct int_element *)this;
    ie = NULL;
}

struct int_element *int_element_new(int value) {
  struct int_element *ie = rc_malloc(sizeof(struct int_element), int_element_finalizer);
  ie->class = &int_element_class_obj;
  ie->value = value;
  return ie;
}

int is_int_element(struct element *this) {
    struct int_element * ie = (struct int_element *)this;

    if (ie->class == &int_element_class_obj) {
        return 1;
    } else {
        return 0;
    }
}