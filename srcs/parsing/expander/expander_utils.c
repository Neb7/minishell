/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:49:12 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/04 14:09:40 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Get the value of the variable.
 * 
 * @param	shell The shell structure.
 * @param	var_name The name of the variable.
 * @return	The value of the variable if it exists, an empty string otherwise.
 */
char	*get_envvar_value(t_shell shell, char *var_name)
{
	t_list	*var_lst;

	var_lst = shell.global_var;
	if (!ft_strncmp(var_name, "PATH", 5) && shell.save_path)
		return (DEFAULT_PATH);
	while (var_lst)
	{
		if (!ft_strcmp(((t_var *)var_lst->content)->name, var_name))
		{
			if (!((t_var *)var_lst->content)->value)
				return ("");
			return (((t_var *)var_lst->content)->value);
		}
		var_lst = var_lst->next;
	}
	return ("");
}

/**
 * @brief	Get the size of the variable name.
 * 
 * @param	str A pointer to the string.
 * @return	The size of the variable name.
 */
size_t	get_var_size(char *str)
{
	size_t	var_size;

	var_size = 0;
	while (str[var_size] && str[var_size] != '$' \
			&& (ft_isalnum(str[var_size]) || str[var_size] == '_'))
		var_size++;
	return (var_size);
}

/**
 * @brief	Get the command path.
 * 
 * @param	shell The shell structure
 * @param	value A pointer to the token value.
 * @param	error A pointer to the error flag.
 * @return	The command path if it exists, NULL if an error occurs.
 */
static char	*get_path(t_shell shell, char *value, bool *error)
{
	char	*path;
	char	*command_name;

	errno = 12;
	if (is_executable(value))
	{
		path = ft_strdup(value);
		if (!path)
			return (perror("minishell"), NULL);
	}
	else
	{
		command_name = ft_strrchr(value, '/');
		if (!command_name)
			command_name = value;
		else
			command_name++;
		path = search_command(shell, command_name, error);
		if (!path)
			path = add_default_invalid_path(shell, command_name);
	}
	if (!path || *error)
		return (NULL);
	return (path);
}

/**
 * @brief	Expand the command path.
 * 
 * @param	shell The shell structure
 * @param	token A pointer to the token to expand the command path.
 * @return	true if the command path is successfully expanded,
 * 			false if an error occurs.
 */
bool	expand_command_path(t_shell shell, t_token *token)
{
	bool	error;
	char	*new_path;

	error = false;
	if (!token || !token->value)
		return (false);
	new_path = get_path(shell, token->value, &error);
	if (error)
		return (false);
	if (new_path)
	{
		free(token->value);
		token->value = new_path;
	}
	return (true);
}

/**
 * @brief	Add a character to a string at index.
 * 
 * The function don't replace the character at index, 
 * it shifts all the characters
 * 
 * @param	value value to modify
 * @param	c the character to insert
 * @param	index the index where to insert the character
 * @return	char* the new string
 * 
 * insert_char("'   wer  '", "\", 0) --> "\'   wer  '"
 * insert_char("\'   wer  '", "\", 10) --> "\'   wer  \'"
 */
char	*insert_char(char *value, char c, size_t index)
{
	char	*new_value;

	new_value = (char *)ft_calloc(ft_strlen(value) + 2, sizeof(char));
	if (!new_value)
		return (perror("minishell"), free(value), NULL);
	ft_strlcpy(new_value, value, index + 1);
	new_value[index] = c;
	ft_strlcat(new_value, value + index, ft_strlen(value) + 2);
	free(value);
	return (new_value);
}
