#include "shell.h"

/**
 * _eputs - Prints an input string to stderr
 * @str: The string to be printed
 *
 * Return: Nothing
 */
void _eputs(char *str)
{
	int index = 0;

	if (!str)
		return;
	while (str[index] != '\0')
	{
		_eputchar(str[index]); /* Print each character to stderr */
		index++;
	}
}

/**
 * _eputchar - Writes the character 'c' to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int _eputchar(char c)
{
	static int index;
	static char buffer[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(2, buffer, index); /* Write the buffer to stderr */
		index = 0;
	}
	if (c != BUF_FLUSH)
		buffer[index++] = c; /* Store character in buffer */
	return (1);
}

/**
 * _putfd - Writes the character 'c' to the given file descriptor 'fd'
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int _putfd(char c, int fd)
{
	static int index;
	static char buffer[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(fd, buffer, index); /* Write buffer to specified file descriptor */
		index = 0;
	}
	if (c != BUF_FLUSH)
		buffer[index++] = c; /* Store character in buffer */
	return (1);
}

/**
 * _putsfd - Prints an input string to the specified file descriptor
 * @str: The string to be printed
 * @fd: The file descriptor to write to
 *
 * Return: The number of characters written
 */
int _putsfd(char *str, int fd)
{
	int count = 0;

	if (!str)
		return (0);
	while (*str)
	{
		count += _putfd(*str++, fd); /* Print each character to file descriptor */
	}
	return (count);
}

