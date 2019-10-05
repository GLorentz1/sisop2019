#include <stdio.h>
#include "../include/cthread.h"

void* printa4() {
    printf("funcao 4\n");
}

void* printa3() {
    printf("funcao 3\n");
    printf("Thread criada e tid %d retornado\n", ccreate(printa4, NULL, 2));
}

void* printa2() {
    printf("funcao 2\n");
    printf("Thread criada e tid %d retornado\n", ccreate(printa3, NULL, 3));
}

void* printa() {
    printf("funcao 1\n");
    printf("Thread criada e tid %d retornado\n", ccreate(printa2, NULL, 4));
}

int main() {
    printf("Thread criada e tid %d retornado\n", ccreate(printa, NULL, 5));
    cyield();

    //char name[72] = "";
    //char *namep = &name;
    //int size = 72;
    //cidentify(namep, size);
    printf("o");

    return 0;
}
