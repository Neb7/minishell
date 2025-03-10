/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:32:05 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/09 18:05:30 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Expand variables in a token, and skip quotes if needed.
 * 
 * @param	shell A pointer to the shell structure containing tokens.
 * @param	value A pointer to the token value.
 * @param	skip_quote true if the function should skip quotes, false otherwise.
 * @return	true if the variables are successfully expanded,
 * 			false if an error occurs.
 */
bool	expand_vars(t_shell shell, char **value, bool skip_quote)
{
	bool	in_dquote;
	size_t	i;

	i = 0;
	in_dquote = false;
	if ((!ft_strncmp(*value, "~", 1) && ((*value)[1] == '/' || !(*value)[1])))
		return (expand_var(shell, value, &i, in_dquote));
	else
	{
		while ((*value)[i])
		{
			if ((*value)[i] == '\"')
				in_dquote = !in_dquote;
			if (!in_dquote && skip_quote && (*value)[i] == '\'')
				skip_simple_quote(*value, &i);
			else if ((*value)[i] == '$' \
				&& !check_and_do_expand_var(shell, value, &i, in_dquote))
				return (false);
			else if ((*value)[i])
				i++;
		}
	}
	return (true);
}

/**
 * @brief	The function remove all backslash before quotes.
 * The backslash has been inserted previously with the function expand_var.
 * example : "hello \"world\"" -> "hello "world""
 * 
 * @param	value The value to remove backslash.
 * @return	char* The value without backslash.
 */
char	*remove_backslash_quote(char *value)
{
	size_t	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '\\' && (value[i + 1] == '\'' || value[i + 1] == '\"'))
		{
			value = del_charset_n(value, "\\", i, i + 1);
			if (!value)
				return (NULL);
		}
		i++;
	}
	return (value);
}

/**
 * @brief	Expands a token. (variables and quotes)
 * 
 * @param	shell A pointer to the shell structure containing tokens.
 * @param	value A pointer to the token value.
 * @return	true if the token is successfully expanded,
 * 			false if an error occurs.
 */
bool	expand_token(t_shell shell, char **value)
{
	if (!expand_vars(shell, value, true))
		return (false);
	if (!expand_quote(value))
		return (false);
	*value = remove_backslash_quote(*value);
	if (!*value)
		return (false);
	return (true);
}

/**
 * @brief	Expands the path of the command if it's not builtin.
 * 
 * @param	shell A pointer to the shell structure containing tokens.
 * @param	token A pointer to the command token to expand.
 * @return	true if the command is successfully expanded, 
 *			false if an error occurs. 
 */
static bool	expand_command(t_shell shell, t_token *token)
{
	if (!expand_token(shell, &token->value))
		return (false);
	token->raw_value = ft_strdup(token->value);
	if (!token->raw_value)
		return (perror("minishell"), false);
	if (!is_builtins(token->value))
	{
		if (!expand_command_path(shell, token))
			return (false);
	}
	return (true);
}

/**
 * @brief Expands tokens in the shell.
 *
 * This function expands the tokens (variables, quotes, command path, etc...)
 *
 * @param shell A pointer to the shell structure containing tokens.
 * @return true if all expansions are successful, false if an error occurs.
 */
bool	expander(t_shell *shell)
{
	size_t	i;

	i = 0;
	while (shell->tokens[i].value)
	{
		if (shell->tokens[i].type == TYPE_COMMAND \
			&& !expand_command(*shell, &shell->tokens[i]))
			return (false);
		else if (shell->tokens[i].type == TYPE_ARG \
			&& !expand_token(*shell, &shell->tokens[i].value))
			return (false);
		else if (shell->tokens[i].type == TYPE_FILE \
			&& i > 0 && shell->tokens[i - 1].type != TYPE_HERE_DOC)
		{
			if (!expand_token(*shell, &shell->tokens[i].value))
				return (false);
		}
		i++;
	}
	return (true);
}
