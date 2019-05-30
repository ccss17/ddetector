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

char** str_split(char* a_str, const char a_delim) {
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
    *        knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result) {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


int check_deadlock(char * section){
    int i, j, last, flag;

    flag = 1;
    for (i=0; i<MUTEX_CT * THREAD_CT; i++) {
        if (section[i] != NULL) {
            for (j = i+1; j<MUTEX_CT * THREAD_CT; j++) {
                if (strcmp(section[i], section[j]) == 0) {
                    section[i] = section[j] = NULL;
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
    FILE * f;
    size_t sz;
    char ** result;
    char * content = "awefawefaw\n"
        "awefawefawef\n"
        "\n"
        "bbbbbbbbbbbbbbbbb\n"
        "zzzzzzzzzzzzzzzzz\n"
        "\n";
    char * test = "123123:99999";

    f = fopen(DEMONITOR_TRACE, "r");
    fseek(f, 0L, SEEK_END);
    sz = ftell(f);
    /*printf("size: %ld\n", sz);*/
    /*result = str_split(content, "\n");*/
    result = str_split(test, ':');
    puts("#");
    printf("%s\n", *(result));
    puts("#");
    /*printf("%s\n", *(result+1));*/
    puts("#");
    
    /*last_section = str_split(last , "\n");*/

    fclose(f);

    return 0;
}
