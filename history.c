#include "shell.h"
#define HISTORY_FILE ".shell_history"
/**
 * get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containing history file
 */
char *get_history_file(info_t *info)
{
	char *home_dir, *history_file;

	home_dir = _getenv(info, "HOME=");
	if (!home_dir)
		return (NULL);
	history_file = malloc(sizeof(char) * (_strlen(home_dir) + _strlen(HISTORY_FILE) + 2));
	if (!history_file)
		return (NULL);
	history_file[0] = '\0';
	_strcpy(history_file, home_dir);
	_strcat(history_file, "/");
	_strcat(history_file, HISTORY_FILE);
	free(home_dir);
	return (history_file);
}

/**
 * write_history - writes the history to a file
 * @info: the parameter struct
 *
 * Return: 1 on success, -1 on failure
 */
int write_history(info_t *info)
{
	int fd;
	char *filename = get_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	if (close(fd) == -1)
		return (-1);
	return (1);
}

/**
 * read_history - reads the history from a file
 * @info: the parameter struct
 *
 * Return: number of history entries on success, 0 on failure
 */
int read_history(info_t *info)
{
	int fd, linecount = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (fstat(fd, &st) == -1)
		return (close(fd), 0);
	if (st.st_size < 2)
		return (close(fd), 0);
	buf = malloc(sizeof(char) * (st.st_size + 1));
	if (!buf)
		return (close(fd), 0);
	if (read(fd, buf, st.st_size) == -1)
		return (free(buf), close(fd), 0);
	buf[st.st_size] = '\0';
	if (close(fd) == -1)
		return (free(buf), 0);
	linecount = parse_history_buffer(info, buf);
	free(buf);
	return (linecount);
}

/**
 * parse_history_buffer - parses the history buffer and builds the history list
 * @info: the parameter struct
 * @buffer: the history buffer
 *
 * Return: number of history entries
 */
int parse_history_buffer(info_t *info, char *buffer)
{
	int linecount = 0;
	char *line = NULL;

	line = _strtok(buffer, "\n");
	while (line)
	{
		build_history_list(info, line, linecount++);
		line = _strtok(NULL, "\n");
	}
	return (linecount);
}

/**
 * build_history_list - adds an entry to the history linked list
 * @info: the parameter struct
 * @str: the history entry string
 * @linecount: the history line count
 *
 * Return: Always 0
 */
int build_history_list(info_t *info, char *str, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	add_node_end(&node, str, linecount);
	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: the parameter struct
 *
 * Return: the new histcount
 */
int renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	info->histcount = i;
	return (i);
}
