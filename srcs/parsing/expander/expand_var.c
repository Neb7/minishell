/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:18:19 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/09 23:34:20 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Replace all whitespaces by a single space.
 * 
 * @param	value A pointer to the value.
 * @param	i The index of the value.
 * @return	char* The new value.
 */
static char	*replace_wspace(char *value, size_t i)
{
	size_t	next_char;

	next_char = i;
	while (value[next_char] && ft_strchr(WSPACE, value[next_char]))
		next_char++;
	if (i != next_char && (!value[next_char] || !i))
		value = del_charset_n(value, WSPACE, i, next_char);
	else if (i != next_char)
		value = del_charset_n(value, WSPACE, i + 1, next_char);
	if (!value)
		return (NULL);
	return (value);
}

/**
 * @brief	This function replace all whitespaces by a single space if the
 * 			variable is not in double quotes.
 * 			If the value of the variable contains quotes or double quotes,
 * 			the function will add a backslash before the quotes.
 * 
 * example: "hello   'world'" -> "hello \'world\'"
 * 
 * @param	value the value to modify
 * @param	in_quotes if the value is in quotes
 * @return	char* The new value.
 */
static char	*world_splitting(char *value, bool in_quotes)
{
	size_t	i;

	i = 0;
	if (!value)
		return (NULL);
	while (value[i])
	{
		if (!in_quotes)
		{
			value = replace_wspace(value, i);
			if (!value)
				return (NULL);
		}
		if (value[i] == '\'' || value[i] == '\"')
		{
			value = insert_char(value, '\\', i);
			if (!value)
				return (NULL);
			i += 1;
		}
		if (value[i])
			i++;
	}
	return (value);
}

/**
 * @brief	Get the value of the variable, with a world splitting if needed.
 * 
 * @param	shell The shell structure
 * @param	var_name The name of the variable.
 * @param	in_quotes true if the variable is in quotes, false otherwise.
 * @return	char* The value of the variable if it exists, NULL otherwise.
 */
char	*get_split_var_value(t_shell shell, char *var_name, bool in_quotes)
{
	char	*var_value;

	if (!var_name)
		return (NULL);
	if (!ft_strcmp(var_name, "?"))
		var_value = ft_itoa(shell.last_exit_status);
	else
		var_value = ft_strdup(get_envvar_value(shell, var_name));
	if (!var_value)
		return (perror("minishell"), NULL);
	var_value = world_splitting(var_value, in_quotes);
	if (!var_value)
		return (NULL);
	return (var_value);
}

/**
 * @brief	Replace the variable name with its value.
 * 
 * @param	value The value to replace.
 * @param	i The index of the variable name.
 * @param	var_name_size The size of the variable name.
 * @param	var_value The value of the variable.
 * @return	char* The new value.
 */
static char	*replace_envvar(char *value, size_t *i, \
	size_t var_name_size, char *var_value)
{
	char	*new_value;
	size_t	new_value_size;

	new_value_size = ft_strlen(value) - var_name_size + ft_strlen(var_value);
	new_value = (char *)ft_calloc(new_value_size + 1, sizeof(char));
	if (!new_value)
		return (free(value), NULL);
	ft_strlcpy(new_value, value, *i + 1);
	ft_strlcat(new_value, var_value, new_value_size + 1);
	ft_strlcat(new_value, value + *i + var_name_size + 1, new_value_size + 1);
	return (free(value), new_value);
}

/**
 * @brief	Get the value of the variable and replace $var_name with it.
 * 			if var_name is not found, replace $var_name with an empty string.
 * 
 * @param	shell The shell structure
 * @param	value A pointer to the token value.
 * @param	i A pointer to the index of the value.
 * @return	true if the variable is successfully expanded,
 * 			false if an error occurs.
 */
bool	expand_var(t_shell shell, char **value, size_t *i, bool in_quotes)
{
	char	*var_value;
	char	*var_name;
	size_t	name_size;

	if (!value || !*value)
		return (false);
	name_size = 1;
	var_name = get_var_name(*value + *i + 1, &name_size);
	if ((*value)[*i + 1] == '?' \
		|| (!ft_strncmp(*value, "~", 1) \
		&& ((*value)[1] == '/' || !(*value)[1])))
		var_value = get_special_var_value(shell, *value, i, in_quotes);
	else
	{
		if (!name_size && ((*value)[*i + 1] != '\'' && (*value)[*i + 1] != '"'))
			return (((void)(*i += 1)), true);
		var_value = get_split_var_value(shell, var_name, in_quotes);
	}
	if (!var_value)
		return (free(var_name), false);
	*value = replace_envvar(*value, i, name_size, var_value);
	if (!*value)
		return (free(var_name), free(var_value), false);
	*i += ft_strlen(var_value);
	return (free(var_name), free(var_value), true);
}
