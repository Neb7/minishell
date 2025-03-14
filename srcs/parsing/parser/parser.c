/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:33:55 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/10 01:25:46 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Set error to true and return false.
 * 
 * @param	error A pointer to a boolean to set to true.
 * @param	cmd A pointer to the command structure to free.
 * @return	false
 */
bool	ret_false_error(bool *error, t_command *cmd)
{
	if (cmd)
		free_command(cmd);
	*error = true;
	return (false);
}

/**
 * @brief	Parse the tokens and create an array of nodes.
 * 
 * The parser check every tokens and create a node for each simple command
 * and pipe. The nodes are stored in the shell structure.
 * 
 * @param	shell A pointer to the shell structure.
 * @return	true if the parsing has been done successfully, false otherwise.
 */
bool	parser(t_shell *shell)
{
	bool	error;
	size_t	i;

	i = 0;
	error = false;
	shell->node_tab = (t_ast_node **)ft_calloc(1, sizeof(t_ast_node *));
	if (!shell->tokens || !shell->node_tab)
		return (perror("minishell"), free(shell->node_tab), false);
	while (shell->tokens[i].value)
	{
		if (!shell->tokens[i].value)
			break ;
		if (!get_simple_command(shell, &i, &error) \
			|| error == true)
			return (false);
		if (!shell->tokens[i].value)
			break ;
		if (!get_pipe(shell, &i, &error) || error == true)
			return (false);
	}
	return (true);
}
