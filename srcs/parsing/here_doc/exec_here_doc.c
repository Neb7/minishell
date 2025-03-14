/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_here_doc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:38:26 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/10 13:57:20 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	exit_free(t_shell *shell, char **value)
{
	free(*value);
	*value = NULL;
	free(shell->file_name_temp);
	shell->file_name_temp = NULL;
	free_command2(&shell->c_command);
	shell->c_command = NULL;
	free(shell->trim_command);
	shell->trim_command = NULL;
	ft_free_child(shell, 0);
}

static char	*get_value(t_shell *shell, t_file *file)
{
	char	*value;

	value = readline("> ");
	if (!value)
	{
		ft_putstr_fd(WARNING_HERE_DOC_EOF, 2);
		return (NULL);
	}
	if (!ft_strncmp(value, file->name, ft_strlen(file->name)))
		exit_free(shell, &value);
	return (value);
}

static bool	wait_child_here_doc(int pid)
{
	waitpid(pid, NULL, 0);
	while (g_signal[2] == 2)
	{
		g_signal[2] = 0;
		waitpid(pid, NULL, 0);
	}
	ft_wait_parent(1024);
	if (g_signal[1] == 123)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (false);
	}
	return (true);
}

/**
 * @brief	Executes the here_doc
 * 
 * @param	shell The shell structure
 * @param	file The file structure of the here_doc
 * @return	true if the here_doc has been successfully executed, 
 * 			false otherwise.
 */
bool	exec_here_doc(t_shell *shell, t_file *file)
{
	char	*value;
	int		pid;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), false);
	else if (pid == 0)
	{
		ft_init_sig(SIGQUIT, ft_handle_here_doc_child);
		ft_init_sig(SIGINT, ft_handle_here_doc_child);
		while (1)
		{
			value = get_value(shell, file);
			if (!value)
				break ;
			if (file->expand && !expand_vars(*shell, &value, false))
				return (exit_free(shell, &value), false);
			ft_putendl_fd(value, file->fd);
			free(value);
		}
		exit_free(shell, &value);
	}
	else
		return (wait_child_here_doc(pid));
	return (true);
}
