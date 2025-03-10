/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:04:35 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/05 13:28:25 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_cd_update_pwd(t_shell *shell, bool *edit_promte);
static bool	ft_cd_find_pwd(t_shell *shell, char **tmp, bool *edit_promte);
static bool	ft_cd_edit_var(t_list **lst, char **tmp, bool is_pwd, \
	bool *edit_promte);
static bool	ft_cd_edit_oldpwd(t_list **lst, char **tmp);

/**
 * @brief	Use chdir for doing like cd command
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 * @return	0 all good, 1 have error
 */
int	ft_cd(t_shell *shell, t_command *com)
{
	bool	edit_prompte;
	char	*home;

	edit_prompte = true;
	if (ft_len(com->args) > 2)
		return (ft_putstr_fd("minishell: cd: Too many arguments\n", 2), 1);
	if (ft_len(com->args) < 2)
	{
		home = ft_cd_get_home(shell);
		if (!home || !home[0])
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
		if (chdir(home) < 0)
			return (ft_putstr_fd("minishell: cd: ", 2), perror(home), 1);
	}
	else if (chdir(com->args[1]) < 0)
		return (ft_putstr_fd("minishell: cd: ", 2), perror(com->args[1]), 1);
	if (!ft_cd_update_pwd(shell, &edit_prompte))
		return (ft_error_malloc(shell), 1);
	if (edit_prompte)
	{
		free(shell->prompte);
		shell->prompte = ft_init_prompt(" $> ");
	}
	ft_sup_here_doc(com);
	return (0);
}

/**
 * @brief	Check if "PWD" and "OLDPWD" is in global varriable for edit them
 * 
 * @param	shell Our big struct 
 * @return	true All good;
 * @return	false Error mall
 */
static bool	ft_cd_update_pwd(t_shell *shell, bool *edit_promte)
{
	t_list	*lst;
	t_var	*var;
	char	*tmp;

	tmp = NULL;
	if (!ft_cd_find_pwd(shell, &tmp, edit_promte))
		return (false);
	lst = shell->global_var;
	while (lst != NULL)
	{
		var = (t_var *)lst->content;
		if (!ft_strncmp(var->name, "OLDPWD", 7))
		{
			if (!ft_cd_edit_var(&lst, &tmp, false, edit_promte))
				return (false);
			break ;
		}
		lst = lst->next;
	}
	return (true);
}

/**
 * @brief	Try to find PWD variable for edit
 * 
 * @param	shell Our big struct 
 * @param	tmp Pointer a string to keep the value of PWD
 * @return	true All good;
 * @return	false Error mall
 */
static bool	ft_cd_find_pwd(t_shell *shell, char **tmp, bool *edit_promte)
{
	t_list	*lst;
	t_var	*var;

	lst = shell->global_var;
	while (lst != NULL)
	{
		var = (t_var *)lst->content;
		if (!ft_strncmp(var->name, "PWD", 4))
		{
			if (!ft_cd_edit_var(&lst, tmp, true, edit_promte))
				return (false);
			break ;
		}
		lst = lst->next;
	}
	return (true);
}

/**
 * @brief	Edit PWD or OLDPWD variable
 * 
 * @param	lst Pointer to the node of the list
 * @param	tmp Pointer a string to keep the value of PWD
 * @param	is_pwd true = edit PWD ; false = edit OLDPWD
 * @return	true All good;
 * @return	false Error malloc
 */
static bool	ft_cd_edit_var(t_list **lst, char **tmp, bool is_pwd, \
	bool *edit_promte)
{
	t_var	*var;

	var = (t_var *)(*lst)->content;
	if (is_pwd)
	{
		*tmp = var->value;
		var->value = getcwd(NULL, 0);
		if (!var->value)
		{
			*edit_promte = false;
			if (errno == ENOENT)
				return (ft_putstr_fd(ERR_CD, 2), true);
			return (perror("minishell: cd"), false);
		}
	}
	else
		return (ft_cd_edit_oldpwd(lst, tmp));
	return (true);
}

/**
 * @brief	Edit OLDPWD variable
 * 
 * @param	lst Pointer to the node of the list
 * @param	tmp Pointer a string to keep the value of PWD
 * @return	true All good;
 * @return	false Error malloc
 */
static bool	ft_cd_edit_oldpwd(t_list **lst, char **tmp)
{
	t_var	*var;

	var = (t_var *)(*lst)->content;
	if (var->value)
		free(var->value);
	if (*tmp == NULL)
		var->value = ft_strdup("");
	else
		var->value = *tmp;
	if (!var->value)
		return (perror("minishell"), false);
	return (true);
}
