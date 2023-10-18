#include "shell.h"

/**
 * allocate_history_file - Allocates the history file path
 * @info: Parameter struct
 *
 * Return: Allocated string containing the history file path
 */
char *allocate_history_file(info_t *info)
{
	char *buf, *dir;

	dir = getEnvValue(info, "HOME=");
	if (!dir)
		return (NULL);

	buf = malloc(sizeof(char) * (string_length(dir) +
		 string_length(HIST_FILE) + 2));
	if (!buf)
		return (NULL);

	buf[0] = 0;
	string_copy(buf, dir);
	string_concat(buf, "/");
	string_concat(buf, HIST_FILE);

	return (buf);
}

/**
 * save_history - Saves the history to a file, creating it if necessary
 * @info: The parameter struct
 *
 * Return: 1 on success, else -1
 */
int save_history(info_t *info)
{
	ssize_t fd;
	char *filename = allocate_history_file(info);
	list_t *node = info->history;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);

	if (fd == -1)
		return (-1);

	while (node)
	{
		writeStringToFD(node->str, fd);
		writeCharToFD('\n', fd);
		node = node->next;
	}

	writeCharToFD(BUF_FLUSH, fd);
	close(fd);

	return (1);
}

/**
 * load_history - Loads history from a file
 * @info: The parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int load_history(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = allocate_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			add_to_history(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		add_to_history(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		remove_node_at_index(&(info->history), 0);
	update_history_numbers(info);
	return (info->histcount);
}

/**
 * add_to_history - Adds an entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: Buffer
 * @linecount: The history line count, histcount
 *
 * Return: Always 0
 */
int add_to_history(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	prepend_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;

	return (0);
}

/**
 * update_history_numbers - Updates the
 * history linked list numbers after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: The new histcount
 */
int update_history_numbers(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	for (i = 0; node; i++, node = node->next)
	{
		node->num = i;
	}

	info->histcount = i;
	return (i);
}
