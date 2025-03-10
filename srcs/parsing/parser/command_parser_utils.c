/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:53:55 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/09 18:05:04 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Close all files of the list.
 * 
 * @param	file_list The list of files to close.
 */
static void	close_files(t_list *file_list)
{
	t_list	*tmp;

	if (!file_list)
		return ;
	tmp = file_list;
	while (tmp)
	{
		if (((t_file *)(tmp->content))->fd > 2)
			close(((t_file *)(tmp->content))->fd);
		tmp = tmp->next;
	}
}

/**
 * @brief	Open only here_doc files of the command.
 * 
 * @param	shell The shell structure.
 * @param	command A pointer to the command structure.
 * @return	true if all here_doc files were opened successfully, 
 * 			false otherwise.
 */
static bool	open_here_doc(t_shell *shell, t_command *command)
{
	t_list	*tmp;
	t_file	*file;

	tmp = command->input_file;
	while (tmp)
	{
		file = (t_file *)(tmp->content);
		if (file->append)
		{
			if (!file->name)
			{
				ft_putendl_fd(REDIRECTION_AT_END, STDERR_FILENO);
				return (close_files(command->input_file), true);
			}
			if (!here_doc(shell, file))
				return (close_files(command->input_file), false);
		}
		tmp = tmp->next;
	}
	return (true);
}

/**
 * @brief	Open all input files of the command.
 * 
 * The function open all input files of the command, but not the here_doc files.
 * 
 * @param	command A pointer to the command structure.
 * @return	true if all input files were opened successfully, false otherwise.
 */
static bool	open_input(t_command *command)
{
	t_list	*tmp;
	t_file	*file;

	tmp = command->input_file;
	while (tmp)
	{
		file = (t_file *)(tmp->content);
		if (!file->append)
		{
			if (!file->name)
			{
				ft_putendl_fd(REDIRECTION_AT_END, STDERR_FILENO);
				return (close_files(command->input_file), true);
			}
			file->fd = open(file->name, O_RDONLY, 0644);
		}
		tmp = tmp->next;
	}
	return (true);
}

/**
 * @brief	Open all output files of the command.
 * 
 * @param	command A pointer to the command structure.
 * @return	true if all output files were opened successfully, false otherwise.
 */
bool	open_output_files(t_command *command)
{
	t_list	*tmp;
	t_file	*file;
	int		o_flag;

	tmp = command->output_file;
	while (tmp)
	{
		file = (t_file *)(tmp->content);
		if (!file->name)
		{
			ft_putendl_fd(REDIRECTION_AT_END, STDERR_FILENO);
			return (close_files(command->output_file), true);
		}
		if (file->append)
			o_flag = O_CREAT | O_WRONLY | O_APPEND;
		else
			o_flag = O_CREAT | O_WRONLY | O_TRUNC;
		file->fd = open(file->name, o_flag, 0644);
		tmp = tmp->next;
	}
	return (true);
}

/**
 * @brief	Open all files of the command.
 * 
 * Initially the function open all here_doc files, 
 * then all input files and finally all output files.
 * 
 * @param	shell The shell structure.
 * @param	command A pointer to the command structure.
 * @return	true if all files were opened successfully, false otherwise.
 */
bool	open_files(t_shell *shell, t_command *command)
{
	ft_init_sig(SIGQUIT, ft_handle_here_doc);
	ft_init_sig(SIGINT, ft_handle_here_doc);
	if (!open_here_doc(shell, command))
		return (false);
	if (!open_input(command))
		return (false);
	if (!open_output_files(command))
		return (false);
	return (true);
}
	// if (file->fd == -1)
		// {
		// 	shell->last_exit_status = 1;
		// 	return (ft_putstr_fd("minishell: ", STDERR_FILENO), \
		// 		perror(file->name), close_files(command->input_file), false);
		// }