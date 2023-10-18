#include "shell.h"

/**
 * count_list - determines the length of linked list
 * @h: pointer to the first node
 *
 * Return: size of the list
 */
size_t count_list(const list_t *h)
{
	size_t count = 0;

	for (; h; h = h->next)
	{
		count++;
	}

	return (count);
}

/**
 * convert_list_to_strings - returns an array of strings from the list->str
 * @head: pointer to the first node
 *
 * Return: array of strings
 */
char **convert_list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t count = count_list(head);
	char **strings;
	char *str;
	size_t i = 0, j;

	if (!head || count == 0)
		return (NULL);

	strings = malloc(sizeof(char *) * (count + 1));
	if (!strings)
		return (NULL);

	while (node)
	{
		str = string_duplicate(node->str);
		if (!str)
		{
			for (j = 0 ; j < i; j++)
				free(strings[j]);
			free(strings);
			return (NULL);
		}

		strings[i] = str;
		node = node->next;
		i++;
	}

	strings[i] = NULL;
	return (strings);
}

/**
 * print_history - prints all elements of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t print_history(const list_t *h)
{
	size_t i = 0;

	for (; h; h = h->next, i++)
	{
		print_string(int_to_base(h->num, 10, 0));
		write_char_to_stdout(':');
		write_char_to_stdout(' ');
		print_string(h->str ? h->str : "(nil)");
		print_string("\n");
	}

	return (i);
}

/**
 * find_node_with_prefix - returns a node whose string
 * starts with a given prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after the prefix to match
 *
 * Return: matching node or NULL
 */
list_t *find_node_with_prefix(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	for (; node; node = node->next)
	{
		p = find_start(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
	}

	return (NULL);
}

/**
 * find_node_index - finds the index of a node in a linked list
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of the node or -1 if not found
 */
ssize_t find_node_index(list_t *head, list_t *node)
{
	size_t index = 0;

	for (; head; head = head->next, index++)
	{
		if (head == node)
			return (index);
	}

	return (-1);
}
