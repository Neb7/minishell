/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_no_arg.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:35:43 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/09 16:45:13 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_find_lower(char **actual, char **last, t_list *lst);
static void	ft_print_lower(char **actual, t_list *lst);
static void	ft_find_first_lower(char **actual, t_list *lst);

/**
 * @brief	Browse 'lst' to print alphabetically all variable
 * 
 * @param	lst Global variable's list
 * @param	nb_var Lenght of the global variable's list
 * @param	shell Our big struct
 */
void	ft_export_no_arg(t_list *lst, int nb_var, t_shell *shell)
{
	char	*last;
	char	*actual;
	int		count;

	count = 0;
	actual = NULL;
	ft_find_first_lower(&actual, lst);
	ft_print_lower(&actual, lst);
	while (count < nb_var)
	{
		last = actual;
		ft_find_lower(&actual, &last, lst);
		ft_print_lower(&actual, lst);
		count++;
	}
	ft_free_child(shell, EXIT_SUCCESS);
}

/**
 * @brief	Find the first variable to print (alphabetically)
 * 
 * @param	actual Pointer to the string of the name of the actual variable to
 * 				   print
 * @param	lst Global variable's list
 */
static void	ft_find_first_lower(char **actual, t_list *lst)
{
	t_list	*tmp;
	t_var	*var;

	tmp = lst;
	if (lst == NULL)
		exit(EXIT_FAILURE);
	var = (t_var *)tmp->content;
	if (*actual == NULL)
		*actual = var->name;
	tmp = lst;
	while (tmp != NULL)
	{
		var = (t_var *)tmp->content;
		if (ft_strcmp(*actual, var->name) > 0)
			*actual = var->name;
		tmp = tmp->next;
	}
}

/**
 * @brief	Find the next variable to print (alphabetically)
 * 
 * @param	actual Pointer to the string of the name of the actual variable to
 * 				   print
 * @param	last Name of the last printed variable
 * @param	lst Global variable's list
 */
static void	ft_find_lower(char **actual, char **last, t_list *lst)
{
	t_list	*tmp;
	t_var	*var;

	tmp = lst;
	if (lst == NULL)
		exit(EXIT_FAILURE);
	tmp = lst;
	while (tmp != NULL)
	{
		var = (t_var *)tmp->content;
		if (ft_strcmp(*last, var->name) < 0)
		{
			*actual = var->name;
			break ;
		}
		tmp = tmp->next;
	}
	while (tmp != NULL)
	{
		var = (t_var *)tmp->content;
		if (!ft_strcmp(*last, *actual) || (ft_strcmp(*actual, var->name) > 0 \
		&& ft_strcmp(*last, var->name) < 0))
			*actual = var->name;
		tmp = tmp->next;
	}
}

/**
 * @brief	Print the actual variable and her value (but not the variable '_')
 * 
 * @param	actual Pointer to the string of the name of the actual variable to
 * 				   print
 * @param	lst Global variable's list
 */
static void	ft_print_lower(char **actual, t_list *lst)
{
	t_list	*tmp;
	t_var	*var;

	if (!ft_strcmp("_", *actual))
		return ;
	tmp = lst;
	while (tmp != NULL)
	{
		var = (t_var *)tmp->content;
		if (!ft_strcmp(*actual, var->name))
		{
			printf("declare -x %s", var->name);
			if (var->value != NULL)
				printf("=\"%s\"\n", var->value);
			else
				printf("\n");
			break ;
		}
		tmp = tmp->next;
	}
}

/**
 * @brief	Check if 'arg' is valid
 * 
 * @param	arg Actuall argument
 * @return	0 all good, 1 not good 
 */
int	ft_is_valid_arg(char *arg, ssize_t j)
{
	ssize_t	i;

	i = 0;
	if (j < 0)
		j = ft_strlen (arg);
	if (!(arg[0] == '_' || ft_isalpha(arg[0])))
		return (0);
	while (++i < j)
	{
		if (!(arg[i] == '_' || ft_isalnum(arg[i])))
			return (0);
	}
	return (1);
}
