#include "shell.h"

/**
 * _myexit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int my_exit(info_t *info)
{
	if (info->argv[1])
	{
		int exit_status = string_to_int(info->argv[1]);

		if (exit_status >= 0)
			exit(exit_status);
		else
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			printString(info->argv[1]);
			printCharToStderr('\n');
		}
	}
	else
		exit(0);

	return (0);
}

/**
 * _mycd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: 0 on success, -1 on failure
 */
int my_cd(info_t *info)
{
	char *new_dir = info->argv[1];
	char *current_dir = getcwd(NULL, 0);

	if (!new_dir || _strcmp(new_dir, "~") == 0)
		new_dir = getEnvValue(info, "HOME=");

	if (!new_dir)
		new_dir = "/";

	if (_strcmp(new_dir, "-") == 0)
		new_dir = getEnvValue(info, "OLDPWD=");

	if (chdir(new_dir) == -1)
	{
		print_error(info, "can't cd to ");
		printString(new_dir);
		printCharToStderr('\n');
		free(current_dir);
		return (-1);
	}

	_setenv(info, "OLDPWD", current_dir);
	_setenv(info, "PWD", getcwd(NULL, 0));
	free(current_dir);

	return (0);
}

/**
 * _myhelp - displays help for the shell
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: 0
 */
int my_help(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
