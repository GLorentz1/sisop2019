#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"

#define	PROCST_APTO	0		/* Processo em estado apto a ser escalonado */
#define	PROCST_EXEC	1		/* Processo em estado de execução */
#define	PROCST_BLOQ	2		/* Processo em estado bloqueado */
#define	PROCST_TERMINO	3	/* Processo em estado de terminado */

int initialized = 0;

int currentTid = 0;

ucontext_t contextEscalonador;

FILA2 filaAptos;
FILA2 filaBloqueados;
FILA2 filaExecutando;

PFILA2 pfilaAptos;
PFILA2 pfilaBloqueados;
PFILA2 pfilaExecutando;

void escalonador()
{
    //parar de contar tempo

    if(NextFila2(pfilaAptos) != -NXTFILA_VAZIA)
    {
        FirstFila2(pfilaAptos);

        TCB_t *maiorPrioTCB = (TCB_t *)GetAtIteratorFila2(pfilaAptos);
        TCB_t *nodoAtual = NULL;

        float menorTempo = maiorPrioTCB->prio;
        float tempoNodoAtual = menorTempo;

        while(NextFila2(pfilaAptos) != -NXTFILA_ENDQUEUE)
        {
            nodoAtual = GetAtIteratorFila2(pfilaAptos);
            tempoNodoAtual = nodoAtual->prio;

            if (tempoNodoAtual < menorTempo)
            {
                //printf("Achei thread com prioriddade %d e a menor era %d\n", )
                menorTempo = tempoNodoAtual;
                maiorPrioTCB = nodoAtual;
            }
        }

        AppendFila2(pfilaExecutando, maiorPrioTCB);

        printf("escalonei thread %d com prioridade %d\n", maiorPrioTCB->tid, maiorPrioTCB->prio);

        if (setcontext(&(maiorPrioTCB->context)) == -1)
        {
            exit(-1);
        }
    }
}

int init()
{
    if (!initialized)
    {
        getcontext(&contextEscalonador);

        contextEscalonador.uc_link = NULL;
        contextEscalonador.uc_stack.ss_sp = malloc(sizeof(SIGSTKSZ));
        contextEscalonador.uc_stack.ss_size = SIGSTKSZ;
        contextEscalonador.uc_stack.ss_flags = 0;

        makecontext(&contextEscalonador, (void (*)(void))escalonador, 0);

        pfilaAptos = &filaAptos;
        pfilaBloqueados = &filaBloqueados;
        pfilaExecutando = &filaExecutando;

        if ( CreateFila2(pfilaAptos) != 0)
            exit(-1);

        if ( CreateFila2(pfilaBloqueados) != 0)
            exit(-1);

        if ( CreateFila2(pfilaExecutando) != 0)
            exit(-1);

        initialized = 1;

        TCB_t *mainThread;
        mainThread = malloc(sizeof(TCB_t));
        mainThread->tid = 0;
        mainThread->state = PROCST_EXEC;
        mainThread->prio = 10;
        getcontext(&(mainThread->context));

        if (AppendFila2(pfilaExecutando, mainThread) != 0){
            exit(-1);
        }
        else{
            FirstFila2(pfilaExecutando);
        }

        printf("criei thread main com tid %d e ela esta no estado %d, com prioridade %d e contexto %d\n", mainThread->tid, mainThread->state, mainThread->prio, mainThread->context.uc_stack.ss_size);

        return 0;
    }
    else
    {
        printf("ja inicializou\n");
        return -1;
    }
}

int ccreate (void* (*start)(void*), void *arg, int prio) {

    init();

    TCB_t *newThread = malloc(sizeof(TCB_t));

    newThread->tid = ++currentTid;
    newThread->state = PROCST_APTO;
    newThread->prio = prio;
    getcontext(&(newThread->context));

    newThread->context.uc_stack.ss_sp = malloc(sizeof(SIGSTKSZ));
    newThread->context.uc_stack.ss_size = SIGSTKSZ;
    newThread->context.uc_stack.ss_flags = 0;
    newThread->context.uc_link = &contextEscalonador;

    makecontext(&(newThread->context), (void (*)(void))start, 1, arg);


    if (AppendFila2(pfilaAptos, newThread) != 0){
        exit(-1);
    }
    else{
        FirstFila2(pfilaAptos);
    }

	return newThread->tid;
}

int cyield(void) {

    init();

    FirstFila2(pfilaExecutando);

    TCB_t *currentThread = (TCB_t *)GetAtIteratorFila2(pfilaExecutando);

    DeleteAtIteratorFila2(pfilaExecutando);

    if (AppendFila2(pfilaAptos, currentThread) != 0){
        exit(-1);
    }

    printf("depois de pehgarr thera %d\n", currentThread->tid);

    currentThread->state = PROCST_APTO;
    // salva o contexto na thread
    if (getcontext(&(currentThread->context)) == -1)
    {
        return -1;
    }

    if (currentThread->state == PROCST_APTO){
        escalonador();
    }
    else {
        return 0;
    }
    return 0;
}

int cjoin(int tid) {

    init();

	return -1;
}

int csem_init(csem_t *sem, int count) {

    init();

	return -1;
}

int cwait(csem_t *sem) {

    init();

	return -1;
}

int csignal(csem_t *sem) {

    init();

	return -1;
}

int cidentify (char *name, int size) {

    init();

	strncpy (name, "Demetrio Boeira - 297693\nGustavo Lorentz - 287681\nPedro Weber - 287678\n", size);
	return 0;
}

