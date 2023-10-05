#include "shell.h"

/**
 * clear_info - initializes info_t struct
 * @info: struct address
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void set_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replace_alias(info);
		replace_vars(info);
	}
}

/**
 * free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_info(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			free_list(&(info->env));
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		ffree(info->environ);
		info->environ = NULL;
		bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}

/**
 * replace_alias - Replaces aliases in command with their respective values
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 */
void replace_alias(info_t *info)
{
	list_t *alias = info->alias;
	char *cmd = info->arg, *key = NULL, *value = NULL;
	size_t key_len = 0, cmd_len = 0, value_len = 0;

	while (alias)
	{
		key = alias->str;
		key_len = _strlen(key);
		value = _strchr(key, '=');
		if (value)
		{
			value++;
			value_len = _strlen(value);
			if (_strncmp(cmd, key, key_len) == 0 && (cmd[key_len] == ' ' || cmd[key_len] == '\t' || cmd[key_len] == '\0'))
			{
				cmd_len = _strlen(cmd);
				info->arg = malloc(cmd_len - key_len + value_len + 1);
				if (!info->arg)
					return;
				_strncpy(info->arg, cmd, key_len);
				_strncpy(info->arg + key_len, value, value_len);
				_strcpy(info->arg + key_len + value_len, cmd + key_len);
				free(cmd);
				cmd = info->arg;
			}
		}
		alias = alias->next;
	}
}

/**
 * replace_vars - Replaces variables in command with their respective values
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 */
void replace_vars(info_t *info)
{
	char *cmd = info->arg, *key = NULL, *value = NULL, *var = NULL, *temp = NULL;
	size_t key_len = 0, cmd_len = 0, value_len = 0;

	while ((var = _strchr(cmd, '$')))
	{
		if (var[1] == '?')
		{
			cmd_len = _strlen(cmd);
			info->arg = malloc(cmd_len - 1 + 4);
			if (!info->arg)
				return;
			_strncpy(info->arg, cmd, var - cmd);
			_strncpy(info->arg + (var - cmd), "0", 1);
			_strncpy(info->arg + (var - cmd) + 1, var + 2, cmd_len - (var - cmd) - 1);
			free(cmd);
			cmd = info->arg;
			continue;
		}
		key = var + 1;
		key_len = strcspn(key, " \t\n$");
		value = _getenv(info, key);
		if (value)
		{
			value_len = _strlen(value);
			cmd_len = _strlen(cmd);
			info->arg = malloc(cmd_len - key_len + value_len + 1);
			if (!info->arg)
				return;
			_strncpy(info->arg, cmd, var - cmd);
			_strncpy(info->arg + (var - cmd), value, value_len);
			_strncpy(info->arg + (var - cmd) + value_len, var + key_len + 1, cmd_len - (var - cmd) - key_len);
			free(cmd);
			cmd = info->arg;
		}
		var++;
	}
	info->arg = cmd;
}
