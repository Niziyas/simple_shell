#include "shell.h"

/**
 **copy_n_characters - copies a string
 *@dest: the destination string to be copied to
 *@src: the source string
 *@n: the amount of characters to be copied
 *Return: the concatenated string
 */
char *copy_n_characters(char *dest, char *src, int n)
{
	int i;
	char *s = dest;

	for (i = 0; src[i] != '\0' && i < n - 1; i++)
	{
		dest[i] = src[i];
	}

	for (int j = i; j < n; j++)
	{
		dest[j] = '\0';
	}

	return (s);
}

/**
 **concatenate_n_characters - concatenates two strings
 *@dest: the first string
 *@src: the second string
 *@n: the amount of bytes to be maximally used
 *Return: the concatenated string
 */
char *concatenate_n_characters(char *dest, char *src, int n)
{
	char *s = dest;
	int i, j = 0;

	for (i = 0; dest[i] != '\0'; i++)
	{
		/* Find the end of the destination string */
	}

	for (j = 0; src[j] != '\0' && j < n; j++)
	{
		dest[i] = src[j];
		i++;
	}

	if (j < n)
	{
		dest[i] = '\0';
	}

	return (s);
}

/**
 * find_character - Locates a character in a string
 * @s: The string to be searched
 * @c: The character to look for
 *
 * Return: A pointer to the first occurrence of
 * the character in the string, or NULL if not found.
 */
char *find_character(char *s, char c)
{
	do {
		if (*s == c)
			return (s);
	} while (*s++ != '\0');

	return (NULL);
}

