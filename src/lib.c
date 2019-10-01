
#include <stdio.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"


int ccreate (void* (*start)(void*), void *arg, int prio) {
	return -1;
}

int cyield(void) {
	return -1;
}

int cjoin(int tid) {
	return -1;
}

int csem_init(csem_t *sem, int count) {
	return -1;
}

int cwait(csem_t *sem) {
	return -1;
}

int csignal(csem_t *sem) {
	return -1;
}

int cidentify (char *name, int size) {
	strncpy (name, "Demetrio Boeira - xxxxxx\nGustavo Lorentz - 287681\nPedro Weber - 287678\n", size);
	return 0;
}

int main() {
    char name[74] = "";
    char *namep = &name;
    int size = 74;
    cidentify(namep, size);
    printf("%s", name);
    return 0;
}

