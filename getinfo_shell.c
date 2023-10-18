#include "shell.h"

/**
 * initialize_info_struct - Initializes an info_t struct
 * @info: Pointer to the struct to be initialized
 */
void initialize_info_struct(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}


/**
 * initialize_info - Initializes an info_t struct
 * @info: Pointer to the struct to be initialized
 * @av: Argument vector
 */
void initialize_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = split_string(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = string_duplicate(info->arg);
				info->argv[1] = NULL;
			}
		}

		while (info->argv && info->argv[i])
			i++;

		info->argc = i;

		replaceAlias(info);
		replaceAlias(info);
	}
}


/**
 * free_info_struct_fields - Frees fields of an info_t struct
 * @info: Pointer to the info_t struct
 * @all: true if freeing all fields
 */
void free_info_struct_fields(info_t *info, int all)
{
	freeStrings(info->argv);
	info->argv = NULL;
	info->path = NULL;

	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			clear_list(&(info->env));
		if (info->history)
			clear_list(&(info->history));
		if (info->alias)
			clear_list(&(info->alias));
		freeStrings(info->environ);
		info->environ = NULL;
		free_and_null((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		write_char_to_stdout(BUF_FLUSH);
	}
}
