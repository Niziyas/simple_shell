#include "shell.h"

/**
 * buffer_chained_commands - Buffers chained commands
 * @info: Parameter struct
 * @buf: Address of the buffer
 * @len: Address of the length variable
 *
 * Return: Bytes read
 */
ssize_t buffer_chained_commands(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*free_and_null((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, block_ctrl_c);
#if USEget_next_input_line
		r = getline(buf, &len_p, stdin);
#else
		r = get_next_input_line(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			info->linecount_flag = 1;
			remove_first_comment(*buf);
			add_to_history(info, *buf, info->histcount++);
			/* if (find_character(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * read_and_parse_input - Reads and parses input, handling chained commands
 * @info: Parameter struct
 *
 * Return: Bytes read
 */
ssize_t read_and_parse_input(info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	write_char_to_stdout(BUF_FLUSH);
	r = buffer_chained_commands(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);

	if (len) /* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		checkChain(info, buf, &j, i, len);

		while (j < len) /* iterate to semicolon or end */
		{
			if (findCharacter(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';' */
		if (i >= len) /* reached the end of the buffer? */
		{
			i = len = 0; /* reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back the pointer to the current command position */
		return (string_length(p)); /* return the length of the current command */
	}
	/* else not a chain, pass back the buffer from get_next_input_line() */
	*buf_p = buf;
	/* return the length of the buffer from get_next_input_line() */
	return (r);
}

/**
 * read_buffer_data - Reads data into a buffer
 * @info: Parameter struct
 * @buf: Buffer
 * @i: Size
 *
 * Return: r
 */
ssize_t read_buffer_data(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);

	r = read(info->readfd, buf, read_buffer_data_SIZE);

	if (r >= 0)
		*i = r;

	return (r);
}

/**
 * get_next_input_line - Gets the next line of input from STDIN
 * @info: Parameter struct
 * @ptr: Address of pointer to buffer, preallocated or NULL
 * @length: Size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int get_next_input_line(info_t *info, char **ptr, size_t *length)
{
	static char buf[read_buffer_data_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buffer_data(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = find_character(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = myRealloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		concatenate_n_characters(new_p, buf + i, k - i);
	else
		copy_n_characters(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * block_ctrl_c - Blocks Ctrl-C signal
 * @sig_num: The signal number
 *
 * Return: void
 */
void block_ctrl_c(__attribute__((unused)) int sig_num)
{
	print_string("\n");
	print_string("$ ");
	write_char_to_stdout(BUF_FLUSH);
}
