#include <stdio.h>
#include <ucontext.h>
#include "../include/cthread.h"

csem_t* sem;

void* printa4() {
    printf("funcao 4\n");
    return;
}

void* printa3() {
    int tid = ccreate(printa4, NULL, 0);
    printf("funcao 3\n");
    cwait(sem);
    printf("Thread criada e tid %d retornado\n", tid);
    return;
}

void* printa2() {
    int tid = ccreate(printa3, NULL, 0);
    cwait(sem);
    printf("funcao 2\n");
    printf("Thread criada e tid %d retornado\n", tid);
    printf("Retorno da Cyield na funcao 2: %d\n", cyield());
    printf("Vou esperar tid %d\n", tid);
    csignal(sem);
    cjoin(tid);

    printf("Acabei funcao 2\n");
    return;

}

void* printa() {
    int tid = ccreate(printa2, NULL, 0);
    printf("funcao %d\n", SIGSTKSZ);
    printf("Thread criada e tid %d retornado\n", tid);
    printf("Retorno da Cyield na funcao 1: %d\n", cyield());
    printf("Vou esperar tid %d\n", tid);
    printf("Retorno da Cjoin da funcao 1: %d\n", cjoin(tid));
    printf("Voltei da join funcao 1\n");
    sleep(3);
    cyield();
    cyield();
    return;

}

void* dummy() {
    printf("Retorno da Cyield na funcao dummy: %d\n", cyield());
    printf("Eu sou a dummy\n");
    return;

}

int main() {
    int i = 1;
    sem = (csem_t*) malloc(sizeof(csem_t));

    csem_init(sem, 1);
    int tid1 = ccreate(printa, (void *)&i, 0);
    printf("Thread criada e tid %d retornado\n", tid1);
    cyield();
    cjoin(tid1);
    int tid2 = ccreate(dummy, (void *)&i, 0);
    printf("Thread criada e tid %d retornado\n", tid2);
    cyield();
    cjoin(tid2);

    char name[72] = "";
    char *namep = &name;
    int size = 72;
    cidentify(namep, size);

    printf("Fim da main\n");

    return 0;
}
