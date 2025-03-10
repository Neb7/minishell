/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_wait.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 17:45:02 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/09 14:45:09 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Make a waitpid for all processe
 * 
 * @param	shell Our big struct
 */
void	ft_execute_wait(t_shell *shell)
{
	shell->idx_pid = 0;
	if (!shell->pid)
		return ;
	while (++(shell->idx_pid) - 1 < (int)shell->nb_command)
	{
		if (shell->pid[shell->idx_pid - 1] > 0)
			waitpid(shell->pid[shell->idx_pid - 1], &shell->status, 0);
		if (g_signal[0] == 1 && ft_sigint(shell))
			break ;
		else if (g_signal[0] == 2 && ft_sigquit(shell))
			break ;
		else if (WTERMSIG(shell->status) == SIGSEGV && ft_sigsegv())
			break ;
		ft_loop_mini(shell);
	}
	if (shell->idx_pid - 1 == (int)shell->nb_command)
		if (shell->pid[shell->idx_pid - 2] > 0)
			ft_last_exit_status(shell);
	ft_free_pipe(shell);
}

/**
 * @brief	Edit the "_" variabvle
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
void	ft_edit__var(t_shell *shell, t_command *com)
{
	t_list	*tmp;
	t_var	*var;
	int		i;

	if (!com || !com->args || !com->args[0])
		return ;
	var = NULL;
	tmp = shell->global_var;
	while (tmp != NULL)
	{
		var = (t_var *)tmp->content;
		if (!ft_strncmp(var->name, "_", 2))
			break ;
		tmp = tmp->next;
	}
	if (!var)
		return ;
	free(var->value);
	i = 0;
	while (com->args[i])
		i++;
	i--;
	var->value = ft_strdup(com->args[i]);
	if (!var->value)
		return (ft_execute_wait(shell), ft_error_malloc(shell));
}

void	ft_wait_parent(int div)
{
	int	i;

	i = 0;
	while (i < INT_MAX / div)
		i++;
}

/**
 * @brief	For skip the ./minishell execve if it's not the last pipe
 * 
 * @param	com Struct of our command
 */
void	ft_piped_mini(t_command *com, t_shell *shell)
{
	if (access(com->path, F_OK | X_OK))
	{
		ft_putstr_fd("minishell: ", 2);
		perror(com->args[0]);
		exit(EXIT_FAILURE);
	}
	ft_free_child(shell, EXIT_SUCCESS);
}
