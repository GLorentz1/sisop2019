#include <stdio.h>
#include "../include/cthread.h"

void* printa4() {
    printf("funcao 4\n");
    return;
}

void* printa3() {
    int tid = ccreate(printa4, NULL, 0);
    printf("funcao 3\n");
    printf("Thread criada e tid %d retornado\n", tid);
    return;
}

void* printa2() {
    //int tid = ccreate(printa3, NULL, 0);
    printf("funcao 2\n");
    //printf("Thread criada e tid %d retornado\n", tid);
    //printf("Vou esperar tid %d\n", tid);
    printf("Retorno da Cyield na funcao 2: %d\n", cyield());
    //printf("Vou esperar tid %d\n", tid);
    //cjoin(tid);

    printf("Acabei funcao 2\n");
    return;

}

void* printa() {
    int tid = ccreate(printa2, NULL, 0);
    printf("funcao 1\n");
    printf("Thread criada e tid %d retornado\n", tid);
    printf("Retorno da Cyield na funcao 1: %d\n", cyield());
    cjoin(tid);
    //printf("saladinha/n");
    //printf("salada/n");
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
    int tid1 = ccreate(printa, (void *)&i, 0);
    printf("Thread criada e tid %d retornado\n", tid1);
    cyield();
    cjoin(tid1);
    int tid2 = ccreate(dummy, (void *)&i, 0);
    printf("Thread criada e tid %d retornado\n", tid2);
    cyield();
    cjoin(tid2);

    //char name[72] = "";
    //char *namep = &name;
    //int size = 72;
    //cidentify(namep, size);
    printf("Fim da main\n");

    return 0;
}
