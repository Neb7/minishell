/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:33:06 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/10 13:56:39 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	The sigaction structure is defined by something like:

struct sigaction {
    void   (*sa_handler) (int);
    void   (*sa_sigaction) (int, siginfo_t *, void *);
    sigset_t sa_mask;
    int sa_flags;
    void   (*sa_restorer) (void);
};

option:
SA_SIGINFO: Allows the use of sa_sigaction instead of sa_handler. This provides
access to additional information on the signal, via a siginfo_t structure (used
to access the client PID) and a void context.
SA_RESTART: Automatically restores system calls interrupted by this signal.
SA_NODEFER: Prevents the processed signal from being blocked automatically 
during the execution of the handler.

int sigemptyset(sigset_t *set); empty set of all signals to not block any signal

sigaction(the signal, pointer to sigaction structure, pointer to store)
sigaction is used to modify the action to be performed when receiving a specific
signal
 * 
 * @param	sig Signal id
 * @param	ft_ges Function to use in sa_sigaction
 */
void	ft_init_sig(int sig, void (*ft_ges)(int, siginfo_t *, void *))
{
	struct sigaction	signal;

	signal.sa_sigaction = ft_ges;
	signal.sa_flags = SA_SIGINFO;
	sigemptyset(&signal.sa_mask);
	if (sig == SIGQUIT)
		sigaction(SIGQUIT, &signal, 0);
	else if (sig == SIGINT)
		sigaction(SIGINT, &signal, 0);
	else if (sig == SIGSEGV)
		sigaction(SIGSEGV, &signal, 0);
	else if (sig == SIGUSR2)
		sigaction(SIGUSR2, &signal, 0);
	else if (sig == SIGUSR1)
		sigaction(SIGUSR1, &signal, 0);
	else if (sig == SIGPIPE)
		sigaction(SIGPIPE, &signal, 0);
}

/**
 * @brief	The handle signal function for here doc
 * 
 * @param	sig Signal id
 * @param	info Struct with some information from the signal's sender
 * @param	context Processor registry context
 */
void	ft_handle_here_doc(int sig, siginfo_t *info, void *context)
{
	t_shell	*shell;

	shell = ft_acces_shell(NULL);
	if (sig == SIGINT)
	{
		g_signal[1] = 123;
		shell->last_exit_status = 130;
	}
	else if (sig == SIGQUIT)
	{
		g_signal[2] = 2;
	}
	(void)info;
	(void)context;
}

void	ft_handle_here_doc_child(int sig, siginfo_t *info, void *context)
{
	t_shell	*shell;

	shell = ft_acces_shell(NULL);
	if (sig == SIGQUIT)
	{
		g_signal[2] = 2;
		rl_on_new_line();
		rl_redisplay();
		write(STDIN_FILENO, "  \b\b", 4);
	}
	else if (sig == SIGINT)
	{
		free(shell->trim_command);
		free_command2(&shell->c_command);
		free(shell->file_name_temp);
		ft_free_child(shell, 130);
	}
	(void)info;
	(void)context;
}

/**
 * @brief	The handle signal function for execution
 * 
 * @param	sig Signal id
 * @param	info Struct with some information from the signal's sender
 * @param	context Processor registry context
 */
void	ft_ges_in_exe(int sig, siginfo_t *info, void *context)
{
	if (g_signal[1] == 0)
	{
		if (sig == SIGINT)
		{
			g_signal[0] = 1;
			g_signal[3] = 1;
		}
		else if (sig == SIGQUIT)
		{
			g_signal[0] = 2;
			g_signal[3] = 1;
		}
	}
	if (sig == SIGUSR1)
	{
		g_signal[1] -= 1;
	}
	(void)context;
	(void)info;
}

/**
 * @brief	The handle signal function before execution
 * 
 * @param	sig Signal id
 * @param	info Struct with some information from the signal's sender
 * @param	context Processor registry context
 */
void	ft_ges(int sig, siginfo_t *info, void *context)
{
	t_shell	*shell;

	shell = ft_acces_shell(NULL);
	if (sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		shell->last_exit_status = 130;
	}
	else if (sig == SIGUSR2)
	{
		shell->pid_parent = info->si_pid;
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("  \b\b");
	}
	(void)context;
}
