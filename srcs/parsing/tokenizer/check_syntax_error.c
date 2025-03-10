/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:58:23 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/06 12:48:25 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Print an error message for the given token.
 * 
 * @param	token The token to print the error message for.
 */
static void	syntax_error(t_token token)
{
	ft_putstr_fd(UNEXPECTED_TOKEN, STDERR_FILENO);
	ft_putstr_fd(token.value, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

/**
 * @brief	Check if a syntax error is present in the given token array.
 * 
 * @param	tokens The token array to check.
 * @return	true if a syntax error is present, false otherwise. 
 */
bool	check_syntax_error(t_token *tokens)
{
	size_t	i;

	i = 0;
	if (!tokens)
		return (false);
	if (tokens[i].type == TYPE_PIPE)
		return (syntax_error(tokens[i]), true);
	while (tokens[i].value)
	{
		if (tokens[i].type == TYPE_PIPE && is_redirection(tokens[i + 1]))
		{
			i++;
			continue ;
		}
		if (is_operator(tokens[i]) && is_operator(tokens[i + 1]))
			return (syntax_error(tokens[i]), true);
		if (is_operator(tokens[i]) && !tokens[i + 1].value)
			return (syntax_error(tokens[i]), true);
		i++;
	}
	return (false);
}
