/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sup_here_doc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:40:29 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/06 15:48:44 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Delete the here_doc file
 * 
 * @param	command A pointer to the command containing here_doc
 * @return	true if the here_doc file has been successfully deleted,
 * 			false otherwise.
 */
bool	sup_here_doc(t_command *command)
{
	t_list	*list;
	t_file	*file;

	list = command->input_file;
	while (list)
	{
		file = (t_file *)(list->content);
		if (file->append)
		{
			if (file->fd > 2)
				close(file->fd);
			if (file->name && unlink(file->name) == -1)
				return (false);
		}
		list = list->next;
	}
	return (true);
}
