#include "shell.h"

/**
 * prepend_node - Prepends a node to the start of the list
 * @head: Address of the pointer to the head node
 * @str: String field of the node
 * @num: Node index used by history
 *
 * Return: The size of the list
 */
list_t *prepend_node(list_t **head, const char *str, int num)
{
	list_t *new_head;

	if (!head)
		return (NULL);

	new_head = malloc(sizeof(list_t));

	if (!new_head)
		return (NULL);

	fillMemory((void *)new_head, 0, sizeof(list_t));
	new_head->num = num;

	if (str)
	{
		new_head->str = string_duplicate(str);

		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}

	new_head->next = *head;
	*head = new_head;

	return (new_head);
}

/**
 * prepend_node_end - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
list_t *prepend_node_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);
	fillMemory((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;
	if (str)
	{
		new_node->str = string_duplicate(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * print_environment_list - Prints the str element of a list_t linked list
 * @head: Pointer to the first node
 *
 * Return: Size of the list
 */
size_t print_environment_list(const list_t *head)
{
	size_t count = 0;

	for (; head; head = head->next)
	{
		print_string(head->str ? head->str : "(nil)");
		print_string("\n");
		count++;
	}

	return (count);
}

/**
 * remove_node_at_index - Removes a node at the given index
 * @head: Address of the pointer to the first node
 * @index: Index of the node to remove
 *
 * Return: 1 on success, 0 on failure
 */
int remove_node_at_index(list_t **head, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (index == 0)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}

	node = *head;
	prev_node = NULL;
	for (; node; i++, prev_node = node, node = node->next)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
	}

	return (0);
}

/**
 * clear_list - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void clear_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;

	head = *head_ptr;
	for (node = head; node; node = next_node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
	}

	*head_ptr = NULL;
}
