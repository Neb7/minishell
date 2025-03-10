/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:07:20 by llemmel           #+#    #+#             */
/*   Updated: 2025/02/27 15:56:00 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Add a new pipe node to the node_tab.
 * 
 * @param	shell A pointer to the shell structure.
 * @param	i A pointer to the index of the current token.
 * @param	error A pointer to the error flag.
 * @return	true if the pipe node was added successfully, false otherwise.
 * 			If an error occurs, the error flag is set to true.
 */
bool	get_pipe(t_shell *shell, size_t *i, bool *error)
{
	if (*error)
		return (false);
	if (shell->tokens[*i].type == TYPE_PIPE)
	{
		add_node(&shell->node_tab, create_node(NULL, NULL, NODE_PIPE), error);
		*i += 1;
	}
	if (*error)
		return (false);
	return (true);
}
