/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:13:04 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/13 13:59:41 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_unset_check_exist(char *name, t_list *lst, t_list *tmp[3]);
static bool	ft_check_name(char *name, t_var *var);

/**
 * @brief	Browse 'lst' to check if "PATH" will be delete and delete variable
 * 			in args of the globale variable's list (but not the '_' variable)
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 * @return	int 
 */
int	ft_unset(t_shell *shell, t_command *com, bool in_fork)
{
	t_list	*lst;
	t_list	*tmp[3];
	size_t	i;

	lst = shell->global_var;
	i = 0;
	while (com->args[++i] != NULL)
	{
		if (!ft_strncmp(com->args[i], "_", 2))
			continue ;
		else if (!ft_strncmp(com->args[i], "PATH", 5))
			shell->save_path = false;
		tmp[0] = lst;
		tmp[2] = NULL;
		ft_unset_check_exist(com->args[i], lst, tmp);
	}
	sup_here_doc(com);
	ft_lstclear(&com->input_file, free_file);
	ft_lstclear(&com->output_file, free_file);
	if (in_fork)
		ft_free_child(shell, EXIT_SUCCESS);
	return (0);
}

/**
 * @brief	Delete the 'name' variable of the global variable's list
 * 
 * @param	name Name of the variable
 * @param	lst Global variable's list
 * @param	tmp tmp[1] = Pointer of current node ([0] and [2] are for temporary
 * 				variable)
 */
static void	ft_unset_check_exist(char *name, t_list *lst, t_list *tmp[3])
{
	while (tmp[0] != NULL)
	{
		if (ft_check_name(name, tmp[0]->content))
		{
			tmp[1] = tmp[0]->next;
			if (tmp[2] != NULL)
				tmp[2]->next = tmp[1];
			else
				lst = tmp[1];
			ft_lstdelone(tmp[0], ft_del_var);
			break ;
		}
		tmp[2] = tmp[0];
		tmp[0] = tmp[0]->next;
	}
}

/**
 * @brief	Check if 'name' have same name of var
 * 
 * @param	name Name of the variable
 * @param	var Struct for one variable (name, value)
 * @return	true Smae name;
 * @return	false Not same name
 */
static bool	ft_check_name(char *name, t_var *var)
{
	if (!ft_strncmp(name, var->name, ft_strlen(name) + 1))
		return (true);
	return (false);
}

/**
 * @brief	Fonction to use in the settings of ft_lstdelone
 * 
 * @param	content Pointer of the content
 */
void	ft_del_var(void *content)
{
	t_var	*var;

	var = (t_var *)content;
	free(var->name);
	free(var->value);
	free(content);
}
