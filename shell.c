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

/**
 * @brief Displays the shell prompt.
 */
void displayPrompt(void)
{
	printf("#cisfun$ ");
	fflush(stdout);
}
/**
 * @brief Executes a command using fork and exec.
 * @param command The command to be executed.
 */
void executeCommand(const char *command)
{
	pid_t pid = fork();
	int status;
	execlp(command, command, (char *)NULL);
	waitpid(pid, &status, 0);

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		fprintf(stderr, "%s: command not found\n", command);
		exit(EXIT_FAILURE);
	}
	else
	{
	if (WIFEXITED(status))
	{
	if (WEXITSTATUS(status) != EXIT_SUCCESS)
	{
		fprintf(stderr, "%s: command failed with exit status %d\n", command, WEXITSTATUS(status));
	}
	}
	else if (WIFSIGNALED(status))
	{
		fprintf(stderr, "%s: command terminated by signal %d\n", command, WTERMSIG(status));
	}
	}
}/**
 * @brief Main function for the simple shell.
 */
int main(void)
{
	char command[MAX_COMMAND_LENGTH];

	while (1)
	{
		displayPrompt();
		if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
		{
			printf("\nGoodbye.\n");
			break;
		}
		command[strcspn(command, "\n")] = '\0';
		executeCommand(command);
	}
	return (0);
}

