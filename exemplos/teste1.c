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
    printf("Acabei funcao 2\n");
}

void* printa() {
    printf("funcao 1\n");
    printf("Thread criada e tid %d retornado\n", ccreate(printa2, NULL, 4));
    //printf("Retorno da Cyield na funcao 1: %d\n", cyield());
    printf("saladinha/n");
    printf("salada/n");
    printf("saladao/n");
}

void* dummy() {
    printf("Retorno da Cyield na funcao dummy: %d\n", cyield());
    printf("Eu sou idiota\n");
}

int main() {
    printf("Thread criada e tid %d retornado\n", ccreate(printa, NULL, 5));
    printf("Thread criada e tid %d retornado\n", ccreate(printa2, NULL, 4));
    printf("Thread criada e tid %d retornado\n", ccreate(printa, NULL, 3));
    printf("Thread criada e tid %d retornado\n", ccreate(printa, NULL, 6));
    printf("Thread criada e tid %d retornado\n", ccreate(dummy, NULL, 9));
    cyield();
    cyield();
    scanf("%d");
    //char name[72] = "";
    //char *namep = &name;
    //int size = 72;
    //cidentify(namep, size);
    printf("Fim da main\n");

    return 0;
}
