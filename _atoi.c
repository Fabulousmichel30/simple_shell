#include "shell.h"

/**
 * interactive - Checks if the shell is in interactive mode.
 * @info: Pointer to the struct containing shell information.
 *
 * Return: 1 if in interactive mode, 0 otherwise.
 */
int interactive(info_t *info)
{
	/* Check if the standard input is a terminal */
	if (isatty(STDIN_FILENO))
	{
		/* Check if the read file descriptor is within the range 0-2 */
		if (info->readfd <= 2)
			return (1); /* Shell is in interactive mode */
	}
	return (0); /* Shell is not in interactive mode */
}

/**
 * is_delim - Checks if a character is a delimiter.
 * @c: The character to check.
 * @delim: The delimiter string.
 *
 * Return: 1 if the character is a delimiter, 0 otherwise.
 */
int is_delim(char c, char *delim)
{
	while (*delim)
	{
		if (*delim == c)
			return (1); /* Character is a delimiter */
		delim++;
	}
	return (0); /* Character is not a delimiter */
}

/**
 * _isalpha - Checks if a character is alphabetic.
 * @c: The character to check.
 *
 * Return: 1 if the character is alphabetic, 0 otherwise.
 */
int _isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1); /* Character is alphabetic */
	else
		return (0); /* Character is not alphabetic */
}

/**
 * _atoi - Converts a string to an integer.
 * @s: The string to be converted.
 *
 * Return: 0 if no numbers in the string, the converted number otherwise.
 */
int _atoi(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}

