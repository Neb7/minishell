/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:31:17 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/05 15:55:42 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Check if it's the last command
 * 
 * @param	shell Our big struct
 * @param	tmp Struct of our ast
 * @param	last_com Bool true if it's the last command
 */
static void	ft_check_node(t_shell *shell, t_ast_node *tmp, bool last_com)
{
	t_command	*com;

	com = (t_command *)tmp->data;
	if (!com->args || !com->args[0])
		return ((void)sup_here_doc(com), ft_lstclear(&com->input_file, \
		free_file), ft_lstclear(&com->output_file, free_file));
	if (last_com)
		ft_pars_exec_last(shell, tmp->data);
	else
		ft_pars_exec(shell, tmp->data);
}

/**
 * @brief	Create an array of int[2] with mwlloc for pipe of all command
 * 
 * @param	shell Our big struct
 * @return	true 
 * @return	false 
 */
static bool	ft_init_pipe(t_shell *shell)
{
	size_t	i;

	shell->pid = (int *)ft_calloc(sizeof(int), shell->nb_command);
	if (!shell->pid)
		return (perror("minishell"), false);
	shell->pipe = (int **)ft_calloc(shell->nb_command, sizeof(int *));
	if (!shell->pipe)
		return (perror("minishell"), false);
	i = 0;
	while (i < shell->nb_command)
	{
		shell->pipe[i] = (int *)ft_calloc(2, sizeof(int));
		if (!shell->pipe[i])
			return (ft_free_pipe(shell), false);
		if (pipe(shell->pipe[i]) < 0)
			return (ft_free_pipe(shell), false);
		i++;
	}
	return (true);
}

/**
 * @brief	Initialise handle of signal and active SIGQUIT, and init all pipe
 * 
 * @param	shell Our big struct
 * @return	true 
 * @return	false 
 */
static bool	ft_prep_exe(t_shell *shell)
{
	shell->nb_com = 0;
	if (!ft_init_pipe(shell))
		return (perror("minishell"), false);
	ft_init_sig(SIGINT, ft_ges_in_exe);
	ft_init_sig(SIGQUIT, ft_ges_in_exe);
	return (true);
}

/**
 * @brief	Browse ast and execute itch command
 * 
 * @param	shell Our big struct
 * @param	tmp Struct of our ast
 */
static void	ft_browse_ast(t_shell *shell, t_ast_node *tmp)
{
	while (tmp->left != NULL)
		tmp = tmp->left;
	while (tmp != shell->ast)
	{
		if (!ft_init_our_envp(shell->global_var, -1, shell))
			return (ft_execute_wait(shell), ft_error_malloc(shell));
		if (tmp->type == NODE_PIPE)
			ft_check_node(shell, tmp->right, false);
		else
			ft_check_node(shell, tmp, false);
		shell->idx_pid++;
		tmp = tmp->parent;
		shell->nb_com++;
	}
	if (!ft_init_our_envp(shell->global_var, -1, shell))
		return (ft_execute_wait(shell), ft_error_malloc(shell));
	if (tmp->type == NODE_PIPE)
		ft_check_node(shell, tmp->right, true);
	else
		ft_check_node(shell, tmp, true);
	ft_free_array(&shell->envp);
}

/**
 * @brief	Will execute all command
 * 
 * @param	shell Our big struct
 * @return	true 
 * @return	false 
 */
bool	ft_execute(t_shell *shell)
{
	t_ast_node	*tmp;

	tmp = shell->ast;
	if (!tmp)
		return (true);
	if (!ft_prep_exe(shell))
		return (false);
	ft_browse_ast(shell, tmp);
	if (shell->nb_command == 1)
		ft_edit__var(shell, shell->ast->data);
	ft_execute_wait(shell);
	free(shell->pid);
	shell->pid = NULL;
	return (true);
}
