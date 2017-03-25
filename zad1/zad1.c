#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include "generator.h"

struct rusage startRusage;
struct rusage endRusage;
struct timespec startReal;
struct timespec endReal;


void resetTimer() {
    getrusage(RUSAGE_SELF, &startRusage);
    clock_gettime(CLOCK_MONOTONIC_RAW, &startReal);
}

void measureTime(char *description) {
    getrusage(RUSAGE_SELF, &endRusage);
    clock_gettime(CLOCK_MONOTONIC_RAW, &endReal);
    printf("\tUSER:\t %10ldus\t", (long) ((endRusage.ru_utime.tv_sec - startRusage.ru_utime.tv_sec) * 1000000 +
                                          (endRusage.ru_utime.tv_usec - startRusage.ru_utime.tv_usec)));
    printf("\tSYSTEM:\t %10ldus\t", (long) ((endRusage.ru_stime.tv_sec - startRusage.ru_stime.tv_sec) * 1000000 +
                                            (endRusage.ru_stime.tv_usec - startRusage.ru_stime.tv_usec)));
    printf("\tREAL:\t %10ldus  ",
           (long) ((endReal.tv_sec - startReal.tv_sec) * 1000000000 + (endReal.tv_nsec - startReal.tv_nsec)) / 1000);
    printf("|--- %s\n", description);
}

void sysOperations(char *fileName, size_t recordLength, int rowsNumber) {
    resetTimer();
    sortFile_Sys(fileName, recordLength);
    measureTime("first sort");

    resetTimer();
    shuffleFile_Sys(fileName, recordLength, rowsNumber);
    measureTime("shuffle");

    resetTimer();
    sortFile_Sys(fileName, recordLength);
    measureTime("second sort");
}

void libOperations(char *fileName, size_t recordLength, int rowsNumber) {
    resetTimer();
    sortFile_Lib(fileName, recordLength);
    measureTime("first sort");

    resetTimer();
    shuffleFile_Lib(fileName, recordLength, rowsNumber);
    measureTime("shuffle");

    resetTimer();
    sortFile_Lib(fileName, recordLength);
    measureTime("second sort");
}

void test(size_t recordLength, int rowsNumber) {
    printf("---Record length: %d, number of rows: %d---\n", (int) recordLength, rowsNumber);
    generateRecordsFile("./test1.txt", recordLength, rowsNumber);
    copyFile("./test1.txt", "./test2.txt");
    printf("---------With system functions:----------\n");
    sysOperations("./test1.txt", recordLength, rowsNumber);
    printf("---------With library functions:---------\n");
    libOperations("./test2.txt", recordLength, rowsNumber);
    printf("-----------------------------------------\n");
}

int main() {
    srand(time(NULL));
    test(4, 500);
    test(4, 1000);
    test(512, 200);
    test(512, 400);
    test(4096, 100);
    test(4096, 200);
    test(8192, 50);
    test(8192, 100);
    return 0;
}
