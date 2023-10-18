#include "shell.h"

/**
 * isExecutableCommand - determines if a file is an executable command
 * @info: the info struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int isExecutableCommand(info_t *info, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * duplicateChars - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *duplicateChars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = start, k = 0;

	while (k < 1024 && i < stop)
	{
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
		i++;
	}

	buf[k] = '\0';
	return (buf);
}

/**
 * findPath - finds this cmd in the PATH string
 * @info: the info struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *findPath(info_t *info, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((string_length(cmd) > 2) && find_start(cmd, "./"))
	{
		if (isExecutableCommand(info, cmd))
			return (cmd);
	}
	for (i = 0; pathstr[i]; i++)
	{
		if (pathstr[i] == ':')
		{
			path = duplicateChars(pathstr, curr_pos, i);
			if (!*path)
				string_concat(path, cmd);
			else
			{
				string_concat(path, "/");
				string_concat(path, cmd);
			}
			if (isExecutableCommand(info, path))
				return (path);
			curr_pos = i + 1;
		}
	}
	path = duplicateChars(pathstr, curr_pos, i);
	if (!*path)
		string_concat(path, cmd);
	else
	{
		string_concat(path, "/");
		string_concat(path, cmd);
	}
	if (isExecutableCommand(info, path))
		return (path);
	return (NULL);
}
