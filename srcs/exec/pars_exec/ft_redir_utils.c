/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:45:53 by benpicar          #+#    #+#             */
/*   Updated: 2025/02/24 14:53:43 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Write an error message for an output file and edit ret to false
 * 
 * @param	file Struct of our file
 * @param	ret Pointer of a bool for edit his value if needed
 */
static void	ft_wrong_fd_output(t_file *file, bool *ret)
{
	if (*ret)
	{
		access(file->name, F_OK | R_OK | W_OK);
		ft_putstr_fd("minishell: ", 2);
		perror(file->name);
	}
	*ret = false;
}

/**
 * @brief	Write an error message for an input file and edit ret to false
 * 
 * @param	file Struct of our file
 * @param	ret Pointer of a bool for edit his value if needed
 */
static void	ft_wrong_fd_input(t_file *file, bool *ret)
{
	if (*ret)
	{
		if (file->append)
			ft_error(ERR_HERE, file->name);
		else
		{
			access(file->name, F_OK | R_OK | W_OK);
			ft_putstr_fd("minishell: ", 2);
			perror(file->name);
		}
	}
	*ret = false;
}

/**
 * @brief	Parse all list of input_file and dup the last one on STDIN if
 * 			there is no error
 * 
 * @param	com Struct of our command
 * @param	ret Pointer of a bool for edit hhhis value if needed
 */
void	ft_input_file(t_command *com, bool *ret)
{
	t_file	*file;
	t_list	*tmp;

	tmp = com->input_file;
	while (tmp->next != NULL)
	{
		file = (t_file *)tmp->content;
		if (file->fd < 0)
			ft_wrong_fd_input(file, ret);
		else
			close(file->fd);
		tmp = tmp->next;
	}
	file = (t_file *)tmp->content;
	if (file->fd < 0)
		ft_wrong_fd_input(file, ret);
	else
	{
		if (file->append)
			file->fd = open(file->name, O_RDONLY);
		dup2(file->fd, STDIN_FILENO);
		close(file->fd);
	}
}

/**
 * @brief	Parse all list of output_file and dup the last one on STDOUT if
 * 			there is no error
 * 
 * @param	com Struct of our command
 * @param	ret Pointer of a bool for edit hhhis value if needed
 */
void	ft_output_file(t_command *com, bool *ret)
{
	t_file	*file;
	t_list	*tmp;

	tmp = com->output_file;
	while (tmp->next != NULL)
	{
		file = (t_file *)tmp->content;
		if (file->fd < 0)
			ft_wrong_fd_output(file, ret);
		else
			close(file->fd);
		tmp = tmp->next;
	}
	file = (t_file *)tmp->content;
	if (file->fd < 0)
		ft_wrong_fd_output(file, ret);
	else
	{
		dup2(file->fd, STDOUT_FILENO);
		close(file->fd);
	}
}
