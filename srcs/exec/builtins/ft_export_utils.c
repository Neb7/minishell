/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:03:11 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/05 13:28:39 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Edit 'arg' variable to the global variable's list
 * 
 * @param	lst Global variable's list
 * @param	arg Actuall argument
 * @param	j Index of the first '=' character of 'arg'
 * @return	true All good;
 * @return	false Error malloc
 */
bool	ft_export_edit_var(t_list *lst, char *arg, int j)
{
	t_list	*tmp;
	t_var	*tmp_var;

	tmp = lst;
	while (tmp != NULL)
	{
		tmp_var = (t_var *)tmp->content;
		if (!ft_strncmp(arg, tmp_var->name, ft_strlen(tmp_var->name)))
			break ;
		tmp = tmp->next;
	}
	tmp_var = (t_var *)tmp->content;
	free(tmp_var->value);
	tmp_var->value = ft_strdup(arg + j + 1);
	if (!tmp_var->value)
		return (perror("minishell: malloc"), false);
	return (true);
}

/**
 * @brief	Add 'arg' variable to the global variable's list
 * 
 * @param	lst Global variable's list
 * @param	arg Actuall argument
 * @param	j Index of the first '=' character of 'arg'
 * @return	true All good;
 * @return	false Error malloc
 */
bool	ft_export_add_new(t_list *lst, char *arg, int j)
{
	t_var	*new_var;
	t_list	*new;

	new_var = (t_var *)malloc(sizeof(t_var));
	if (! new_var)
		return (perror("minishell: export"), false);
	new_var->name = (char *)ft_calloc(j + 1, sizeof(char));
	if (! new_var->name)
		return (free(new_var), perror("minishell: export"), false);
	ft_memcpy(new_var->name, arg, j);
	new_var->value = ft_strdup(arg + j + 1);
	if (!new_var->value)
		return (free(new_var->name), free(new_var), \
		perror("minishell: export"), false);
	new = ft_lstnew(new_var);
	if (!new)
		return (free(new_var->value), free(new_var->name), \
		free(new_var), perror("minishell: export"), false);
	ft_lstadd_back(&lst, new);
	return (true);
}

/**
 * @brief	If the 'arg' variable doesn't exit, add it to the global variable's
 * 			list and check if 'arg' is the "PATH" variable
 * 
 * @param	lst Global variable's list
 * @param	arg Actuall argument
 * @param	j Index of the first '=' character of 'arg'
 * @param	shell Our big struct
 * @return	true All good;
 * @return	false Error malloc
 */
bool	ft_export_add_var_null(t_list *lst, char *arg, int j, t_shell *shell)
{
	t_var	*new_var;
	t_list	*new;

	if (!ft_is_exist_already(arg, lst, (int)ft_strlen(arg)))
	{
		if (!ft_strncmp(arg, "PATH", 5))
			shell->save_path = false;
		new_var = (t_var *)malloc(sizeof(t_var));
		if (! new_var)
			return (perror("minishell: export"), false);
		new_var->name = (char *)malloc(sizeof(char) * j + 1);
		if (! new_var->name)
			return (free(new_var), perror("minishell: export"), false);
		ft_memcpy(new_var->name, arg, j + 1);
		new_var->value = NULL;
		new = ft_lstnew(new_var);
		if (!new)
			return (free(new_var->value), free(new_var->name), \
			free(new_var), perror("minishell: export"), false);
		ft_lstadd_back(&lst, new);
	}
	return (true);
}

/**
 * @brief	Check the return of ft_export_edit_var() or ft_export_add_new()
 * 
 * @param	arg Actuall argument
 * @param	shell Our big struct
 * @param	j Index of the first '=' character of 'arg'
 * @param	lst Global variable's list
 */
void	ft_check_return(char *arg, t_shell *shell, ssize_t j, t_list *lst)
{
	if (ft_is_exist_already(arg, lst, j))
	{
		if (!ft_export_edit_var(lst, arg, j))
			return (ft_error_malloc(shell));
	}
	else
	{
		if (!ft_export_add_new(lst, arg, j))
			return (ft_error_malloc(shell));
	}
}
