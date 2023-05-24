#include "shell.h"

/**
 * get_environ - Returns a copy of the string array 'environ'.
 * @info: Structure containing potential arguments.
 *        Used to maintain constant function prototype.
 *
 * Return: The string array copy of 'environ'.
 */
char **get_environ(info_t *info)
{
	if (info->environ == NULL || info->env_changed)
	{
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _setenv - Initializes new environment variable or modifies existing one.
 * @info: Structure containing potential arguments.
 *        Used to maintain constant function prototype.
 * @var: The environment variable name.
 * @value: The environment variable value.
 *
 * Return: Always 0.
 */
int _setenv(info_t *info, char *var, char *value)
{
	char *buf;
	list_t *node;
	char *p;

	if (var == NULL || value == NULL)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (buf == NULL)
		return (1);

	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);

	node = info->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}

	add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}

/**
 * _unsetenv - Removes an environment variable.
 * @info: Structure containing potential arguments.
 *        Used to maintain constant function prototype.
 * @var: The environment variable name to remove.
 *
 * Return: 1 on successful deletion, 0 otherwise.
 */
int _unsetenv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (node == NULL || var == NULL)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}

	return (info->env_changed);
}

