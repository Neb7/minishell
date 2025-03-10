/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 22:23:50 by capi              #+#    #+#             */
/*   Updated: 2025/03/03 14:05:06 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief	Create a new node for an AST.
 * 
 * @param	data The data of the node
 * @param	root The root of the AST
 * @param	type The type of the node
 * @return	t_ast_node* A pointer to the new node or NULL if an error occurs.
 */
t_ast_node	*create_node(void *data, t_ast_node *root, t_node_type type)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->data = data;
	node->type = type;
	if (root)
		node->root = root;
	else
		node->root = node;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
