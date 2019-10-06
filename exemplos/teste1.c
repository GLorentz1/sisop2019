#include <stdio.h>
#include "../include/cthread.h"

void* printa4() {
    printf("funcao 4\n");
    return;
}

void* printa3() {
    printf("funcao 3\n");
    printf("Thread criada e tid %d retornado\n", ccreate(printa4, NULL, 0));
    return;

}

void* printa2() {
    printf("funcao 2\n");
    printf("Thread criada e tid %d retornado\n", ccreate(printa3, NULL, 0));
    printf("Retorno da Cyield na funcao 2: %d\n", cyield());

    printf("Acabei funcao 2\n");
    return;

}

void* printa() {
    printf("funcao 1\n");
    printf("Thread criada e tid %d retornado\n", ccreate(printa2, NULL, 0));
    printf("Retorno da Cyield na funcao 1: %d\n", cyield());
    printf("saladinha/n");
    printf("salada/n");
    printf("saladao/n");
    return;

}

void* dummy() {
    printf("Retorno da Cyield na funcao dummy: %d\n", cyield());
    printf("Eu sou idiota\n");
    return;

}

int main() {
    int i;
    printf("Thread criada e tid %d retornado\n", ccreate(printa, (void *)&i, 0));
    printf("Thread criada e tid %d retornado\n", ccreate(printa2, (void *)&i, 0));
    printf("Thread criada e tid %d retornado\n", ccreate(printa2, (void *)&i, 0));
    printf("Thread criada e tid %d retornado\n", ccreate(dummy, (void *)&i, 0));
    cyield();
    printf("Segundo yield da main\n");
    cyield();
    //char name[72] = "";
    //char *namep = &name;
    //int size = 72;
    //cidentify(namep, size);
    printf("Fim da main\n");

    return 0;
}
