/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:40:29 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/06 15:35:49 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Create a new here_doc file with an id
 * 
 * @param	file_name 
 * @return	true 
 * @return	false 
 */
static bool	create_here_doc_file(char **file_name)
{
	static size_t	here_doc_id = 0;
	char			*tmp;

	(void)here_doc_id;
	tmp = ft_itoa(here_doc_id++);
	if (!tmp)
		return (perror("minishell"), free(*file_name), false);
	*file_name = ft_strjoin(HERE_DOC_PREFIX, tmp);
	free(tmp);
	if (!*file_name)
		return (perror("minishell"), false);
	return (true);
}

/**
 * @brief	Free and set the file name of the here_doc
 * 
 * @param	file The file structure of the here_doc
 * @param	file_name The new file name
 */
static bool	free_set_file_name(t_shell *shell, t_file *file, \
		char *file_name)
{
	(void)shell;
	free(file->name);
	file->name = file_name;
	return (false);
}

/*
	mb ctrl + c return 130
*/

/**
 * @brief	Create and execute a here_doc
 * 
 * @param	shell The shell structure
 * @param	file The file structure of the here_doc
 * @return	true if the here_doc has been successfully executed,
 * 			false otherwise.
 */
bool	here_doc(t_shell *shell, t_file *file)
{
	char	*file_name;

	file_name = NULL;
	if (g_signal[2] == 1)
		return (true);
	if (!create_here_doc_file(&file_name))
		return (free_set_file_name(shell, file, file_name));
	shell->file_name_temp = file_name;
	if (!expand_quote(&file->name))
		return (free_set_file_name(shell, file, file_name));
	file->fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file->fd == -1)
		return (free_set_file_name(shell, file, file_name));
	if (!exec_here_doc(shell, file))
		return (free_set_file_name(shell, file, file_name));
	free(file->name);
	file->name = file_name;
	return (true);
}
