#include "shell.h"
#include <stdlib.h>

/**
 * get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: pointer to the string array
 */
char **get_environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return info->environ;
}

/**
 * unset_env - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * Return: 1 on delete, 0 otherwise
 */
int unset_env(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return 0;

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
	return info->env_changed;
}

/**
 * set_env - Initialize a new environment variable,
 *           or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 * Return: 0 on success, 1 on failure
 */
int set_env(info_t *info, char *var, char *value)
{
	char *buf;
	list_t *node;
	char *p;

	if (!var || !value)
		return 0;

	buf = strdup(var);
	if (!buf)
		return 1;

	buf = realloc(buf, _strlen(buf) + _strlen(value) + 2);
	if (!buf)
		return 1;

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
			return 0;
		}
		node = node->next;
	}
	add_node_end(&(info->env), buf, 0);
	info->env_changed = 1;
	return 0;
}
