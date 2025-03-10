/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 23:05:04 by capi              #+#    #+#             */
/*   Updated: 2025/02/28 14:35:06 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief	Add a new node to the left of the parent node.
 * 
 * @param	node_parent The parent node
 * @param	node_left The left node
 */
void	add_left(t_ast_node *node_parent, t_ast_node *node_left)
{
	if (!node_left)
		return ;
	if (!node_parent)
	{
		node_parent = node_left;
		return ;
	}
	node_parent->left = node_left;
	node_left->parent = node_parent;
}

/**
 * @brief	Add a new node to the right of the parent node.
 * 
 * @param	node_parent The parent node
 * @param	node_right The right node
 */
void	add_right(t_ast_node *node_parent, t_ast_node *node_right)
{
	if (!node_right)
		return ;
	if (!node_parent)
	{
		node_parent = node_right;
		return ;
	}
	node_parent->right = node_right;
	node_right->parent = node_parent;
}

/**
 * @brief	Add a new node to the left and right of the parent node.
 * 
 * @param	node_parent The parent node
 * @param	node_left The left node
 * @param	node_right The right node
 */
void	add_left_right(t_ast_node *node_parent, \
						t_ast_node *node_left, t_ast_node *node_right)
{
	if (!node_parent || !node_left || !node_right)
		return ;
	node_parent->left = node_left;
	node_parent->left->parent = node_parent;
	node_parent->right = node_right;
	node_parent->right->parent = node_parent;
}
