/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:29:47 by llemmel           #+#    #+#             */
/*   Updated: 2025/02/27 17:28:35 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Add a pipe node to the AST
 * 
 * @param	ast A pointer to the AST
 * @param	pipe_chain A pointer to a boolean indicating 
 * if there are multiple pipes
 * @param	node_tab A pointer to the array of nodes
 * @param	i A pointer to the index of the current node
 */
static void	add_pipe_to_ast(t_ast_node **ast, bool *pipe_chain, \
	t_ast_node **node_tab, size_t *i)
{
	t_ast_node	*pipe_node;

	pipe_node = node_tab[*i];
	if (!*pipe_chain && *i > 0)
	{
		*pipe_chain = true;
		add_left_right(pipe_node, node_tab[*i - 1], node_tab[*i + 1]);
	}
	else
		add_left_right(pipe_node, *ast, node_tab[*i + 1]);
	*ast = pipe_node;
	*i += 1;
}

/**
 * @brief	Create AST from node_tab
 * 
 * This function creates an AST from the array of nodes created by the parser.
 * 
 * @param	shell A pointer to the shell structure
 * @param	node_tab A pointer to the array of nodes
 * @return	true if the AST was created successfully, false otherwise
 */
bool	make_ast(t_shell *shell, t_ast_node **node_tab)
{
	t_ast_node	*ast;
	bool		pipe_chain;
	size_t		i;

	i = 0;
	ast = NULL;
	pipe_chain = false;
	shell->nb_command = get_command_count(node_tab);
	if (!get_next_pipe(node_tab, &i))
	{
		shell->ast = node_tab[0];
		return (true);
	}
	while (node_tab[i])
	{
		if (get_next_pipe(node_tab, &i))
			add_pipe_to_ast(&ast, &pipe_chain, node_tab, &i);
		else
			break ;
		i++;
	}
	shell->ast = ast;
	return (true);
}
