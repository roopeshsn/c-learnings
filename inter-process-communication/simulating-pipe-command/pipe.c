#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_file(char[]);
void read_file(char[], char[]);

int main() {
    int fd[2];
    // fd[0] - read, fd[1] - write
    if(pipe(fd) == -1) {
        return 1;
    }

    // Create child process 1 for reading the file
    int child_pid1 = fork();
    if(child_pid1 < 0) {
        return 2;
    }
    if(child_pid1 == 0) {
        printf("From child process 1 ... \n");
        close(fd[0]); // Close unused read end
        char data[10000];
        read_file(data, "interface-status.txt");
        write(fd[1], data, strlen(data) + 1); // +1 for null terminator
        close(fd[1]); // Close write end after writing
    }

    // Create child process 2 for grep
    int child_pid2 = fork();
    if(child_pid2 < 0) {
        return 2;
    }
    if(child_pid2 == 0) {
        printf("From child process 2 ... \n");
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        char data[10000];
        read(fd[0], data, sizeof(data)); // Read data from pipe
        puts(data);
        execlp("grep", "grep", "Ma1", NULL);
        close(fd[0]);
    }

    // Parent process: Close both ends of the pipe and wait for both child processes
    close(fd[0]);
    close(fd[1]);
    waitpid(child_pid1, NULL, 0);
    waitpid(child_pid2, NULL, 0);
    return 0;
}

void print_file(char file_name[]) {
    FILE *fp;
    int c;
    fp = fopen(file_name, "r");

    while ((c = fgetc(fp)) != EOF) {
        putchar(c);
    }
    fclose(fp);
}

void read_file(char data[], char file_name[]) {
    FILE *fp;
    int c;
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    int i = 0;
    while ((c = fgetc(fp)) != EOF && i < 10000) { // if the condition i < 999 is not there then it leads to the error "segmentation fault"
        data[i] = c;
        i++;
    }
    printf("i: %d \n", i);
    data[i] = '\0'; 
    fclose(fp);
}