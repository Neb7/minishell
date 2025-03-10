/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_ast_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:31:26 by llemmel           #+#    #+#             */
/*   Updated: 2025/02/27 15:56:00 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Get the number of command nodes in the array of nodes.
 * 
 * @param	node_tab A pointer to the array of nodes.
 * @return	The number of command nodes.
 */
size_t	get_command_count(t_ast_node **node_tab)
{
	size_t	i;
	size_t	command_count;

	i = 0;
	command_count = 0;
	while (node_tab[i])
	{
		if (node_tab[i]->type == NODE_COMMAND)
			command_count++;
		i++;
	}
	return (command_count);
}

/**
 * @brief	Put the next position of a pipe in pos.
 * 
 * @param	node_tab A pointer to the array of nodes.
 * @param	pos A pointer to the index of the current node.
 * @return	true if a pipe was found, false otherwise.
 */
bool	get_next_pipe(t_ast_node **node_tab, size_t *pos)
{
	size_t	i;

	i = *pos;
	while (node_tab[i] && node_tab[i]->type != NODE_PIPE)
		i++;
	if (!node_tab[i])
		return (false);
	*pos = i;
	return (true);
}
