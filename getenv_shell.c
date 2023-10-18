#include "shell.h"

/**
 * copy_environment - Returns a copy of the environment strings
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *
 * Return: A copy of the environment strings.
 */
char **copy_environment(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = convert_list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * remove_environment_variable - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 *
 * Return: 1 on delete, 0 otherwise
 */
int remove_environment_variable(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	for (i = 0; node; i++)
	{
		p = find_start(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = remove_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
		}
		else
		{
			node = node->next;
		}
	}

	return (info->env_changed);
}


/**
 * initialize_or_modify_environment - Initialize a new environment variable
 *                                            or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *
 * Return: 0 on success, 1 on failure
 */
int initialize_or_modify_environment(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(string_length(var) + string_length(value) + 2);
	if (!buf)
		return (1);

	string_copy(buf, var);
	string_concat(buf, "=");
	string_concat(buf, value);

	for (node = info->env; node; node = node->next)
	{
		p = find_start(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			free(buf);
			return (0);
		}
	}

	prepend_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
