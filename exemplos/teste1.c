#include <stdio.h>
#include "../include/cthread.h"

void* printa4() {
    printf("funcao 4\n");
    cyield();
}

void* printa3() {
    printf("funcao 3\n");
    printf("%d\n", ccreate(printa4, NULL, 2));
    cyield();
}

void* printa2() {
    printf("funcao 2\n");
    printf("%d\n", ccreate(printa3, NULL, 3));
    cyield();
}

void* printa() {
    printf("funcao 1\n");
    printf("%d\n", ccreate(printa2, NULL, 4));
    cyield();
}

int main() {
    printf("%d\n", ccreate(printa, NULL, 5));
    cyield();

    char name[72] = "";
    char *namep = &name;
    int size = 72;
    cidentify(namep, size);
    printf("%s", name);

    return 0;
}
