#include "shell.h"

/**
 * main - Entry point
 * @ac: Argument count
 * @av: Argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	info_t info[] = { INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);

		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);

			if (errno == ENOENT)
			{
				printString(av[0]);
				printString(": 0: Can't open");
				printString(av[1]);
				printString("\n");
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readfd = fd;
	}

	populateEnvironmentList(info);
	load_history(info);
	hsh(info, av);
	return (EXIT_SUCCESS);
}
