/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:08:52 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/05 16:35:16 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Free all string in the array and the array and points it at NULL
 * 
 * @param	array_string 
 */
void	ft_free_array(char ***array_string)
{
	int	i;

	i = 0;
	if (*array_string)
	{
		while ((*array_string)[i] != NULL)
		{
			free((*array_string)[i]);
			i++;
		}
		free(*array_string);
		*array_string = NULL;
	}
}

/**
 * @brief	Print an error message with 2 string
 * 
 * @param	str First string
 * @param	s2 Second string
 */
void	ft_error(char *str, char *s2)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(s2, 2);
	write(2, "'\n", 2);
}

/**
 * @brief	Free all the struct shell
 * 
 * @param	shell Our big struct
 */
void	ft_free_shell(t_shell *shell)
{
	rl_clear_history();
	if (shell)
	{
		if (shell->pid)
			free(shell->pid);
		ft_free_pipe(shell);
		if (shell->envp)
			ft_free_array(&shell->envp);
		free_var(shell, NULL);
		free(shell->prompte);
		free(shell->input);
		ft_lstclear(&shell->global_var, ft_del_var);
		free(shell);
	}
}

/**
 * @brief	Free all pipe in shell->pipe and free shell->pipe
 * 
 * @param	shell Our big struct 
 */
void	ft_free_pipe(t_shell *shell)
{
	size_t	i;

	i = 0;
	if (shell->pipe)
	{
		while (i < shell->nb_command)
		{
			free(shell->pipe[i]);
			i++;
		}
		free(shell->pipe);
		shell->pipe = NULL;
	}
}

/**
 * @brief	Free all value in ast and free shell before exit with EXIT_FAILURE
 * 
 * @param	shell Our big struct
 */
void	ft_error_malloc(t_shell *shell)
{
	ft_free_shell(shell);
	exit(EXIT_FAILURE);
}
