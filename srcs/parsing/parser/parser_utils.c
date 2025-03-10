/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:59:54 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/03 21:32:08 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Get the size of a tab of strings, terminated by a NULL pointer.
 * 
 * @param	tab The tab of strings, terminated by a NULL pointer.
 * @return	The size of the tab.
 */
size_t	get_tab_size(char **tab)
{
	size_t	i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i])
		i++;
	return (i);
}

static void	error_add_node(bool *error, t_ast_node *new_node)
{
	if (new_node && new_node->type == NODE_COMMAND)
		free_command(new_node->data);
	free(new_node);
	*error = true;
}

/**
 * @brief	Add a new node to the node_tab.
 * 
 * The function add a new node to the array of nodes. It realloc the array
 * and add the new node at the end.
 * 
 * @param	node_tab A pointer to the array of nodes.
 * @param	new_node A pointer to the new node to add.
 * @param	error A pointer to a boolean to set to true if an error occured.
 */
void	add_node(t_ast_node ***node_tab, t_ast_node *new_node, bool *error)
{
	t_ast_node	**new_node_tab;
	size_t		tab_size;

	tab_size = 0;
	new_node_tab = NULL;
	if (!node_tab || !*node_tab || !new_node)
		return (perror("minishell"), error_add_node(error, new_node));
	while ((*node_tab)[tab_size])
		tab_size++;
	new_node_tab = (t_ast_node **)ft_calloc(tab_size + 2, sizeof(t_ast_node *));
	if (!new_node_tab)
		return (perror("minishell"), error_add_node(error, new_node));
	ft_memcpy(new_node_tab, *node_tab, tab_size * sizeof(t_ast_node *));
	new_node_tab[tab_size] = new_node;
	new_node_tab[tab_size + 1] = NULL;
	free(*node_tab);
	*node_tab = new_node_tab;
}

/**
 * @brief	Create a new structure t_file.
 * 
 * The function allocate memory for a new structure t_file and set the
 * path of the file if it is provided.
 * 
 * @param	path The path of the file.
 * @param	append True if the redirection is (<< or >>), false otherwise.
 * @return	A pointer to the new structure t_file
 * 			or NULL if an error occured.
 */
t_file	*create_file(char *path, bool append)
{
	t_file	*file;

	file = (t_file *)ft_calloc(1, sizeof(t_file));
	if (!file)
		return (perror("minishell"), NULL);
	file->expand = true;
	if (path)
	{
		file->name = ft_strdup(path);
		if (!file->name)
			return (perror("minishell"), free(file), NULL);
		if (file->name && \
			(ft_strchr(file->name, '\'') || ft_strchr(file->name, '\"')))
			file->expand = false;
	}
	file->append = append;
	return (file);
}

/**
 * @brief	Create a new structure t_command.
 * 
 * The function allocate memory for a new structure t_command and set the
 * path of the command if it is provided.
 * 
 * @param	path The path of the command.
 * @return	A pointer to the new structure t_command 
 * 			or NULL if an error occured.
 */
t_command	*create_command(char *path)
{
	t_command	*command;

	command = (t_command *)ft_calloc(1, sizeof(t_command));
	if (!command)
		return (perror("minishell"), NULL);
	if (path)
	{
		command->path = ft_strdup(path);
		if (!command->path)
			return (perror("minishell"), free(command), NULL);
	}
	command->fd[0] = STDIN_FILENO;
	command->fd[1] = STDOUT_FILENO;
	return (command);
}
