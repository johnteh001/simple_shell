#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

#define MAX_ARGUMENTS 100
/**
 * executeCommand - executes a command with fork and execvp
 * @args: array of command-line arguments
 */
void executeCommand(char *args[]);
/**
 * @brief Custom implementation of strlen function.
 * @param str The input string.
 * @return The length of the string.
 */
char *custom_strcat(const char *str1, const char *str2);

/**
 * @brief Custom implementation of strlen function.
 * @param str The input string.
 * @return The length of the string.
 */
size_t custom_strlen(const char *str);

/**
 * @brief Custom implementation of getline function.
 * @param line A pointer to the line buffer.
 * @param len A pointer to the buffer size.
 * @param stream The input stream.
 * @return The number of bytes read.
 */
ssize_t custom_getline(char **line, size_t *len, FILE *stream);

/**
 * @brief Execute a command with fork and execvp.
 * @param args An array of command-line arguments.
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
		free(args);
	}
}
/**
 * @brief Custom implementation of getline function.
 * @param line A pointer to the line buffer.
 * @param len A pointer to the buffer size.
 * @param stream The input stream.
 * @return The number of bytes read.
 */
ssize_t custom_getline(char **line, size_t *len, FILE *stream)
{
	char *buffer = NULL;
	size_t bufsize = 0;
	ssize_t bytesRead;

	if (*line == NULL || *len == 0)
	{
		bufsize = 1024;
		buffer = (char *)malloc(bufsize);
		if (buffer == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		*line = buffer;
		*len = bufsize;
	}
	else
	{
		buffer = *line;
		bufsize = *len;
	}
	bytesRead = getline(&buffer, &bufsize, stream);
	if (bytesRead != -1)
	{
		if (bytesRead > 0 && buffer[bytesRead - 1] == '\n')
		{
			buffer[bytesRead - 1] = '\0';
			bytesRead--;
		}
	}
	else
	{
		free(buffer);
		*line = NULL;
		*len = 0;
	}
	return (bytesRead);
}
/**
 * @brief Custom implementation of strcat function.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return The concatenated string.
 */
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
 * @brief Main function for the simple shell program.
 * The function takes user input, processes commands,
 * and executes them.
 * @return 0 on successful execution.
 */
int main()
{
	char *input = NULL;
	size_t MAX_INPUT_SIZE = 1024;
	size_t argCount = 0;
	char *token = NULL;
	char **env = environ;
	const char *str1 = "simple, ";
	const char *str2 = "shell";
	char *invalid = custom_strcat(str1, str2);
	char *args[MAX_ARGUMENTS];

	while (1)
	{
		printf("$ ");
		if (custom_getline(&input, &MAX_INPUT_SIZE, stdin) == -1)
		{
			break;
		}
		token = strtok(input, " \n\t\r\a");
		while (token != NULL)
		{
			args[argCount++] = token;
			token = strtok(NULL, " \n\t\r\a");
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
		printf("invalid input: %s\n", invalid);
	}
	free(invalid);
	printf("Goodbye.\n");
	return (0);
}
