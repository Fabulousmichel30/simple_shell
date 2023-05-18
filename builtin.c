#include "shell.h"

/**
 * _myhelp - Implements the help command
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int _myhelp(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* Temporary unused workaround */
	return (0);
}

/**
 * _mycd - Implements the cd command to change the current directory
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int _mycd(info_t *info)
{
	char *dir, buffer[1024];
	int chdir_ret;

	if (!info->argv[1])
		dir = _getenv(info, "HOME=");
	else if (_strcmp(info->argv[1], "-") == 0)
		dir = _getenv(info, "OLDPWD=");
	else
		dir = info->argv[1];

	if (!dir)
		dir = _getenv(info, "PWD=");

	chdir_ret = chdir(dir);

	if (chdir_ret == -1)
	{
		print_error(info, "can't cd to ");
		_eputs(dir);
		_eputchar('\n');
	}
	else
	{
		getcwd(buffer, 1024);
		_setenv(info, "OLDPWD", _getenv(info, "PWD="));
		_setenv(info, "PWD", buffer);
	}

	return (0);
}

/**
 * _myexit - Exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Exits with a given exit status (0) if info->argv[0] != "exit"
 */
int _myexit(info_t *info)
{
	int exitcheck;

	if (info->argv[1]) /* If there is an exit argument */
	{
		exitcheck = _erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->err_num = _erratoi(info->argv[1]);
		return (-2);
	}

	info->err_num = -1;
	return (-2);
}

