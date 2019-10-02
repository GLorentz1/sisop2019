
#include <stdio.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"

bool initialized = false;

int currentTid = 0;

PFILA2 filaAptos;
PFILA2 filaBloqueados;
PFILA2 filaExecutando;

int init()
{
    if (!initialized)
    {
        CreateFila2(filaAptos);
        CreateFila2(filaBloqueados);
        CreateFila2(filaExecutando);



        initialized = true;
        return 0;
    }
    else
    {
        return -1;
    }
}

int ccreate (void* (*start)(void*), void *arg, int prio) {
    init()

    TCB_t newThread;
    TCB_t* newThreadP = &newThread;

    ucontext_t newContext;
    ucontext_t* newContextP;

    getcontext(newContext);
    makecontext(newContext, start, arg);

    newThread->tid = ++currentTid;
    newThread->state = 0;
    newThread->prio = 0;
    newThread->context = newContext;

    AppendFila2(filaAptos, newThread);

	return -1;
}

int cyield(void) {

    init()

	return -1;
}

int cjoin(int tid) {

    init()

	return -1;
}

int csem_init(csem_t *sem, int count) {

    init()

	return -1;
}

int cwait(csem_t *sem) {

    init()

	return -1;
}

int csignal(csem_t *sem) {

    init()

	return -1;
}

int cidentify (char *name, int size) {

    init()

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

