/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:54:38 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/13 13:55:39 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Free a command structure.
 * 
 * The path, args, input_file and output_file are freed. 
 * If command is NULL, nothing is done.
 * 
 * @param	command A pointer to the command structure to free.
 */
void	free_command2(t_command **command)
{
	if (!*command)
		return ;
	if ((*command)->path)
		free((*command)->path);
	if ((*command)->args)
		free_tab((*command)->args);
	ft_lstclear(&(*command)->input_file, free_file);
	ft_lstclear(&(*command)->output_file, free_file);
	free(*command);
}

/**
 * @brief	Free an array of node.
 * 
 * Each node is freed and the array of node is freed. 
 * After the function, the pointer to the array is set to NULL.
 * 
 * @param	node_tab A pointer to the array of node to free.
 */
static void	free_tab_node2(t_ast_node **node_tab)
{
	size_t	i;

	i = 0;
	if (!node_tab)
		return ;
	while (node_tab[i])
	{
		if (node_tab[i]->data && node_tab[i]->type == NODE_COMMAND)
		{
			free_command2((t_command **)&node_tab[i]->data);
			node_tab[i]->data = NULL;
		}
		free(node_tab[i]);
		i++;
	}
	free(node_tab);
}

/**
 * @brief	Free all structures and variables allocated during parsing.
 * 
 * @param	shell A pointer to the shell structure.
 * @param	command_line_trimmed The command line trimmed.
 */
static void	free_var2(t_shell *shell, char *command_line_trimmed)
{
	if (shell->tokens)
	{
		free_token(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->node_tab)
	{
		free_tab_node2(shell->node_tab);
		shell->node_tab = NULL;
	}
	if (command_line_trimmed)
	{
		free(command_line_trimmed);
		command_line_trimmed = NULL;
	}
	shell->ast = NULL;
	shell->nb_command = 0;
}

void	ft_free_child(t_shell *shell, int exit_status)
{
	rl_clear_history();
	if (shell)
	{
		if (shell->pid)
			free(shell->pid);
		if (shell->envp)
			ft_free_array(&shell->envp);
		ft_free_pipe(shell);
		free_var2(shell, NULL);
		free(shell->prompte);
		free(shell->input);
		ft_lstclear(&shell->global_var, ft_del_var);
		free(shell);
	}
	exit(exit_status);
}
