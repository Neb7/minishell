/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pars_exec_last.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:42:33 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/14 14:54:19 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_bultin_cheld_last(t_command *com, t_shell *shell)
{
	if (com->path == NULL && !ft_strncmp(com->args[0], "cd", 3))
		ft_cd(shell, com, true);
	else if (!com->path && !ft_strncmp(com->args[0], "export", 7) && \
	com->args[1])
		ft_export(shell, com, true);
	else if (com->path == NULL && !ft_strncmp(com->args[0], "unset", 6))
		ft_unset(shell, com, true);
}

/**
 * @brief	Parse the command for execute the good function (for the last
 * 			command)
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
void	ft_pars_exec_last(t_shell *shell, t_command *com)
{
	if (!com->args)
	{
		ft_command_last(shell, com);
		return ;
	}
	if (com->path == NULL && !ft_strncmp(com->args[0], "cd", 3))
		shell->last_exit_status = ft_cd(shell, com, false);
	else if (!com->path && !ft_strncmp(com->args[0], "export", 7) && \
	com->args[1])
		shell->last_exit_status = ft_export(shell, com, false);
	else if (com->path == NULL && !ft_strncmp(com->args[0], "unset", 6))
		shell->last_exit_status = ft_unset(shell, com, false);
	else if (shell->nb_command == 1 && !ft_strncmp(com->args[0], "exit", 5))
		shell->last_exit_status = ft_exit_spe(shell, com);
	else
		ft_command_last(shell, com);
}

/**
 * @brief	Create a new processe and parse the command to execute the good
 * 			function and free and delete file we don't use anymore (for the
 * 			last command)
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
void	ft_command_last(t_shell *shell, t_command *com)
{
	shell->pid[shell->idx_pid] = fork();
	if (shell->pid[shell->idx_pid] < 0)
		return (perror("minishell: fork"));
	else if (shell->pid[shell->idx_pid] == 0)
	{
		ft_bultin_cheld_last(com, shell);
		ft_child_last(shell, com);
	}
	else
	{
		if (com->args && com->args[0] && ft_strlen(com->args[0]) >= 10 && \
		!access(com->path, F_OK | X_OK))
		{
			if (!ft_strncmp(com->path + ft_strlen(com->args[0]) - 10, \
			"/minishell", 11) && !com->args[1])
			{
				g_signal[1] = 1;
				ft_wait_parent(32);
				if (kill(shell->pid[shell->idx_pid], SIGUSR2) != 0)
					perror("minishell: kill");
			}
		}
		ft_close_pipe(shell);
	}
}

/**
 * @brief	Handle in the child process
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
void	ft_child_last(t_shell *shell, t_command *com)
{
	ft_init_sig(SIGPIPE, ft_ges);
	ft_check_args_null(shell, com);
	if (!ft_redir_check_last(com, shell))
		ft_free_child(shell, EXIT_FAILURE);
	if (com->path == NULL && !ft_strncmp(com->args[0], "echo", 5))
		ft_echo(com, shell);
	else if (com->path == NULL && !ft_strncmp(com->args[0], "pwd", 4))
		ft_pwd(shell);
	else if (com->path == NULL && !ft_strncmp(com->args[0], "env", 4))
		ft_env(shell);
	else if (com->path == NULL && !ft_strncmp(com->args[0], "export", 7))
		ft_export(shell, com, true);
	else if (execve(com->path, com->args, shell->envp) < 0)
	{
		ft_error_execve(shell, com);
	}
	ft_free_child(shell, EXIT_SUCCESS);
}

/**
 * @brief	Browse the output_file list and the input_file list (for the
 * 			last command)
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 * @return	true 
 * @return	false 
 */
bool	ft_redir_check_last(t_command *com, t_shell *shell)
{
	bool	ret;

	ret = true;
	if (com->input_file != NULL)
		ft_input_file(com, &ret);
	else
	{
		if (com->args[0] && ft_strlen(com->args[0]) >= 10 && \
		!access(com->path, F_OK | X_OK))
		{
			if (!(!ft_strncmp(com->path + ft_strlen(com->args[0]) - 10, \
			"/minishell", 11) && !com->args[1]))
				if (shell->nb_com > 0)
					dup2(shell->pipe[shell->nb_com - 1][0], STDIN_FILENO);
		}
		else
			if (shell->nb_com > 0)
				dup2(shell->pipe[shell->nb_com - 1][0], STDIN_FILENO);
	}
	if (com->output_file != NULL)
		ft_output_file(com, &ret);
	ft_close_pipe(shell);
	return (ret);
}
