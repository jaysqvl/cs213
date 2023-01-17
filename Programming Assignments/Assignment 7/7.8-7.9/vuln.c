#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct X {
    int len;
    char * str;
};

char _str1[] = "Welcome! Please enter a name:"; // 30 is 29 chars + 1 ending bit
char _str2[] = "Good luck, "; // 11 is 10 chars + 1 ending bit
char _str3[] = "The secret phrase is 'squeamish ossifrage'"; // 43 is 42 chars + 1 ending bit (not including back slashes)

void proof(struct X * str) {
    struct X * str3 = malloc(4 + (43)*sizeof(char));
    str3->len = 43;
    str3->str = (char *)malloc((str3->len)*sizeof(char));
    printStr(str3);
}

int printStr(struct X * str) {
    write(1, str->str, str->len);
    printf("\n");
}

int main() {
    struct X * str1 = malloc(4 + (30)*sizeof(char));
    str1->len = 30;
    str1->str = (char *)malloc((str1->len)*sizeof(char));
    str1->str = _str1;
    printStr(str1);

    char buf1[256];
    int size = read(0, buf1, 256);

    struct X * str2 = malloc(4 + (11)*sizeof(char));
    str2->len = 11;
    str2->str = (char *)malloc((str2->len)*sizeof(char));
    str2->str = _str2;
    printStr(str2);
    write(1,buf1, size);
}
