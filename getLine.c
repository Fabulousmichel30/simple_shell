include "shell.h"

/**
 * read_buf - Reads data into a buffer.
 * @info: Parameter struct.
 * @buf: Buffer.
 * @i: Size of the buffer.
 *
 * Return: Number of bytes read.
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);

	r = read(info->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;

	return (r);
}

/**
 * _getline - Retrieves the next line of input from STDIN.
 * @info: Parameter struct.
 * @ptr: Address of a pointer to the buffer (preallocated or NULL).
 * @length: Size of the preallocated buffer pointed to by ptr (if not NULL).
 *
 * Return: Number of bytes read.
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
	{
		i = len = 0;
		r = read_buf(info, buf, &len);
		if (r == -1 || (r == 0 && len == 0))
			return (-1);
	}
	c = _strchr(buf + i, '\n');
	size_t k = c ? 1 + (unsigned int)(c - buf) : len;

	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p)
	{
		if (p)
			free(p);
		return (-1);
	}
	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);
	s += k - i;
	i = k;
	p = new_p;
	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - Handles the SIGINT signal (Ctrl+C).
 * @sig_num: The signal number.
 *
 * Return: None.
 */
void sigintHandler(__attribute__((unused)) int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

/**
 * input_buf - Buffers chained commands.
 * @info: Parameter struct.
 * @buf: Address of the buffer.
 * @len: Address of the length variable.
 *
 * Return: Number of bytes read.
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (*len == 0)
	{/* If there is nothing left in the buffer, fill it. */
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);

#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(info, buf, &len_p);
#endif

		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* Remove trailing newline. */
				r--;
			}

			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			/* If (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}

	return (r);
}

/**
 * get_input - Retrieves a line of input without the newline character.
 * @info: Parameter struct.
 *
 * Return: Number of bytes read.
 */
ssize_t get_input(info_t *info)
{
	static char *buf; /* The ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	r = input_buf(info, &buf, &len);
	if (r == -1) /* Reached EOF */
		return (-1);

	if (len)
	{/* There are commands left in the chain buffer. */
		j = i; /* Initialize a new iterator to the current buffer position. */
		p = buf + i; /* Get the pointer for return. */

		check_chain(info, buf, &j, i, len);
		while (j < len)
		{/* Iterate until a semicolon or the end is reached. */
			if (is_chain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* Increment past the nullified ';'. */
		if (i >= len)
		{/* Reached the end of the buffer. Reset position and length. */
			i = len = 0;
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* Pass back the pointer to the current command position. */
		return (_strlen(p)); /* Return the length of the current command. */
	}

	*buf_p = buf; /* Not a chain, pass back the buffer from _getline(). */
	return (r); /* Return the length of the buffer from _getline(). */
}

