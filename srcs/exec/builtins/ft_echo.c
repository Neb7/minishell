/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:05:28 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/05 15:36:13 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Check if 'str' is composed only of the character 'c'
 * 
 * @param	str The string to compare
 * @param	c The character with which check
 * @return	true : The string is composed only of the character 'c' ; false :
 * 			The string is not just composing from the character 'c'
 */
static bool	ft_echo_only_char(char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c)
			return (false);
		i++;
	}
	if (i == 0)
		return (false);
	return (true);
}

/**
 * @brief	Print 'str' and check if next is NULL for print an space
 * 
 * @param	str String to write
 * @param	next POinter of next string
 * @param	exit_status 
 */
static void	ft_print_echo(char *str, char *next, int *exit_status)
{
	if (write(1, str, ft_strlen(str)) < 0)
		*exit_status = 1;
	if (next != NULL)
	{
		if (write(1, " ", 1) < 0)
			*exit_status = 1;
	}
}

/**
 * @brief	Parse all arguments until one of them isn't an flags "-n" then
 * 			write the others and exit the processe.
 * 
 * @param	shell Our big struct
 * @param	com Struct of our command
 */
void	ft_echo(t_command *com, t_shell *shell)
{
	size_t	i;
	bool	flags;
	int		exit_status;

	i = 0;
	exit_status = 0;
	flags = false;
	while (com->args[++i] != NULL)
	{
		if (com->args[i][0] == '-' && ft_echo_only_char(com->args[i] + 1, 'n'))
			flags = true;
		else
			break ;
	}
	while (com->args[i] != NULL)
	{
		ft_print_echo(com->args[i], com->args[i + 1], &exit_status);
		i++;
	}
	if (!flags)
		if (write(1, "\n", 1) < 0)
			exit_status = 1;
	ft_free_child(shell, exit_status);
}
