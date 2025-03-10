/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:02:08 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/05 17:17:13 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Add an argument to the command.
 * 
 * @param	tab A pointer to the argument array.
 * @param	value The value of the argument to add.
 * @return	true if the argument was added successfully, false otherwise.
 */
static bool	add_arg(char ***tab, char *value)
{
	char	**new_tab;
	size_t	tab_size;

	if (!*tab)
	{
		*tab = (char **)ft_calloc(2, sizeof(char *));
		if (!*tab)
			return (perror("minishell"), false);
		(*tab)[0] = ft_strdup(value);
		if (!(*tab)[0])
			return (perror("minishell"), false);
		return (true);
	}
	tab_size = get_tab_size(*tab);
	new_tab = (char **)ft_calloc(tab_size + 2, sizeof(char *));
	if (!new_tab)
		return (perror("minishell"), false);
	ft_memcpy(new_tab, *tab, tab_size * sizeof(char *));
	free(*tab);
	new_tab[tab_size] = ft_strdup(value);
	if (!new_tab[tab_size])
		return (perror("minishell"), free_tab(new_tab), false);
	*tab = new_tab;
	return (true);
}

/**
 * @brief	Add the command name to the command structure.
 * 
 * @param	command A pointer to the command structure.
 * @param	token A pointer to the current token.
 * @param	error A pointer to the error flag.
 * @return	true if the command name was added successfully, false otherwise.
 * 			If an error occurs, the error flag is set to true.
 */
static bool	add_cmd_name(t_command *command, t_token *token, bool *error)
{
	if (!is_builtins(token->value))
	{
		command->path = ft_strdup(token->value);
		if (!command->path)
			return (perror("minishell"), ret_false_error(error, NULL));
	}
	if (!ft_strrchr(token->value, '/'))
		add_arg(&command->args, token->value);
	else
		add_arg(&command->args, token->raw_value);
	if (!command->args)
		return (ret_false_error(error, NULL));
	return (true);
}

/**
 * @brief	Add redirection file to the command.
 * 
 * The function create a new file structure and add it to the command.
 * 
 * @param	command A pointer to the command structure.
 * @param	tokens A pointer to the tokens array.
 * @param	i A pointer to the index of the current token.
 * @param	err A pointer to the error flag.
 * @return	true if the file was added successfully, false otherwise.
 * 			If an error occurs, the error flag is set to true.
 */
static bool	add_redirection(t_command *command, t_token *tokens, \
		size_t *i, bool *err)
{
	t_list	*node;
	t_file	*f;
	char	*value;

	if (!tokens[*i + 1].value)
		value = NULL;
	else
		value = tokens[*i + 1].value;
	if (tokens[*i].type == TYPE_REDIRECT_APPEND \
		|| tokens[*i].type == TYPE_HERE_DOC)
		f = create_file(value, true);
	else
		f = create_file(value, false);
	if (!f)
		return (ret_false_error(err, NULL));
	node = ft_lstnew(f);
	if (!node)
		return (perror("minishell"), free_file(f), ret_false_error(err, NULL));
	if (tokens[*i].type == TYPE_REDIRECT_APPEND \
		|| tokens[*i].type == TYPE_REDIRECT_OUTPUT)
		ft_lstadd_back(&command->output_file, node);
	else
		ft_lstadd_back(&command->input_file, node);
	*i += 1;
	return (true);
}

/**
 * @brief	Get a new command node and add it to the node_tab.
 * 
 * Initially the function get the command name, then the arguments and finally
 * the redirections. If an error occurs, the error flag is set to true.
 * 
 * After getting all the informations, open all redirection files. 
 * 
 * @param	shell A pointer to the shell structure.
 * @param	i A pointer to the index of the current token.
 * @param	error A pointer to the error flag.
 * @return	true if the command node was added successfully, false otherwise.
 * 			If an error occurs, the error flag is set to true.
 */
bool	get_simple_command(t_shell *shell, size_t *i, bool *error)
{
	t_command	*cmd;

	cmd = create_command(NULL);
	if (!cmd)
		return (ret_false_error(error, NULL));
	shell->c_command = cmd;
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
	if (!open_files(shell, cmd))
		return (ret_false_error(error, cmd));
	if (*error == false)
		add_node(&shell->node_tab, create_node(cmd, NULL, NODE_COMMAND), error);
	return (!*error);
}
