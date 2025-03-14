/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:34:43 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/13 14:06:25 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_export_arg(t_list *lst, t_command *com, t_shell *shell, \
	int *ret);
static int	ft_nbvar(t_list *lst);
static void	ft_export_wrong_arg(char *arg, int *ret);

/**
 * @brief	Check if it's with or without arguments
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 * @param	in_fork True if we're in a child process
 * @return	int 
 */
int	ft_export(t_shell *shell, t_command *com, bool in_fork)
{
	int	ret;

	ret = 0;
	if (com->args[1] == NULL)
		ft_export_no_arg(shell->global_var, ft_nbvar(shell->global_var), shell);
	else
		ft_export_arg(shell->global_var, com, shell, &ret);
	if (in_fork)
		ft_free_child(shell, EXIT_SUCCESS);
	return (ret);
}

/**
 * @brief	Parse the argument and check if there is a change of "PATH"
 * 
 * @param	lst Global variable's list
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
static void	ft_export_arg(t_list *lst, t_command *com, t_shell *shell, int *ret)
{
	size_t	i;
	ssize_t	j;

	i = 1;
	while (com->args[i] != NULL)
	{
		j = ft_strchar(com->args[i], '=');
		if (j == 0 || !ft_is_valid_arg(com->args[i], j))
			ft_export_wrong_arg(com->args[i], ret);
		else if (j > 0)
		{
			if (!ft_strncmp(com->args[i], "PATH", j))
				shell->save_path = false;
			ft_check_return(com->args[i], shell, j, lst);
		}
		else
		{
			if (!ft_export_add_var_null(lst, com->args[i], \
				ft_strlen(com->args[i]), shell))
				return (ft_error_malloc(shell));
		}
		i++;
	}
}

/**
 * @brief	Check if the variable in arg is already in the global variable
 * 
 * @param	arg Actuall argument
 * @param	lst Global variable's list
 * @param	j Index of the first '=' character of 'arg'
 * @return	true 'arg' is in the list;
 * @return	false 'arg' isn't in the list
 */
bool	ft_is_exist_already(char *arg, t_list *lst, int j)
{
	bool	is_here;
	t_list	*tmp;
	t_var	*var;

	is_here = false;
	tmp = lst;
	while (tmp != NULL && !is_here)
	{
		var = (t_var *)tmp->content;
		if (!ft_strncmp(arg, var->name, j))
		{
			if (var->name[j] == 0)
				is_here = true;
		}
		tmp = tmp->next;
	}
	return (is_here);
}

/**
 * @brief	Count the len of the global variable's list
 * 
 * @param	lst Global variable's list
 * @return	Lenght of the globale variable's list
 */
static int	ft_nbvar(t_list *lst)
{
	int		count;
	t_list	*tmp;

	count = 0;
	tmp = lst;
	while (tmp != NULL)
	{
		count++;
		tmp = tmp->next;
	}
	if (count > 0)
		count--;
	return (count);
}

/**
 * @brief	Write an error message
 * 
 * @param	arg Actuall argument
 */
static void	ft_export_wrong_arg(char *arg, int *ret)
{
	ft_putstr_fd("minishell: export: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	*ret = 1;
}
