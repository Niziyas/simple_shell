#include "shell.h"

/**
 * is_interactive - Checks if the shell is in interactive mode
 * @info: A pointer to the info_t structure
 *
 * Return: 1 if in interactive mode, 0 otherwise
 */
int is_interactive(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_delimiter - Checks if a character is a delimiter
 * @c: The character to check
 * @delimiters: The delimiter string
 *
 * Return: 1 if c is a delimiter, 0 otherwise
 */
int is_delimiter(char c, char *delimiters)
{
	while (*delimiters)
	{
		if (*delimiters == c)
			return (1);
		delimiters++;
	}
	return (0);
}

/**
 * is_alpha - Checks if a character is an alphabetic character
 * @c: The character to check
 *
 * Return: 1 if c is an alphabetic character, 0 otherwise
 */
int is_alpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/**
 * string_to_int - Converts a string to an integer
 * @s: The string to be converted
 *
 * Return: The converted integer or 0 if no numbers in the string
 */
int string_to_int(char *s)
{
	int i = 0, sign = 1, flag = 0, output = 0;
	unsigned int result = 0;

	while (s[i] != '\0' && flag != 2)
	{
		if (s[i] == '-')
		{
			sign *= -1;
		}

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result = result * 10 + (s[i] - '0');
		}
		else if (flag == 1)
		{
			flag = 2;
		}

		i++;
	}

	output = (sign == -1) ? -result : result;

	return (output);
}
