/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:06:19 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/09 16:06:20 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Free all structures and variables allocated during parsing.
 * 
 * @param	shell A pointer to the shell structure.
 * @param	command_line_trimmed The command line trimmed.
 */
void	free_var(t_shell *shell, char *command_line_trimmed)
{
	if (shell->tokens)
	{
		free_token(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->node_tab)
	{
		free_tab_node(shell->node_tab);
		shell->node_tab = NULL;
	}
	if (command_line_trimmed)
	{
		free(command_line_trimmed);
		command_line_trimmed = NULL;
	}
	shell->ast = NULL;
	shell->nb_command = 0;
}

/**
 * @brief	Check if all quotes are closed.
 * 
 * @param	command_line The command line to check. 
 * @return	true if all quotes are closed, false otherwise.
 */
static bool	check_quote(char *command_line)
{
	char	*end_quote;

	while (*command_line)
	{
		if (*command_line == '\'' || *command_line == '\"')
		{
			end_quote = ft_strchr(command_line + 1, *command_line);
			if (!end_quote)
			{
				ft_putendl_fd("minishell: syntax error: quote not closed", 2);
				return (false);
			}
			command_line = end_quote + 1;
		}
		else if (*command_line)
			command_line++;
	}
	return (true);
}

/**
 * @brief	Parse the command line and create the AST.
 * 
 * @param	shell A pointer to the shell structure.
 * @param	command_line The command line to parse.
 * @return	true if the command line is successfully parsed,
 * 			false if an error occurs.
 */
bool	parse_command_line(t_shell *shell, const char *command_line)
{
	shell->trim_command = ft_strtrim_llemmel(command_line, WSPACE);
	if (!shell->trim_command)
		return (perror("minishell"), false);
	else if (!*shell->trim_command)
		return (free(shell->trim_command), true);
	if (!check_quote(shell->trim_command))
		return (free(shell->trim_command), true);
	shell->tokens = tokenize(shell->trim_command);
	if (!shell->tokens)
		return (free_var(shell, shell->trim_command), false);
	if (!expander(shell))
		return (free_var(shell, shell->trim_command), false);
	if (!parser(shell))
		return (free_var(shell, shell->trim_command), false);
	if (!make_ast(shell, shell->node_tab))
		return (free_var(shell, shell->trim_command), false);
	free(shell->trim_command);
	return (true);
}
