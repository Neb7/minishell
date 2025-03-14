/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pars_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:38:28 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/14 14:41:09 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Parse the command for execute the good function
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
void	ft_pars_exec(t_shell *shell, t_command *com)
{
	if (!com->args || shell->nb_command > 1)
	{
		ft_command(shell, com);
		return ;
	}
	if (com->path == NULL && !ft_strncmp(com->args[0], "cd", 3))
		ft_cd(shell, com, false);
	else if (!com->path && !ft_strncmp(com->args[0], "export", 7) && \
	com->args[1])
		ft_export(shell, com, false);
	else if (com->path == NULL && !ft_strncmp(com->args[0], "unset", 6))
		ft_unset(shell, com, false);
	else if (shell->nb_command == 1 && !ft_strncmp(com->args[0], "exit", 5))
		ft_exit_spe(shell, com);
	else
		ft_command(shell, com);
}

/**
 * @brief	Create a new processe and parse the command to execute the good
 * 			function and free and delete file we don't use anymore
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
void	ft_command(t_shell *shell, t_command *com)
{
	shell->pid[shell->idx_pid] = fork();
	if (shell->pid[shell->idx_pid] < 0)
		return ;
	else if (shell->pid[shell->idx_pid] == 0)
	{
		if (com->path == NULL && !ft_strncmp(com->args[0], "cd", 3))
			ft_cd(shell, com, true);
		else if (!com->path && !ft_strncmp(com->args[0], "export", 7) && \
		com->args[1])
			ft_export(shell, com, true);
		else if (com->path == NULL && !ft_strncmp(com->args[0], "unset", 6))
			ft_unset(shell, com, true);
		ft_child(shell, com);
	}
}

/**
 * @brief	Handle in the child process
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
void	ft_child(t_shell *shell, t_command *com)
{
	ft_init_sig(SIGPIPE, ft_ges);
	ft_check_args_null(shell, com);
	if (!ft_redir_check(com, shell))
		exit(1);
	if (com->path == NULL && !ft_strncmp(com->args[0], "echo", 5))
		ft_echo(com, shell);
	else if (com->path == NULL && !ft_strncmp(com->args[0], "pwd", 4))
		ft_pwd(shell);
	else if (com->path == NULL && !ft_strncmp(com->args[0], "env", 4))
		ft_env(shell);
	else if (com->path == NULL && !ft_strncmp(com->args[0], "export", 7))
		ft_export(shell, com, true);
	else if (ft_strlen(com->args[0]) >= 10 && !com->args[1] && \
	!ft_strncmp(com->path + ft_strlen(com->args[0]) - 10, "/minishell", 11))
		ft_piped_mini(com, shell);
	else if (execve(com->path, com->args, shell->envp) < 0)
	{
		ft_error_execve(shell, com);
	}
	ft_free_child(shell, EXIT_SUCCESS);
}

/**
 * @brief	Browse the output_file list and the input_file list
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 * @return	true 
 * @return	false 
 */
bool	ft_redir_check(t_command *com, t_shell *shell)
{
	bool	ret;

	ret = true;
	if (com->input_file != NULL)
		ft_input_file(com, &ret);
	else
	{
		if (shell->nb_com > 0)
			dup2(shell->pipe[shell->nb_com - 1][0], STDIN_FILENO);
	}
	if (com->output_file != NULL)
		ft_output_file(com, &ret);
	else
		dup2(shell->pipe[shell->nb_com][1], STDOUT_FILENO);
	ft_close_pipe(shell);
	return (ret);
}

/**
 * @brief	Close all pipe in shell->pipe
 * 
 * @param	shell Our big struct
 */
void	ft_close_pipe(t_shell *shell)
{
	size_t	i;

	i = 0;
	while (i < shell->nb_command)
	{
		if (shell->pipe[i][0] != 0)
			close(shell->pipe[i][0]);
		if (shell->pipe[i][1] != 0)
			close(shell->pipe[i][1]);
		i++;
	}
}
