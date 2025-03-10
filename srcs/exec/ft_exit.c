/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:02:35 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/09 17:43:17 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Check if the first arg is digit
 * 
 * @param	com Struct of our command
 * @return	true : It's digit;
 * @return	false : It's not
 */
static bool	ft_check_exit_arg(t_command *com)
{
	size_t	i;

	i = 1;
	if (!ft_isdigit(com->args[1][0]) && com->args[1][0] != '+' \
		&& com->args[1][0] != '-')
		return (false);
	while (com->args[1][i] != '\0')
	{
		if (!ft_isdigit(com->args[1][i]))
			return (false);
		i++;
	}
	if (i == 0)
		return (false);
	return (true);
}

/**
 * @brief	Write an error message, send a signal to his parent if needed and
 * 			exit 2
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
static void	ft_exit_no_num(t_shell *shell, t_command *com)
{
	ft_putstr_fd("exit\nminishell: exit: ", 2);
	ft_putstr_fd(com->args[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	if (shell->pid_parent)
	{
		if (kill(shell->pid_parent, SIGUSR1))
			perror("minishell: kill");
	}
	ft_free_shell(shell);
	exit(2);
}

/**
 * @brief	Exit the processe and send a signal to his parent if needed
 * 
 * @param	shell Our big struct
 * @param	status 
 */
void	ft_exit(t_shell *shell, int status)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (shell->pid_parent)
	{
		if (kill(shell->pid_parent, SIGUSR1))
			perror("minishell: kill");
	}
	ft_free_shell(shell);
	exit(status);
}

/**
 * @brief	Checks the errors possible for exit and execute the good one if
 * 			needed
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
int	ft_exit_spe(t_shell *shell, t_command *com)
{
	ft_close_pipe(shell);
	if (!com->args[1])
		ft_exit(shell, 0);
	if (!ft_check_exit_arg(com))
		ft_exit_no_num(shell, com);
	else
	{
		if (com->args[2] != NULL)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			return (ft_putstr_fd("bash: exit: too many arguments\n", 2), 1);
		}
		ft_exit(shell, ft_atoi(com->args[1]));
	}
	return (EXIT_FAILURE);
}
