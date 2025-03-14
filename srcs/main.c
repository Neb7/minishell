/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:44:32 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/14 14:40:39 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal[4];

static int	ft_isspace(int c)
{
	if ((char)c == ' ' || ((char)c < 13 && (char)c > 8))
		return (8192);
	return (0);
}

static bool	ft_only_space(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isspace(str[i]))
			return (false);
		i++;
	}
	return (true);
}

static void	ft_reset_sig(t_shell *shell)
{
	g_signal[0] = 0;
	g_signal[1] = 0;
	g_signal[2] = 0;
	g_signal[3] = 0;
	shell->idx_pid = 0;
	ft_init_sig(SIGINT, ft_ges);
	ft_init_sig(SIGQUIT, ft_ges);
	ft_init_sig(SIGQUIT, ft_ges);
}

void	ft_loop(t_shell *shell)
{
	while (1)
	{
		ft_reset_sig(shell);
		shell->input = readline(shell->prompte);
		if (shell->input && !ft_only_space(shell->input))
			add_history(shell->input);
		if (!shell->input)
			return (rl_clear_history(),
				ft_exit(shell, shell->last_exit_status));
		else if (!ft_strncmp(shell->input, "exit", 5))
			return (rl_clear_history(), ft_exit(shell, 0));
		else
		{
			parse_command_line(shell, shell->input);
			if (g_signal[1] != 123 && !ft_execute(shell))
			{
				rl_clear_history();
				free_var(shell, NULL);
				ft_exit(shell, -1);
			}
		}
		free(shell->input);
		free_var(shell, NULL);
	}
}

/*
	env -i bash --norc --noprofile
	// print_ast(shell->ast);
*/

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	ft_init_sig(SIGUSR2, ft_ges);
	ft_init_sig(SIGUSR1, ft_ges_in_exe);
	if (argc != 1)
		return (ft_putstr_fd("Error\nUsage: ./minishell\n", 2), EXIT_FAILURE);
	shell = ft_acces_shell(envp);
	if (!shell)
		return (EXIT_FAILURE);
	ft_loop(shell);
	(void)argv;
	return (ft_exit(shell, 0), 0);
}
