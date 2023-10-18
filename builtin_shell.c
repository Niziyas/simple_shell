#include "shell.h"

/**
 * my_exit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int my_exit(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = string_to_int(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_error_message(info, "Illegal number: ");
			printString(info->argv[1]);
			printCharToStderr('\n');
			return (1);
		}
		info->err_num = str_to_int(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * my_cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: 0 on success, -1 on failure
 */
int my_cd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		printString("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = getEnvValue(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = getEnvValue(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (string_compare(info->argv[1], "-") == 0)
	{
		if (!getEnvValue(info, "OLDPWD="))
		{
			printString(s);
			printCharToStderr('\n');
			return (1);
		}
		printString(getEnvValue(info, "OLDPWD=")), printCharToStderr('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = getEnvValue(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_error_message(info, "can't cd to ");
		printString(info->argv[1]), printCharToStderr('\n');
	}
	else
	{
		initialize_or_modify_environment(info, "OLDPWD", getEnvValue(info, "PWD="));
		initialize_or_modify_environment(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * my_help - displays help for the shell
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: 0
 */
int my_help(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	print_string("help call works. Function not yet implemented \n");
	if (0)
		print_string(*arg_array); /* temp att_unused workaround */
	return (0);
}
