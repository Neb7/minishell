/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 01:29:14 by capi              #+#    #+#             */
/*   Updated: 2025/03/10 14:30:17 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	fill_command(t_shell *shell, t_command *cmd, size_t *i, bool *error)
{
	while (shell->tokens[*i].value && shell->tokens[*i].type != TYPE_PIPE)
	{
		if (shell->tokens[*i].type == TYPE_COMMAND \
			&& !add_cmd_name(cmd, &shell->tokens[*i], error))
			return (ret_false_error(error, cmd));
		else if (shell->tokens[*i].type == TYPE_ARG \
			&& !add_arg(&cmd->args, shell->tokens[*i].value))
			return (ret_false_error(error, cmd));
		else if (is_redirection(shell->tokens[*i]) \
			&& !add_redirection(cmd, shell->tokens, i, error))
			return (ret_false_error(error, cmd));
		if (shell->tokens[*i].value)
			*i += 1;
	}
	return (true);
}

bool	fill_empty_command(t_shell *shell, t_command *cmd, bool *error)
{
	cmd->type_empty = true;
	cmd->path = ft_strdup("");
	if (!cmd->path)
		return (perror("minishell"), ret_false_error(error, cmd));
	if (!add_arg(&cmd->args, ""))
		return (ret_false_error(error, cmd));
	add_node(&shell->node_tab, create_node(cmd, NULL, NODE_COMMAND), error);
	return (true);
}
