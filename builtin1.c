#include "shell.h"

/**
 * my_history - displays the history list, one command by line, preceded
 * with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int my_history(info_t *info)
{
	print_history(info->history);
	return (0);
}

/**
 * unset_alias - unset an alias.
 * @info: parameter struct
 * @alias_str: the alias string to unset
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(info_t *info, char *alias_str)
{
	char *equals_position, original_char;
	int ret;

	equals_position = _strchr(alias_str, '=');
	if (!equals_position)
		return (1);

	original_char = *equals_position;
	*equals_position = '\0';

	ret = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, alias_str, -1)));

	*equals_position = original_char;
	return (ret);
}

/**
 * set_alias - set an alias.
 * @info: parameter struct
 * @alias_str: the alias string to set
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *info, char *alias_str)
{
	char *equals_position;

	equals_position = _strchr(alias_str, '=');
	if (!equals_position)
		return (1);

	if (!*++equals_position)
		return (unset_alias(info, alias_str));

	unset_alias(info, alias_str);
	return (add_node_end(&(info->alias), alias_str, 0) == NULL);
}


/**
 * print_alias - prints an alias string.
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *equals_position = NULL, *alias_name = NULL;

	if (node)
	{
		equals_position = _strchr(node->str, '=');
		for (alias_name = node->str; alias_name <= equals_position; alias_name++)
			_putchar(*alias_name);

		_putchar('\'');
		_puts(equals_position + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * my_alias - mimics the alias builtin (man alias).
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int my_alias(info_t *info)
{
	int i = 0;
	char *equals_position = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}

	for (i = 1; info->argv[i]; i++)
	{
		equals_position = _strchr(info->argv[i], '=');
		if (equals_position)
			set_alias(info, info->argv[i]);
		else
			print_alias(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
