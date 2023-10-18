#include "shell.h"

/**
 * printEnvironment - Print the current environment.
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int printEnvironment(info_t *info)
{
	print_environment_list(info->env);
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
	char *p;

	while (node)
	{
		p = find_start(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
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
		printString("Incorrect number of arguements\n");
		return (1);
	}
	if (initialize_or_modify_environment(info, info->argv[1], info->argv[2]))
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
	int i;

	if (info->argc == 1)
	{
		printString("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		remove_environment_variable(info, info->argv[i]);

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
	size_t i;

	for (i = 0; environ[i]; i++)
		prepend_node_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}
