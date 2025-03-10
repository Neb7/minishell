/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:27:07 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/05 15:42:21 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Print the actual variable if her value isn't NULL
 * 
 * @param	var Struct for one variable (name, value)
 */
static void	ft_print_var(t_var *var)
{
	if (!var || !var->name)
		return ;
	if (!ft_strncmp(var->name, "_", 2))
		printf(SPE_VAR);
	else if (var->value != NULL)
		printf("%s=%s\n", var->name, var->value);
}

/**
 * @brief	Browse the global variable's list for print them
 * 
 * @param	shell Our big struct
 */
void	ft_env(t_shell *shell)
{
	t_list	*lst;

	lst = shell->global_var;
	while (lst != NULL)
	{
		ft_print_var(lst->content);
		lst = lst->next;
	}
	ft_free_child(shell, EXIT_SUCCESS);
}
