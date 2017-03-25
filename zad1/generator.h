#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifndef LAB2_GENERATOR_H
#define LAB2_GENERATOR_H

void generateRecordsFile(char *fileName, size_t recordLength, int rowsNumber);

void copyFile(char *sourceFileName, char *destFileName);

void shuffleFile_Lib(char *fileName, size_t recordLength, int rowsNumber);

void sortFile_Lib(char *fileName, size_t recordLength);

void shuffleFile_Sys(char *fileName, size_t recordLength, int rowsNumber);

void sortFile_Sys(char *fileName, size_t recordLength);

#endif //LAB2_GENERATOR_H
