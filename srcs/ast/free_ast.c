/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 23:09:08 by capi              #+#    #+#             */
/*   Updated: 2025/02/28 14:32:38 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief	This function free the ast and the data of each node.
 * 
 * @param	ast The ast to free.
 * @param	del_fct A pointer to the function to free the data of each node.
 */
void	free_ast(t_ast_node *ast, void (*del_fct)(void *))
{
	if (!ast)
		return ;
	if (ast->left)
		free_ast(ast->left, del_fct);
	if (ast->right)
		free_ast(ast->right, del_fct);
	if (del_fct)
		del_fct(ast->data);
	free(ast);
}
