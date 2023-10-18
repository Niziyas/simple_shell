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
	info_t info = INFO_INIT;

	if (ac == 2)
	{
		info.readfd = open(av[1], O_RDONLY);

		if (info.readfd == -1)
		{
			if (errno == EACCES)
				return (126);

			if (errno == ENOENT)
			{
				printString(av[0]);
				printString("0 : Can't open");
				printString(av[1]);
				printString("\n");
				return (127);
			}
			return (EXIT_FAILURE);
		}
	}

	populateEnvironmentList(&info);
	read_history(&info);
	hsh(&info, av);
	return (EXIT_SUCCESS);
}
