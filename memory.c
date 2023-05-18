#include "shell.h"

/**
 * bfree - freeing a pointer and rendering the address NULL
 * @ptr: free the pointers address
 *
 * Return: 1 if freed, if not 0.
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
