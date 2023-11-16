#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include "shell.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARGUMENTS 100

extern char **environ;

ssize_t custom_getline(char **line, size_t *len, FILE *stream);
void executeCommand(char *args[]);
char *custom_strcat(const char *str1, const char *str2);

/**
 * @brief Custom implementation of getline function.
 *
 * @param line A pointer to the line buffer.
 * @param len A pointer to the buffer size.
 * @param stream The input stream.
 * @return The number of bytes read.
 */
ssize_t custom_getline(char **line, size_t *len, FILE *stream)
{
	size_t i = 0;
	int c;
	if (*line == NULL) {
        *len = MAX_INPUT_SIZE;
        if ((*line = (char *)malloc(*len)) == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    }
    while ((c = getc(stream)) != EOF && c != '\n') {
        if (i == *len - 1) {
            *len *= 2;
            if ((*line = (char *)realloc(*line, *len)) == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        (*line)[i++] = c;
    }

    if (c == EOF && i == 0) {
        return -1;
    }

    (*line)[i] = '\0';

    return i;
}

/**
 * @brief Executes a command using fork and exec.
 * @param command The command to be executed.
 */
void executeCommand(char *args[])
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(NULL);
	}
}
char *custom_strcat(const char *str1, const char *str2)
{
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);
	char *result = (char *)malloc(len1 + len2 + 1);

	if (result == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	strcpy(result, str1);
	strcat(result, str2);
	return (result);
}

/**
 * @brief Main function for the simple shell.
 */
int main(void)
{
	char *input = NULL;
	char **env = environ;
	const char *str1 = "simple, ";
	const char *str2 = "shell";
	char *invalid = custom_strcat(str1, str2);
	size_t argCount = 0;
	char *token = strtok(input, " \n\t\r\a");
	char *args[MAX_ARGUMENTS];

	while (1)
	{
		printf("$ ");
		if (custom_getline(&input, (size_t *)MAX_INPUT_SIZE, stdin) == -1)
		{
			printf("\nGoodbye.\n");
			break;
		}
		while (token != NULL)
		{
			args[argCount++] = token;
			if (argCount >= MAX_ARGUMENTS)
			{
				fprintf(stderr, "Too many arguments. Maximum allowed: %lu\n", (unsigned long)(MAX_ARGUMENTS - 1));
				argCount = 0;
				break;
			}
			token = strtok(NULL, " ");
		}
		args[argCount] = NULL;
		if (argCount > 0)
		{
			if (strcmp(args[0], "exit") == 0)
			{
				break;
			}
			else if (strcmp(args[0], "env") == 0)
			{
				while (*env)
				{
					printf("%s\n", *env);
					env++;
				}
			}
			else
			{
				executeCommand(args);
			}
		}
		argCount = 0;
		printf("invalid input: %s\n", invalid);
	}
	free(invalid);
	free(input);
	return (0);
}
