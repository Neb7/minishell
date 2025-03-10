/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:21:23 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/03 21:09:18 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Free a token structure.
 * 
 * @param	tokens The token structure to free.
 */
void	free_token(t_token *tokens)
{
	size_t	i;

	i = 0;
	while (tokens && tokens[i].value)
	{
		free(tokens[i].value);
		free(tokens[i].raw_value);
		i++;
	}
	free(tokens);
}

/**
 * @brief	Free a tab of strings.
 * 
 * @param	tab The tab to free.
 */
void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

/**
 * @brief	Free a file structure.
 * 
 * The file is closed and the name is freed. If addr is NULL, nothing is done.
 * 
 * @param	addr The address of the file structure to free.
 */
void	free_file(void *addr)
{
	t_file	*file;

	if (!addr)
		return ;
	file = (t_file *)addr;
	if (file->fd > 2)
		close(file->fd);
	if (file->name)
		free(file->name);
	free(file);
}

/**
 * @brief	Free a command structure.
 * 
 * The path, args, input_file and output_file are freed. 
 * All here_doc file are also deleted.
 * If command is NULL, nothing is done.
 * 
 * @param	command A pointer to the command structure to free.
 */
void	free_command(t_command *command)
{
	if (!command)
		return ;
	if (command->path)
		free(command->path);
	if (command->args)
		free_tab(command->args);
	sup_here_doc(command);
	ft_lstclear(&command->input_file, free_file);
	ft_lstclear(&command->output_file, free_file);
	free(command);
}

/**
 * @brief	Free an array of node.
 * 
 * Each node is freed and the array of node is freed. 
 * After the function, the pointer to the array is set to NULL.
 * 
 * @param	node_tab A pointer to the array of node to free.
 */
void	free_tab_node(t_ast_node **node_tab)
{
	size_t	i;

	i = 0;
	if (!node_tab)
		return ;
	while (node_tab[i])
	{
		if (node_tab[i]->data && node_tab[i]->type == NODE_COMMAND)
		{
			free_command(node_tab[i]->data);
			node_tab[i]->data = NULL;
		}
		free(node_tab[i]);
		i++;
	}
	free(node_tab);
}
