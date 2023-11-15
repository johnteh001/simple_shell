#include "shell.h"
#include <signal.h>
#include <unistd.h>

/**
 * signal_handler - handles the signal
 * @signal: signal number (unused)
 */
void signal_handler(int signal)
{
	(void)signal;

	write(STDOUT_FILENO, "\n$ ", 3);
}
