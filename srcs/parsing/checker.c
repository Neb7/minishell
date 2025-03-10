/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:57:14 by llemmel           #+#    #+#             */
/*   Updated: 2025/02/27 16:01:36 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Check if the command is an executable.
 * 
 * @param	command The command to check.
 * @return	true if the command is an executable, false otherwise.
 */
bool	is_executable(char *command)
{
	return (*command == '/' \
		|| !ft_strncmp(command, "./", 2) \
		|| !ft_strncmp(command, "../", 3));
}

/**
 * @brief	Check if the token is a redirection.
 * 
 * @param	token the token to check.
 * @return	true if the token is a redirection, false otherwise.
 */
bool	is_redirection(t_token token)
{
	return (token.type == TYPE_REDIRECT_INPUT \
		|| token.type == TYPE_REDIRECT_OUTPUT \
		|| token.type == TYPE_REDIRECT_APPEND
		|| token.type == TYPE_HERE_DOC);
}

/**
 * @brief	Check if the value is a builtins.
 * 
 * @param	value The value to check.
 * @return	true if the value is a builtins, false otherwise.
 */
bool	is_builtins(char *value)
{
	if (ft_strncmp(value, "echo", 5) == 0)
		return (true);
	else if (ft_strncmp(value, "cd", 3) == 0)
		return (true);
	else if (ft_strncmp(value, "pwd", 4) == 0)
		return (true);
	else if (ft_strncmp(value, "export", 7) == 0)
		return (true);
	else if (ft_strncmp(value, "unset", 6) == 0)
		return (true);
	else if (ft_strncmp(value, "env", 4) == 0)
		return (true);
	else if (ft_strncmp(value, "exit", 5) == 0)
		return (true);
	return (false);
}

/**
 * @brief	Check if the token is an operator.
 * 
 * @param	token The token to check.
 * @return	true if the token is an operator, false otherwise.
 */
bool	is_operator(t_token token)
{
	return (token.type == TYPE_PIPE
		|| token.type == TYPE_REDIRECT_INPUT
		|| token.type == TYPE_REDIRECT_OUTPUT
		|| token.type == TYPE_REDIRECT_APPEND
		|| token.type == TYPE_HERE_DOC);
}

/**
 * @brief	Check if the token is a command.
 * 
 * @param	tokens The tokens array.
 * @param	i The index of the token.
 * @return	true if the token is a command, false otherwise.
 */
bool	is_command(t_token *tokens, size_t i)
{
	if (i == 0 && tokens[i].type == TYPE_ARG)
		return (true);
	else if (i > 0 && tokens[i].type == TYPE_ARG \
		&& tokens[i - 1].type == TYPE_PIPE)
		return (true);
	return (false);
}
