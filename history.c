#include "shell.h"

/**
 * get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containing history file
 */
char *get_history_file(info_t *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME=");
	if (!dir)
		return NULL;

	size_t dir_len = _strlen(dir);
	size_t file_len = _strlen(HIST_FILE);
	buf = malloc(sizeof(char) * (dir_len + file_len + 2));
	if (!buf)
		return NULL;

	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return buf;
}

/**
 * write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(info_t *info)
{
	char *filename = get_history_file(info);
	if (!filename)
		return -1;

	ssize_t fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return -1;

	list_t *node = info->history;
	while (node)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
		node = node->next;
	}

	_putfd(BUF_FLUSH, fd);
	close(fd);
	return 1;
}

/**
 * read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_history(info_t *info)
{
	char *filename = get_history_file(info);
	if (!filename)
		return 0;

	ssize_t fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return 0;

	struct stat st;
	if (!fstat(fd, &st) && st.st_size >= 2)
	{
		char *buf = malloc(sizeof(char) * (st.st_size + 1));
		if (!buf)
			return 0;

		ssize_t rdlen = read(fd, buf, st.st_size);
		buf[st.st_size] = '\0';
		close(fd);

		if (rdlen > 0)
		{
			int last = 0, linecount = 0;
			for (int i = 0; i < rdlen; i++)
			{
				if (buf[i] == '\n')
				{
					buf[i] = '\0';
					build_history_list(info, buf + last, linecount++);
					last = i + 1;
				}
			}

			if (last != rdlen)
				build_history_list(info, buf + last, linecount++);

			free(buf);
			info->histcount = linecount;
			while (info->histcount >= HIST_MAX)
			{
				delete_node_at_index(&(info->history), 0);
				info->histcount--;
			}
		}
		else
		{
			close(fd);
			free(buf);
			return 0;
		}
	}
	else
	{
		close(fd);
		return 0;
	}

	renumber_history(info);
	return info->histcount;
}

/**
 * build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int build_history_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}
