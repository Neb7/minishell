/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:56:58 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/03 21:39:53 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Create, with malloc, a string with the struct 'var'
 * 
 * @param	var Struct for one variable (name, value)
 * @return	char* 
 */
static char	*ft_var_to_char(t_var *var)
{
	char	*d;
	size_t	t_len;
	size_t	len_name;
	size_t	len_value;

	len_value = ft_strlen(var->value);
	len_name = ft_strlen(var->name);
	t_len = len_name + len_value + 2;
	d = (char *)malloc(sizeof(char) * t_len);
	if (!d)
		return (NULL);
	ft_memcpy(d, var->name, len_name);
	ft_memcpy(d + len_name, "=", 1);
	ft_memcpy(d + len_name + 1, var->value, len_value);
	d[t_len - 1] = 0;
	return (d);
}

/**
 * @brief	Return the number of globale variable wo are not NULL
 * 
 * @param	lst Global variable's list
 * @return	int 
 */
static int	ft_len_global(t_list *lst)
{
	int		count;
	t_list	*tmp;
	t_var	*var;

	count = 0;
	tmp = lst;
	while (tmp != NULL)
	{
		var = (t_var *)tmp->content;
		if (var->value != NULL)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

/**
 * @brief	Create, with malloc,  an array of string with globale variable for
 * 			execve
 * 
 * @param	lst Global variable's list
 * @param	i Index initialized at -1
 * @param	shell Our big struct
 * @return	char** 
 */
char	**ft_init_our_envp(t_list *lst, int i, t_shell *shell)
{
	int		len_envp;
	t_var	*var;
	t_list	*tmp;

	ft_free_array(&shell->envp);
	len_envp = ft_len_global(lst);
	shell->envp = (char **)ft_calloc(sizeof(char *), (len_envp + 1));
	if (!shell->envp)
		return (perror("minishell: malloc"), NULL);
	shell->envp[len_envp] = NULL;
	tmp = lst;
	while (++i < len_envp)
	{
		var = (t_var *)tmp->content;
		if (var->value == NULL)
			continue ;
		shell->envp[i] = ft_var_to_char(var);
		if (!shell->envp[i])
			return (perror("minishell: malloc"), ft_free_array(&shell->envp), \
			NULL);
		tmp = tmp->next;
	}
	return (shell->envp);
}

/**
 * @brief	Allocate memory for the input of readline
 * 
 * @param	add string size of 4 character
 * @return	Pointer of the string, NULL if error
 */
char	*ft_init_prompt(char *add)
{
	char	*str;
	char	*str2;
	int		len;

	str = getcwd(NULL, 0);
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	str2 = (char *)malloc(sizeof(char) * (len + 5));
	if (!str2)
		return (perror("minishell: malloc"), free(str), NULL);
	ft_memcpy(str2, str, len);
	ft_memcpy(str2 + len, add, 5);
	return (free(str), str2);
}

// bool	ft_have_four_prim_var(t_list *start)
// {
// 	if (!ft_is_exist_already("OLDPWD", start, 6))
// 		return (false);
// 	if (!ft_is_exist_already("_", start, 1))
// 		return (false);
// 	if (!ft_is_exist_already("SHLVL", start, 5))
// 		return (false);
// 	if (!ft_is_exist_already("PWD", start, 3))
// 		return (false);
// 	return (true);
// }
