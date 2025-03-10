/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:53:16 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/05 15:56:04 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Iniatializ the struct shell
 * 
 * @param	envp Environment variables
 * @return	Pointer of struct or NULL if error
 */
t_shell	*ft_init_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
	if (!shell)
		return (perror("minishell"), NULL);
	shell->prompte = ft_init_prompt(" $> ");
	if (!shell->prompte)
		return (ft_error_malloc(shell), NULL);
	shell->global_var = ft_init_global(envp, 0, NULL, shell);
	if (!shell->global_var)
		return (ft_error_malloc(shell), NULL);
	shell->save_path = true;
	shell->edit_terme = true;
	return (shell);
}

/**
 * @brief	Access to pointer of shell
 * 
 * @param	envp Environment variables
 * @return	Pointer of shell
 */
t_shell	*ft_acces_shell(char **envp)
{
	static t_shell	*shell = NULL;

	if (!shell)
		shell = ft_init_shell(envp);
	if (!shell)
		return (NULL);
	return (shell);
}
