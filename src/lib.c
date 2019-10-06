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
ucontext_t contextThreadKiller;

FILA2 filaAptos;
FILA2 filaBloqueados;
FILA2 filaExecutando;

PFILA2 pfilaAptos;
PFILA2 pfilaBloqueados;
PFILA2 pfilaExecutando;

int threadKiller()
{
    FirstFila2(pfilaExecutando);

    TCB_t *currentThread = (TCB_t *)GetAtIteratorFila2(pfilaExecutando);
    currentThread->state = PROCST_TERMINO;

    printf("Thread Killer: Matei processo com tid %d\n", currentThread->tid);

    printf("Thread Killer: Chamando escalonador\n");
    escalonador();

    return 1;

}

void escalonador()
{
    unsigned int novaPrioridade = stopTimer();//parar de contar tempo

    printf("Entrando no escalonador...\n");
    FirstFila2(pfilaAptos);
    TCB_t *currentThread = (TCB_t *)GetAtIteratorFila2(pfilaExecutando);
    printf("Nova prioridade da thread %d eh %u\n", currentThread->tid, novaPrioridade);

    currentThread->prio = novaPrioridade;

    //todos os casos (State = bloq / apto / terminado tiram processo de executando
    FirstFila2(pfilaExecutando);
    DeleteAtIteratorFila2(pfilaExecutando);
//
//    currentThread->state = PROCST_APTO;

    //se ele eh apto bota na fila de aptos, se eh bloq bota em bloqueados, se for terminado nao faz nada e nunca vai ser exec
    if (currentThread->state == PROCST_APTO)
    {
        if (AppendFila2(pfilaAptos, currentThread) != 0)
        {
            exit(-1);
        }
    }
    else if (currentThread->state == PROCST_BLOQ)
    {
        if (AppendFila2(pfilaBloqueados, currentThread) != 0)
        {
            exit(-1);
        }
    }

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
                //printf("Achei thread com prioridade %f e a menor era %f\n", tempoNodoAtual, menorTempo);
                menorTempo = tempoNodoAtual;
                maiorPrioTCB = nodoAtual;
            }
        }

        FirstFila2(pfilaAptos);
        nodoAtual = GetAtIteratorFila2(pfilaAptos);
        if (nodoAtual->tid == maiorPrioTCB->tid)
        {
            DeleteAtIteratorFila2(pfilaAptos);
        }
        else
        {
            while(NextFila2(pfilaAptos) != -NXTFILA_ENDQUEUE)
            {
                nodoAtual = GetAtIteratorFila2(pfilaAptos);
                if (nodoAtual->tid == maiorPrioTCB->tid)
                {
                    DeleteAtIteratorFila2(pfilaAptos);
                    break;
                }
            }
        }


        maiorPrioTCB->state = PROCST_EXEC;
        AppendFila2(pfilaExecutando, maiorPrioTCB);

        printf("Escalonador: Colocando thread %d com prioridade %d no estado executando\n\n", maiorPrioTCB->tid, maiorPrioTCB->prio);

        startTimer();
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
        startTimer();
        getcontext(&contextEscalonador);

        contextEscalonador.uc_link = NULL;
        contextEscalonador.uc_stack.ss_sp = malloc(sizeof(SIGSTKSZ));
        contextEscalonador.uc_stack.ss_size = SIGSTKSZ;
        contextEscalonador.uc_stack.ss_flags = 0;

        makecontext(&contextEscalonador, (void *)escalonador, 0);

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
        mainThread->join = -1;
        getcontext(&(mainThread->context));

        mainThread->context.uc_link = 0;

        if (AppendFila2(pfilaExecutando, mainThread) != 0)
        {
            exit(-1);
        }
        else
        {
            FirstFila2(pfilaExecutando);
        }

        getcontext(&contextThreadKiller);

        contextThreadKiller.uc_stack.ss_sp = malloc(sizeof(SIGSTKSZ));
        contextThreadKiller.uc_stack.ss_size = SIGSTKSZ;
        contextThreadKiller.uc_stack.ss_flags = 0;
        contextThreadKiller.uc_link = &contextEscalonador;

        makecontext(&contextThreadKiller, (void *)threadKiller, 0);

        printf("Estruturas inicializadas e thread main criada com tid %d\n", mainThread->tid);

        return 0;
    }
    else
    {
        printf("Funcao init: Estruturas ja foram inicializadas!\n");
        return -1;
    }
}

int ccreate (void* (*start)(void*), void *arg, int prio)
{
    printf("Ccreate: ");
    init();

    TCB_t *newThread = malloc(sizeof(TCB_t));

    newThread->tid = ++currentTid;
    newThread->state = PROCST_APTO;
    newThread->prio = prio;
    newThread->join = -1;
    getcontext(&(newThread->context));

    newThread->context.uc_stack.ss_sp = malloc(sizeof(SIGSTKSZ));
    newThread->context.uc_stack.ss_size = SIGSTKSZ;
    newThread->context.uc_stack.ss_flags = 0;
    newThread->context.uc_link = &contextThreadKiller;

    makecontext(&(newThread->context), (void *) start, 1, arg);

    if (AppendFila2(pfilaAptos, newThread) != 0)
    {
        exit(-1);
    }
    else
    {
        FirstFila2(pfilaAptos);
    }

    return newThread->tid;
}

int cyield(void)
{
    printf("Cyield: ");
    init();

    FirstFila2(pfilaExecutando);

    TCB_t *currentThread = (TCB_t *)GetAtIteratorFila2(pfilaExecutando);

    //DeleteAtIteratorFila2(pfilaExecutando);

    printf("Cyield: Tirando thread %d do estado de executando...\n", currentThread->tid);

    currentThread->state = PROCST_APTO;

    int getContextReturn = getcontext(&(currentThread->context));
    // salva o contexto na thread

    printf("Cyield: Tid da thread que fez a cedencia: %d\n", currentThread->tid);
    if (getContextReturn == -1)
    {
        return -1;
    }

    if (currentThread->state == PROCST_APTO)
    {
        printf("Cyield: Chamando escalonador\n\n\n");
        escalonador();
    }
    else
    {
        printf("Retomando execucao a partir da cyield\n\n\n");
        return 0;
    }
    return 0;
}

int cjoin(int tid)
{
    printf("Cjoin: ");
    init();

    TCB_t *currentThread;
    FirstFila2(pfilaAptos);
    int achou = 0;

    printf("antes\n");
    printf("Cjoin: Procurando thread com tid %d na fila de aptos\n", tid);
    printf("depois\n");
    do
    {
        currentThread = (TCB_t *)GetAtIteratorFila2(pfilaAptos);
        if (currentThread != NULL)
        {
            if (currentThread->tid == tid)
            {
                printf("Cjoin: Encontrou thread com tid %d na fila de aptos/n", currentThread->tid);
                achou = 1;
            }
        }
    } while((NextFila2(pfilaAptos != -NXTFILA_ENDQUEUE)) && !achou);

    if (achou)
    {
        if (currentThread->join != -1)
        {
            printf("Cjoin: Nao pode esperar essa thread pois thread %d ja esta esperando\n", currentThread->join);
            return -2;
        }
        else
        {
            TCB_t *execThread = (TCB_t *)GetAtIteratorFila2(pfilaExecutando);
            currentThread->join = execThread->tid;

            execThread->state = PROCST_BLOQ;

            if (getcontext(&(execThread->context)) == -1)
            {
                exit(-1);
            }

            if (execThread->state == PROCST_BLOQ)
            {
                printf("Cjoin: Thread %d bloqueada, chamando escalonador...\n\n", execThread->tid);
                escalonador();
            }
            else
            {
                printf("Cjoin: Retomando execucao da thread %d a partir da Cjoin\n\n", execThread->tid);
                return 0;
            }
        }
    }
    else
    {
        return -1;
    }
}

int csem_init(csem_t *sem, int count)
{

    init();

    return -1;
}

int cwait(csem_t *sem)
{

    init();

    return -1;
}

int csignal(csem_t *sem)
{

    init();

    return -1;
}

int cidentify (char *name, int size)
{

    init();

    strncpy (name, "Demetrio Boeira - 297693\nGustavo Lorentz - 287681\nPedro Weber - 287678\n", size);
    return 0;
}

