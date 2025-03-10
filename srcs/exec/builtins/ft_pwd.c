/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:12:32 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/05 15:37:57 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Print the absolute path of the current repertoire 
 * 
 * @param	shell Our big struct
 */
void	ft_pwd(t_shell *shell)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("minishell: pwd");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", pwd);
	free(pwd);
	ft_free_child(shell, EXIT_SUCCESS);
}
