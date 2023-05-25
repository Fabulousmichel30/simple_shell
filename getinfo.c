#include "shell.h"

/**
 * clear_info - Initializes info_t struct
 * @info: Pointer to the info_t struct
 *
 * Initializes the fields of the info_t struct to their default values.
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - Initializes info_t struct
 * @info: Pointer to the info_t struct
 * @av: Argument vector
 *
 * Initializes fields of info_t struct using the provided argument vector.
 * It also performs alias and variable replacement on the argument string.
 */
void set_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		else
		{
			for (i = 0; info->argv[i]; i++)
				;
			info->argc = i;
		}

		replace_alias(info);
		replace_vars(info);
	}
}

/**
 * free_info - Frees info_t struct fields
 * @info: Pointer to the info_t struct
 * @all: True if freeing all fields
 *
 * Frees the dynamically allocated fields of the info_t struct.
 * If `all` is true, it also frees additional fields and performs cleanup.
 */
void free_info(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;

	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		info->arg = NULL;

		free_list(&(info->env));
		info->env = NULL;

		free_list(&(info->history));
		info->history = NULL;

		free_list(&(info->alias));
		info->alias = NULL;

		ffree(info->environ);
		info->environ = NULL;

		bfree((void **)info->cmd_buf);
		info->cmd_buf = NULL;

		if (info->readfd > 2)
			close(info->readfd);

		_putchar(BUF_FLUSH);
	}
}

