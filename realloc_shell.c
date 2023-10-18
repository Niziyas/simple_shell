#include "shell.h"

/**
 * fillMemory - fills memory with a constant byte
 * @s: the pointer to the memory area
 * @b: the byte to fill *s with
 * @n: the amount of bytes to be filled
 * Return: (s) a pointer to the memory area s
 */
char *fillMemory(char *s, char b, unsigned int n)
{
	unsigned int i = 0;

	while (i < n)
	{
		s[i] = b;
		i++;
	}
	return (s);
}

/**
 * freeStrings - frees a string of strings
 * @pp: string of strings
 */
void freeStrings(char **pp)
{
	char **a = pp;
	int i;

	if (!pp)
		return;
	for (i = 0; pp[i]; i++)
	{
		free(pp[i]);
	}
	free(a);
}

/**
 * myRealloc - reallocates a block of memory
 * @ptr: pointer to previous malloc'ated block
 * @old_size: byte size of the previous block
 * @new_size: byte size of the new block
 *
 * Return: pointer to the reallocated block
 */
void *myRealloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;
	unsigned int i;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	p = malloc(new_size);
	if (!p)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	for (i = 0; i < old_size; i++)
		p[i] = ((char *)ptr)[i];

	free(ptr);
	return (p);
}
