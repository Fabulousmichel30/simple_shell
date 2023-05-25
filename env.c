#include "shell.h"

/**
 * populate_env_list - Populates the environment linked list
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Always 0
 */
int populate_env_list(info_t *info)
{
	size_t i = 0;
	list_t *node = NULL;

	while (environ[i])
	{
		add_node_end(&node, environ[i], 0);
		i++;
	}
	info->env = node;
	return (0);
}

/**
 * _myenv - Prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 *
 * Return: Always 0
 */
int _myenv(info_t *info)
{
	print_list_str(info->env);
	return (0);
}

/**
 * _getenv - Gets the value of an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * @name: Environment variable name
 *
 * Return: The value of the environment variable
 */
char *_getenv(info_t *info, const char *name)
{
	list_t *node = info->env;

	while (node)
	{
		char *p = starts_with(node->str, name);

		if (p && *p)
			return (p);

		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenv - Initializes new environment variable or modifies an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 *
 * Return: Always 0
 */
int _mysetenv(info_t *info)
{
	if (info->argc != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}
	if (_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - Removes an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 *
 * Return: Always 0
 */
int _myunsetenv(info_t *info)
{
	if (info->argc == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}
	for (int i = 1; i <= info->argc; i++)
		_unsetenv(info, info->argv[i]);

	return (0);
}

