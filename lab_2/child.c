#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "%d is invalid arguments amount, must be 2\n", argc);
        exit(1);
    }

    puts("Child process data:");
    printf("Name:       %s\n", argv[0]);
    printf("PID:        %d\n", getpid());
    printf("Parent PID: %d\n", getppid());

    char buff[256];
    FILE* fenvp = fopen(argv[1], "r");
    if (!fenvp) {
        perror("Error open enviroment file...");
        exit(1);
    }
    while (fgets(buff, 256, fenvp) != NULL) {
        buff[strcspn(buff, "\n")] = '\0';
        printf("%s = %s\n", buff, getenv(buff));
    }

    fclose(fenvp);
    exit(0);
}