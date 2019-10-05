
#include <stdio.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"

#define	PROCST_APTO	0		/* Processo em estado apto a ser escalonado */
#define	PROCST_EXEC	1		/* Processo em estado de execução */
#define	PROCST_BLOQ	2		/* Processo em estado bloqueado */
#define	PROCST_TERMINO	3	/* Processo em estado de terminado */

bool initialized = false;

int currentTid = 0;

ucontext_t contextEscalonador;

FILA2 filaAptos;
FILA2 filaBloqueado;
FILA2 filaExecutando;

PFILA2 pfilaAptos;
PFILA2 pfilaBloqueados;
PFILA2 pfilaExecutando;

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
        pfilaBloqueado = &filaBloqueado;
        pfilaExecutando = &filaExecutando;

        if ( CreateFila2(pfilaAptos) != 0)
            exit(-1);

        if ( CreateFila2(pfilaBloqueados) != 0)
            exit(-1);

        if ( CreateFila2(pfilaExecutando) != 0)
            exit(-1);

        initialized = true;

        TCB_t *mainThread;
        mainThread = malloc(sizeof(TCB_t));
        mainThread->tid = 0;
        mainThread->state = PROCST_EXEC;
        mainThread->prio = 0;
        getcontext(&(mainThread->context));

        return 0;
    }
    else
    {
        return -1;
    }
}

void escalonador()
{


}

int ccreate (void* (*start)(void*), void *arg, int prio) {
    init()

    TCB_t *newThread = malloc(sizeof(TCB_t))

    ucontext_t newContext;
    ucontext_t* newContextP;

    newThread->tid = ++currentTid;
    newThread->state = PROCST_APTO;
    newThread->prio = 0;
    getcontext(&(newThread->context));

    newThread->context.uc_stack.ss_sp = malloc(sizeof(SIGSTKSZ));
    newThread->context.uc_stack.ss_size = SIGSTKSZ;
    newThread->context.uc_stack.ss_flags = 0;
    newThread->context.uc_link = &escalonador

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

    currentThread = (TCB_t *)GetAtIteratorFila2(pfilaExecutando);

    currentThread->state = PROCST_APTO;
    // salva o contexto na thread
    if (getcontext(&(currentThread->context)) == -1)
    {
        return -1;
    }

    if (runningThread->state == PROCST_APTO){
        scheduler();
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

int main() {
    char name[71] = "";
    char *namep = &name;
    int size = 71;
    cidentify(namep, size);
    printf("%s", name);
    return 0;
}

