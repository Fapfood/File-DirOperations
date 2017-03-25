#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>



void clearBuffor();

void printHelp() {
    printf("h - help\n");
    printf("r - lock read nonblocking\n");
    printf("R - lock read blocking\n");
    printf("w - lock write nonblocking\n");
    printf("W - lock write blocking\n");
    printf("l - print locked bytes\n");
    printf("u - unlock\n");
    printf("g - get char\n");
    printf("s - set char\n");
    printf("q - quit\n");
}

int getPosition() {
    printf("Position: ");
    int position;
    scanf("%d", &position);
    clearBuffor();
    return position;
}

void setLock_r(int file, int position) {
    struct flock *lock = malloc(sizeof(struct flock));
    lock->l_type = F_RDLCK;
    lock->l_whence = SEEK_SET;
    lock->l_start = position;
    lock->l_len = 1;
    lock->l_pid = getpid();
    if (fcntl(file, F_SETLK, lock) == -1) {
        printf("Cannot set read lock on position %d\n", position);
    }else
        printf("setLock_r");
    free(lock);
}

void setLock_R(int file, int position) {
    struct flock *lock = malloc(sizeof(struct flock));
    lock->l_type = F_RDLCK;
    lock->l_whence = SEEK_SET;
    lock->l_start = position;
    lock->l_len = 1;
    lock->l_pid = getpid();
    fcntl(file, F_SETLKW, lock);
    free(lock);
}

void setLock_w(int file, int position) {
    struct flock *lock = malloc(sizeof(struct flock));
    lock->l_type = F_WRLCK;
    lock->l_whence = SEEK_SET;
    lock->l_start = position;
    lock->l_len = 1;
    lock->l_pid = getpid();
    if (fcntl(file, F_SETLK, lock) == -1) {
        printf("Cannot set write lock on position %d\n", position);
    }
    free(lock);
}

void setLock_W(int file, int position) {
    struct flock *lock = malloc(sizeof(struct flock));
    lock->l_type = F_WRLCK;
    lock->l_whence = SEEK_SET;
    lock->l_start = position;
    lock->l_len = 1;
    lock->l_pid = getpid();
    fcntl(file, F_SETLKW, lock);
    free(lock);
}

void printLockedBytes(int file) {
    if (lseek(file, 0, SEEK_SET) == EOF) {
        return;
    }
    int file_size = lseek(file, 0, SEEK_END);
    struct flock *lock = malloc(sizeof(struct flock));
    lock->l_whence = SEEK_SET;
    lock->l_len = 1;
    for (int position = 0; position < file_size; position++) {
        lock->l_start = position;

        lock->l_type = F_RDLCK;
        fcntl(file, F_GETLK, lock);
        if (lock->l_type == F_RDLCK) {
            printf("Read lock on position %d by %d\n", position, lock->l_pid);
        } else if (lock->l_type == F_WRLCK) {
            printf("Write lock on position %d by %d\n", position, lock->l_pid);
        }
    }
    free(lock);
}

void unlock(int file, int position) {
    struct flock *lock = malloc(sizeof(struct flock));
    lock->l_type = F_UNLCK;
    lock->l_whence = SEEK_SET;
    lock->l_start = position;
    lock->l_len = 1;
    fcntl(file, F_SETLK, lock);
    free(lock);
}

void getChar(int file, int position) {
    char tmp;
    lseek(file, position, SEEK_SET);
    if (read(file, &tmp, 1) < 0) {
        printf("Cannot read byte on position %d\n", position);
    } else {
        printf("Byte on position %d: %s\n", position, &tmp);
    }
}

void setChar(int file, int position) {
    printf("Char: ");
    char byte;
    scanf("%c", &byte);
    clearBuffor();

    lseek(file, position, SEEK_SET);
    if (write(file, &byte, 1) < 1) {
        printf("Cannot write byte on position %d\n", position);
    } else {
        printf("Byte saved\n");
    }
}

void clearBuffor() {
    char c = 0;
    while (c != '\n') {
        c = getchar();
        printf("%c\n",c);
    }
    return;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("No file name specified\n");
        exit(1);
    }
    if (argc > 2) {
        printf("Too many arguments\n");
        exit(2);
    }

    char *filename = argv[1];
    int file = open(filename, O_RDWR);
    if (file == -1) {
        printf("An error with file occurred. Perhaps the file does not exist\n");
        exit(3);
    }

    char command;
    printHelp();
    while (1) {
        printf("Choose command: ");
        scanf("%s", &command);
        clearBuffor();
        switch (command) {
            case 'h':
                printHelp();
                break;
            case 'r':
                setLock_r(file, getPosition());
                break;
            case 'R':
                setLock_R(file, getPosition());
                break;
            case 'w':
                setLock_w(file, getPosition());
                break;
            case 'W':
                setLock_W(file, getPosition());
                break;
            case 'l':
                printLockedBytes(file);
                break;
            case 'u':
                unlock(file, getPosition());
                break;
            case 'g':
                getChar(file, getPosition());
                break;
            case 's':
                setChar(file, getPosition());
                break;
            case 'q':
                close(file);
                exit(0);
            default:
                printf("Command not found\n");
                printHelp();
        }
    }
}