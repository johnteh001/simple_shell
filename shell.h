#ifndef SHELL_H
#define SHELL_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

/* global variables */
extern char **environ;

void signal_handler(int signal);
char *our_strcpy(char *dest, const char *src);
char *strdup(const char *str);
size_t our_strlen(const char *str);
int our_strcmp(const char *string1, const char *string2);
size_t strlen(const char *str);
ssize_t custom_getline(char **line, size_t *len, FILE *stream);
void executeCommand(char *args[]);
void executeCommand(char *args[]);
char *custom_strcat(const char *str1, const char *str2);
int main();

#endif
