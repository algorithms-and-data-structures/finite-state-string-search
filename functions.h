#pragma once

#include <time.h>
#include <math.h>

#define PRIME 101
#define ASCII 256
#define PATTERLEN 1000000

typedef struct timer_data {
    int index;
    double elapsed;
} timer_data;

// struct for each algorithm data
typedef struct algorithm {
    char* name;
    int (*func) (char*, int, char*, int); // algorithm function pointer
    timer_data data;
    int count;
} algorithm;

// NAIVE FIND
int naiveFind(char* str, int strL, char* pat, int patL)
{
    for (int i=0; i < strL; i++) {
        int j=i;
        int k=0;
        for (j=i, k=0; str[j] == pat[k] && k < patL ; j++, k++)
            ;
        if (k == patL)
            return i;
    }
    return -1;
}

int finiteStateFind(char* str, const int strL, char* pat, const int patL)
{
    int table[ASCII];
    memset(table, -1, sizeof(table));

    char distinct[patL];

    int d=0;
    for (int i=0; i < patL; i++)
        if (table[pat[i]] == -1) {
            distinct[d] = pat[i];
            table[pat[i]] = d++;
        }

    // table maps character in pat to an index
    // d is how many distict character in pat

    int states[patL][d];
    memset(states, 0, sizeof(states));

    for (int i=0; i < patL; i++) {
        states[i][0] = i;
        for (int j=0; j < d; j++) {
            if (distinct[j] == pat[i])
                states[i][j] = i+1;
        }
    }

    // finally - the state machine runnin g
    int state = 0;
    int i;
    for (i=0; state != patL && i < strL; i++)
            state = states[state][table[str[i]]];

    if (state == patL)
        return i - patL;

    return -1;
}

timer_data timer(int (*fn)(char*,int,char*,int), char* str, int strL, char* match, int patL)
{
    clock_t t;
    t = clock();
    int index = fn(str, strL, match, patL);
    t = clock() - t;
    double elapsed = ((double)t) * 1000 / CLOCKS_PER_SEC;
    timer_data data = {index, elapsed};
    return data;
}

void create_test(char* str, char* pat, int d, int FAIL)
{
    int i, j;
    for(i=0; i < PATTERLEN; i++) {
        str[i] = '0';
        if (i % d == 0)
            str[i] = '1';
    }
    str[i] = '\0';

    // if fail flag is passed, create d number of zeros, not d - 1
    if (FAIL)
        ++d;

    for(j=0; j < d-1; j++)
        pat[j] = '0';
    pat[j] = '1';
    pat[j+1] = '\0';
}
