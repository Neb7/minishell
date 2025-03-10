/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:51:31 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/03 15:40:08 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Handle for SIGINT
 * 
 * @param	shell Our big struct
 * @return	true 
 * @return	false 
 */
bool	ft_sigint(t_shell *shell)
{
	g_signal[0] = 0;
	ft_putstr_fd("\n", 1);
	shell->last_exit_status = 130;
	return (true);
}

/**
 * @brief	Handle for SIGQUIT
 * 
 * @param	shell Our big struct
 * @return	true 
 * @return	false 
 */
bool	ft_sigquit(t_shell *shell)
{
	g_signal[0] = 0;
	printf("Quit\n");
	shell->last_exit_status = 131;
	return (true);
}

/**
 * @brief	Write a "Segmantation fault" message error
 * 
 * @return	true 
 * @return	false 
 */
bool	ft_sigsegv(void)
{
	ft_putstr_fd("Segmentation fault\n", 2);
	return (true);
}

/**
 * @brief	Loop if we execute minishell in minishell
 * 
 * @param	shell Our big struct
 */
void	ft_loop_mini(t_shell *shell)
{
	while (g_signal[1])
		ft_strlen(shell->prompte);
	if (WIFEXITED(shell->status))
		if (shell->idx_pid == (int)shell->nb_command)
			if (shell->pid[shell->idx_pid - 1] != 0)
				ft_last_exit_status(shell);
}

/**
 * @brief	Edit the last exit status for the $?
 * 
 * @param	shell Our big struct
 */
void	ft_last_exit_status(t_shell *shell)
{
	if (!g_signal[3])
		shell->last_exit_status = WEXITSTATUS(shell->status);
}
