#include "generator.h"

void swapRecords_Sys(int fileToOperate, size_t recordLength, int element1Number, int element2Number);

void swapRecords_Lib(FILE *fileToOperate, size_t recordLength, int element1Number, int element2Number);


void generateRecordsFile(char *fileName, size_t recordLength, int rowsNumber) {

    FILE *fileToSaveIn = fopen(fileName, "wb");
    if (fileToSaveIn == NULL) {
        printf("File not found.\n");
        exit(1);
    }

    FILE *devRandom = fopen("/dev/urandom", "rb");
    if (devRandom == NULL) {
        printf("/dev/urandom not found.\n");
        fclose(fileToSaveIn);
        exit(1);
    }

    char recordTab[recordLength];
    int i;
    for (i = 0; i < rowsNumber; i++) {
        while (1) {
            size_t sizeLoadedElements = fread(recordTab, 1, recordLength, devRandom);
            if (sizeLoadedElements == recordLength) {
                fwrite(recordTab, 1, recordLength, fileToSaveIn);
                //printf("Generated successfully.\n");
                break;
            }
        }
    }
    fclose(devRandom);
    fclose(fileToSaveIn);
}

void copyFile(char *sourceFileName, char *destFileName) {

    FILE *source = fopen(sourceFileName, "rb");
    if (source == NULL) {
        printf("SourceFile not found.\n");
        exit(1);
    }

    FILE *dest = fopen(destFileName, "wb");
    if (dest == NULL) {
        fclose(source);
        printf("DestFile not found.\n");
        exit(1);
    }

    char buffor[2048];
    while (1) {
        size_t sizeReadedElements = fread(buffor, 1, 2048, source);
        if (sizeReadedElements == 0) {
            //printf("End of file %s.", sourceFileName);
            break;
        }
        size_t sizeWritedElements = fwrite(buffor, 1, sizeReadedElements, dest);
        if (sizeWritedElements != sizeReadedElements) {
            printf("Error while writing to file %s.", destFileName);
            exit(1);
        }
    }

    //printf("File copied successfully.\n");

    fclose(source);
    fclose(dest);
}


void shuffleFile_Lib(char *fileName, size_t recordLength, int rowsNumber) {

    FILE *fileToOperate = fopen(fileName, "r+b");
    if (fileToOperate == NULL) {
        printf("File not found\n");
        exit(1);
    }

    int i, j;
    for (i = rowsNumber - 1; i >= 1; i--) {
        j = rand() % i;
        swapRecords_Lib(fileToOperate, recordLength, j, i);
    }
    fclose(fileToOperate);
}

void swapRecords_Lib(FILE *fileToOperate, size_t recordLength, int element1Number, int element2Number) {
    char rec1[recordLength];
    char rec2[recordLength];

    fseek(fileToOperate, (long) (element1Number * recordLength), 0);
    size_t elementsRead1 = fread(rec1, 1, recordLength, fileToOperate);
    fseek(fileToOperate, (long) (element2Number * recordLength), 0);
    size_t elementsRead2 = fread(rec2, 1, recordLength, fileToOperate);

    if ((elementsRead1 != recordLength || elementsRead2 != recordLength) && !feof(fileToOperate)) {
        //jesteśmy w pliku, ale był błąd odczytania
        printf("An error when swap records occured");
        fclose(fileToOperate);
        exit(2);
    }

    fseek(fileToOperate, (long) (element1Number * recordLength), 0);
    fwrite(rec2, 1, recordLength, fileToOperate);
    fseek(fileToOperate, (long) (element2Number * recordLength), 0);
    fwrite(rec1, 1, recordLength, fileToOperate);
}

void sortFile_Lib(char *fileName, size_t recordLength) {

    FILE *fileToOperate = fopen(fileName, "r+b");
    if (fileToOperate == NULL) {
        printf("File not found\n");
        exit(1);
    }

    char rec1[recordLength];
    char rec2[recordLength];

    int isSorted = 0;
    while (!isSorted) {
        isSorted = 1;
        int elementNumber = 0;
        fseek(fileToOperate, 0, 0);
        // feof zwraca zero gdy nie doszliśmy do końca pliku
        while (!feof(fileToOperate)) { //dopóki jesteśmy w pliku

            fseek(fileToOperate, (long) (elementNumber * recordLength), 0);
            size_t elementsRead1 = fread(rec1, 1, recordLength, fileToOperate);
            size_t elementsRead2 = fread(rec2, 1, recordLength, fileToOperate);

            //źle odczytano
            if (elementsRead1 != recordLength || elementsRead2 != recordLength) {
                if (!feof(fileToOperate)) { //jesteśmy w pliku
                    printf("Error while reading file %s.", fileName);
                    fclose(fileToOperate);
                    exit(2);
                } else {//wyszliśmy poza plik
                    continue;//ponawiamy sprawdzenie warunku i wychodzimy z wewnętrzej pętli
                }
            }
            //dobrze odczytano
            if (rec1[0] > rec2[0]) {
                fseek(fileToOperate, (long) (elementNumber * recordLength), 0);
                fwrite(rec2, 1, recordLength, fileToOperate);
                fwrite(rec1, 1, recordLength, fileToOperate);
                isSorted = 0;
            }
            elementNumber++;
        }
    }
    fclose(fileToOperate);
}


void shuffleFile_Sys(char *fileName, size_t recordLength, int rowsNumber) {

    int fileToOperate = open(fileName, 2);
    if (fileToOperate == -1) {
        printf("File not found\n");
        exit(1);
    }

    int i, j;
    for (i = rowsNumber - 1; i >= 1; i--) {
        j = rand() % i;
        swapRecords_Sys(fileToOperate, recordLength, j, i);
    }
    close(fileToOperate);
}

void swapRecords_Sys(int fileToOperate, size_t recordLength, int element1Number, int element2Number) {
    char rec1[recordLength];
    char rec2[recordLength];

    lseek(fileToOperate, element1Number * recordLength, 0);
    size_t elementsRead1 = read(fileToOperate, rec1, recordLength);
    lseek(fileToOperate, element2Number * recordLength, 0);
    size_t elementsRead2 = read(fileToOperate, rec2, recordLength);

    if (elementsRead1 == -1 || elementsRead2 == -1) {
        //jesteśmy w pliku, ale był błąd odczytania
        printf("An error when swap records occured");
        close(fileToOperate);
        exit(2);
    }

    lseek(fileToOperate, element1Number * recordLength, 0);
    write(fileToOperate, rec2, recordLength);
    lseek(fileToOperate, element2Number * recordLength, 0);
    write(fileToOperate, rec1, recordLength);
}

void sortFile_Sys(char *fileName, size_t recordLength) {

    int fileToOperate = open(fileName, 2);
    if (fileToOperate == -1) {
        printf("File not found\n");
        exit(1);
    }

    char rec1[recordLength];
    char rec2[recordLength];

    int isSorted = 0;
    while (!isSorted) {
        isSorted = 1;
        int elementNumber = 0;
        ssize_t elementsRead1 = 1;
        ssize_t elementsRead2 = 1;
        lseek(fileToOperate, 0, 0);

        while (elementsRead1 != 0 && elementsRead2 != 0) {
            lseek(fileToOperate, elementNumber * recordLength, 0);
            elementsRead1 = read(fileToOperate, rec1, recordLength);
            elementsRead2 = read(fileToOperate, rec2, recordLength);

            if (elementsRead1 == -1 || elementsRead2 == -1) {
                printf("Error while reading file %s.", fileName);
                close(fileToOperate);
                exit(2);
            }
            if (elementsRead1 != 0 && elementsRead2 != 0 && rec1[0] > rec2[0]) {
                lseek(fileToOperate, elementNumber * recordLength, 0);
                write(fileToOperate, rec2, recordLength);
                write(fileToOperate, rec1, recordLength);
                isSorted = 0;
            }
            elementNumber++;
        }
    }
    close(fileToOperate);
}