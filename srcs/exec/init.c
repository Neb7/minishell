/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:07:38 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/03 15:42:32 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Add a variable 'name' with a NULL value
 * 
 * @param	name Name of the variable
 * @param	start Start of the list
 * @return	0 : All good ; 1 : Eroor malloc
 */
static int	ft_add_hidden_var(char *name, char *value, t_list **start)
{
	t_list	*new;
	t_var	*var;

	var = (t_var *)malloc(sizeof(t_var));
	if (!var)
		return (perror("minishell: malloc"), 1);
	var->name = ft_strdup(name);
	if (!var->name)
		return (perror("minishell: malloc"), free(var), 1);
	if (!value)
		var->value = NULL;
	else
	{
		var->value = ft_strdup(value);
		if (!var->value)
			return (perror("minishell: malloc"), free(var->value), \
			free(var->name), free(var), 1);
	}
	new = ft_lstnew(var);
	if (!new)
		return (perror("minishell: malloc"), free(var->name), free(var), 1);
	ft_lstadd_back(start, new);
	return (0);
}

/**
 * @brief	Increment SHLVL by 1
 * 
 * @param	var Actual variable
 * @return	true : All good ;
 * @return	false : Error malloc
 */
static bool	ft_add_one_lvl(t_var *var)
{
	int	nb_lvl;

	nb_lvl = ft_atoi(var->value);
	if (nb_lvl > 998)
	{
		ft_putstr_fd("minishell: warnimg: shell level (", 2);
		ft_putnbr_fd(nb_lvl + 1, 2);
		ft_putstr_fd(") too high, resetting to 1", 2);
		nb_lvl = 0;
	}
	nb_lvl++;
	free(var->value);
	var->value = ft_itoa(nb_lvl);
	if (!var->value)
		return (perror("minishell: malloc"), false);
	return (true);
}

/**
 * @brief	Add a new var in the list
 * 
 * @param	variable Actual variable
 * @param	start Start of the list
 * @param	j index of the first '='
 * @return	true : All good ;
 * @return	false : Error malloc
 */
static bool	ft_add_global_var(char **variable, t_list **start, ssize_t j)
{
	t_list	*new;
	t_var	*var;

	var = (t_var *)malloc(sizeof(t_var));
	if (!var)
		return (perror("minishell: malloc"), false);
	var->name = ft_substr(*variable, 0, j);
	if (!var->name)
		return (perror("minishell: malloc"), free(var), false);
	var->value = ft_strdup(*variable + j + 1);
	if (!var->value)
		return (perror("minishell: malloc"), free(var->name), free(var), false);
	if (!ft_strcmp(var->name, "SHLVL"))
		if (!ft_add_one_lvl(var))
			return (perror("minishell: malloc"), free(var->name), \
			free(var->value), free(var), false);
	new = ft_lstnew(var);
	if (!new)
		return (perror("minishell: malloc"), free(var->name), \
		free(var->value), free(var), false);
	ft_lstadd_back(start, new);
	return (true);
}

static bool	ft_add_four_var(t_list **start)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!ft_is_exist_already("OLDPWD", *start, 6))
	{
		if (ft_add_hidden_var("OLDPWD", NULL, start))
			return (ft_lstclear(start, ft_del_var), free(pwd), false);
	}
	if (!ft_is_exist_already("_", *start, 1))
	{
		if (ft_add_hidden_var("_", "./minishell", start))
			return (ft_lstclear(start, ft_del_var), free(pwd), false);
	}
	if (!ft_is_exist_already("SHLVL", *start, 5))
	{
		if (ft_add_hidden_var("SHLVL", "1", start))
			return (ft_lstclear(start, ft_del_var), free(pwd), false);
	}
	if (!ft_is_exist_already("PWD", *start, 3))
	{
		if (ft_add_hidden_var("PWD", pwd, start))
			return (ft_lstclear(start, ft_del_var), free(pwd), false);
	}
	return (free(pwd), true);
}

/**
 * @brief	Create a list for global variable
 * 
 * @param	envp Environment variables
 * @param	j intitial at 0
 * @param	start initial at NULL
 * @param	shell Our big struct
 * @return	Pointer of the list or NULL if error
 */
t_list	*ft_init_global(char **envp, ssize_t j, t_list *start, t_shell *shell)
{
	bool	have_variable;
	ssize_t	i;

	i = -1;
	have_variable = false;
	while (envp[++i] != NULL)
	{
		j = ft_strchar(envp[i], '=');
		if (j > 0)
		{
			if (!ft_strncmp("OLDPWD", envp[i], j) && envp[i][j + 1] == '=')
				have_variable = true;
			if (!ft_add_global_var(&envp[i], &start, j))
				return (ft_lstclear(&start, ft_del_var), NULL);
		}
	}
	if (!ft_add_four_var(&start))
		return (ft_lstclear(&start, ft_del_var), NULL);
	shell->save_path = true;
	return (start);
}
