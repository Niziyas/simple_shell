#include "shell.h"

/**
 * printEnvironment - Print the current environment.
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int printEnvironment(info_t *info)
{
	print_environment(info->env);
	return (0);
}

/**
 * getEnvValue - Get the value of an environment variable.
 * @info: Structure containing potential arguments.
 * @name: Environment variable name.
 * Return: The value of the environment variable or NULL if not found.
 */
char *getEnvValue(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *value = NULL;

	while (node)
	{
		if (starts_with(node->str, name))
		{
			value = strchr(node->str, '=') + 1;
			break;
		}
		node = node->next;
	}

	return (value);
}

/**
 * setEnvironmentVariable - Initialize
 * a new environment variable or modify an existing one.
 * @info: Structure containing potential arguments.
 * Return: 0 if successful, 1 on error.
 */
int setEnvironmentVariable(info_t *info)
{
	if (info->argc != 3)
	{
		printString("Usage: setEnvironmentVariable VARIABLE VALUE\n");
		return (1);
	}

	if (_setenv(info, info->argv[1], info->argv[2]))
		return (0);

	return (1);
}

/**
 * unsetEnvironmentVariable - Remove an environment variable.
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int unsetEnvironmentVariable(info_t *info)
{
	int i = 1;

	if (info->argc < 2)
	{
		printString("Usage: unsetEnvironmentVariable VARIABLE1 [VARIABLE2 ...]\n");
		return (1);
	}

	while (i < info->argc)
	{
		_unsetenv(info, info->argv[i]);
		i++;
	}

	return (0);
}

/**
 * populateEnvironmentList - Populate the environment linked list.
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int populateEnvironmentList(info_t *info)
{
	list_t *node = NULL;
	size_t i = 0;

	while (environ[i])
	{
		add_node_end(&node, environ[i], 0);
		i++;
	}

	info->env = node;
	return (0);
}
