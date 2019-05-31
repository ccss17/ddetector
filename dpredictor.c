#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "dd.h"

typedef struct {
    int size;
    char ** strarr;
} splited_set;

int exists(char * fname) {
    if ( access( fname, F_OK) != -1 ) return 1;
    else return 0;
}

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

int check_deadlock_with_trace(splited_set * mutexes) {
    int i, j, last, flag;

    if (mutexes->size%2 == 1 ||
       (mutexes->size == 2 && 
        strcmp(mutexes->strarr[0], mutexes->strarr[1] == 0)))
        return 0;

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

int main(int argc, char * argv[]) {
    if (argc != 2) {
        fputs("Usage ./dpredictor [dmonitor.trace PATH]\n", stderr);
        return 1;
    }
    if (exists(argv[1]) == 0){
        fprintf(stderr, "%s doesn't exists\n", argv[1]);
        return 1;
    }
	char * content;
    splited_set * sections, * mutexes;
    int i;

    content = read_file(argv[1]);
	sections = split_str(content, "\n");
	mutexes = split_str(sections->strarr[sections->size - 1], ",");
    if (check_deadlock_with_trace(mutexes) == 1)
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
