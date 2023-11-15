#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

void displayPrompt(void);
void executeCommand(const char *command);

#endif /* SHELL_H */
