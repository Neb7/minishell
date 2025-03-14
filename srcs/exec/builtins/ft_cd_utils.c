/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:52:51 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/13 14:03:36 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Seach the value of HOME variable
 * 
 * @param	shell Our big struct
 * @return	char* 
 */
char	*ft_cd_get_home(t_shell *shell)
{
	t_list	*lst;
	t_var	*var;

	lst = shell->global_var;
	while (lst != NULL)
	{
		var = (t_var *)lst->content;
		if (!ft_strncmp(var->name, "HOME", 5))
			return (var->value);
		lst = lst->next;
	}
	return (NULL);
}

/**
 * @brief	Count lenght of an array of string
 * 
 * @param	array_string 
 * @return	int lenght of the array
 */
int	ft_len(char **array_string)
{
	int	i;

	i = 0;
	while (array_string[i] != NULL)
		i++;
	return (i);
}

void	ft_sup_here_doc(t_command *com)
{
	sup_here_doc(com);
	ft_lstclear(&com->input_file, free_file);
	ft_lstclear(&com->output_file, free_file);
}

void	ft_cd_init_prompte(t_shell *shell)
{
	free(shell->prompte);
	shell->prompte = ft_init_prompt(" $> ");
	if (!shell->prompte)
	{
		ft_error_malloc(shell);
	}
}
