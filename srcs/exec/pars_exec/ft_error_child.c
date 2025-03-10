/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 14:37:16 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/09 18:13:25 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_child(int sig, siginfo_t *info, void *context)
{
	t_shell	*shell;

	shell = ft_acces_shell(NULL);
	if (sig == SIGPIPE)
		ft_free_child(shell, EXIT_SUCCESS);
	(void)info;
	(void)context;
}

static void	ft_error_child_last(t_command *com, int *ret)
{
	if (errno != ENOEXEC)
	{
		if (errno == EACCES)
			*ret = 126;
		write(STDERR_FILENO, "minishell: ", 11);
		perror(com->args[0]);
	}
}

static void	ft_error_execve_utils(t_command *com, int *ret)
{
	write(STDERR_FILENO, "minishell: ", 11);
	ft_putstr_fd(com->args[0], 2);
	ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	*ret = 126;
}

/**
 * @brief	Write an error when exeve fail
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
void	ft_error_execve(t_shell *shell, t_command *com)
{
	int			ret;
	struct stat	filestat;

	if (!com->args[0] || !com->args[0][0])
		ft_free_child(shell, 0);
	ret = 127;
	if (ft_strchar(com->args[0], '/') < 0)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		ft_putstr_fd(com->args[0], 2);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else
	{
		if (stat(com->args[0], &filestat) != 0)
			return (perror("minishell"), ft_free_child(shell, ret));
		if (S_ISDIR(filestat.st_mode))
			ft_error_execve_utils(com, &ret);
		else
			ft_error_child_last(com, &ret);
	}
	ft_free_child(shell, ret);
}
