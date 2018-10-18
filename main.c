#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

#define NUMTESTS 12
#define SMPLSIZE 100
#define NUMALGO 2

int main(int argc, char** argv)
{
    char* patd = NULL;
    char* pname = argv[0];
    int FAIL = 0;
    while (--argc > 0) {
        if ((*++argv)[0] == '-')
            switch (*++argv[0]) {
                case 'd':
                    patd = *++argv;
                    --argc;
                    break;
                case 'f':
                    FAIL = 1;
                    break;
                default:
                    argc = 0;
                    break;
            }
    }
    if (argc != 0 || patd == NULL) {
        fprintf(stderr, "Missing argument - match pattern character count'");
        return -1;
    }

    // array of algorithm structs inititialized
    algorithm alg[] = {
        "Naive", naiveFind, {0, 0.0}, 0,
        "Finite-State-Machine", finiteStateFind, {0, 0.0}, 0
    };

    int d = atoi(patd);
    int mult = d ;

    char* str = malloc(PATTERLEN + 1);
    char* pat = malloc(d * NUMTESTS + 2);

    for (int r=0; r < NUMTESTS; r++, d = (r+1) * mult) {

        printf("d: %d\n", d);

        create_test(str, pat, d, FAIL);

        int strL = strlen(str);
        int patL = strlen(pat);

        printf("%s\n", pat);
        // measuring
        for (int i=0; i < NUMALGO; i++) {
            for (int j=0; j < NUMTESTS; j++) {

                /*  when timer() returns data.index, rerun the timer() until we get
                    index -1;
                    in other words, find all instances of the pattern in str.
                */

                alg[i].count = 0;
                int k=0;
                int n = strL;
                char* s = str;
                while (k != -1) {
                    timer_data data = timer(alg[i].func, s, n, pat, patL);
                    alg[i].data.elapsed += data.elapsed;
                    k = alg[i].data.index = data.index;
                    alg[i].count++;
                    s += (k+1); // move str pointer k places
                    n -= (k+1); // reduce str size
                }
            }

            alg[i].count--;
            alg[i].data.elapsed /= NUMTESTS; // average
        }

        // display results
        for (int i=0; i < NUMALGO; i++)
            printf("%.2f\n", alg[i].data.elapsed);
        printf("--------------------------\n");
    }

    free(str);
    free(pat);

    return 0;
}
