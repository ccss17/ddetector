#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define DEMONITOR_TRACE "dmonitor.trace"
#define MUTEX_CT 100
#define THREAD_CT 10

typedef struct {
    int size;
    char ** strarr;
} splited_set;

char * read_file(char * filename) {
    FILE * f;
    long fsize;
    char * content;
    
    f = fopen(filename, "r");
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    content = malloc(fsize + 1);
    fseek(f, 0, SEEK_SET);
    fread(content, 1, fsize, f);
    fclose(f);
    content[fsize] = '\0';

    return content;
}

splited_set * split_str(char * content, char * delim){
    char ** next;
    char * tmp;
    int BUF_SIZE = 2, i;
    splited_set * sset ;

    sset = (splited_set *) malloc(sizeof(splited_set));
    sset->size = 0;
    sset->strarr = (char**)malloc(sizeof(char*) * BUF_SIZE);

	tmp = strtok(content, delim);
	while (tmp != NULL) {
        sset->strarr[sset->size] = tmp;
        tmp = strtok(NULL, delim);
        sset->size++;
        if (sset->size >= BUF_SIZE){
            BUF_SIZE *= 2;
            next = (char**)malloc(sizeof(char*) * BUF_SIZE);
            for (i=0; i<BUF_SIZE/2; i++)
                next[i] = sset->strarr[i];
            free(sset->strarr);
            sset->strarr = next;
        }
    }

    return sset;
}

int check_deadlock(splited_set * mutexes) {
    int i, j, last, flag;

    flag = 1;
    for (i=1; i<mutexes->size-1; i++) {
    if (mutexes->strarr[i] != NULL) {
        for (j = i+1; j<mutexes->size; j++) {
        if (mutexes->strarr[j] != NULL &&
            strcmp(mutexes->strarr[i], mutexes->strarr[j]) == 0) {
                mutexes->strarr[i] = mutexes->strarr[j] = NULL;
                flag = 0;
                break;
        }
        flag = 1;
        }
    }
    if (flag == 1) return 0;
    }
    return 1;
}

int main(void) {
	char * content;
    splited_set * sections, * mutexes;
    int i;

    content = read_file(DEMONITOR_TRACE);
	sections = split_str(content, "\n");
	mutexes = split_str(sections->strarr[sections->size - 1], ",");
    if (check_deadlock(mutexes) == 1)
        printf("\x1b[31mDEADLOCK DETECTED :( \x1b[0m\n");
    else
        printf("\x1b[32mNO DEADLOCK DETECTED :) \x1b[0m\n");

    free(sections->strarr);
    free(mutexes->strarr);
    free(sections);
    free(mutexes);
    free(content);

    return 0;
}
