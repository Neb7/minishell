/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:14:30 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/06 12:49:11 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Get the next token of the command line.
 * 
 * @param	cmd_line The command line to tokenize.
 * @param	tokens A pointer to the token array.
 * @param	error A pointer to a boolean to set to true if an error occured.
 * @return	The command line directly after the token. 
 */
static char	*get_next_token(char *cmd_line, t_token **tokens, bool *error)
{
	t_token	new_token;
	size_t	token_size;

	token_size = get_token_size(cmd_line);
	if (token_size == 0)
	{
		**tokens = (t_token){0};
		return (cmd_line + 1);
	}
	new_token.type = TYPE_UNKNOW;
	new_token.value = ft_substr(cmd_line, 0, token_size);
	if (!new_token.value)
		return (perror("minishell"), *error = true, NULL);
	new_token.raw_value = NULL;
	add_token(tokens, new_token, error);
	if (*error)
		return (free(new_token.value), NULL);
	return (cmd_line + token_size);
}

/**
 * @brief	Check if the token at the given index is a command.
 * 
 * @param	tokens A pointer to the token array.
 * @param	i The index of the token to check.
 * @return	true if the token is a command, false otherwise.
 */
static bool	check_command(t_token **tokens, size_t i)
{
	while (i > 0)
	{
		if ((*tokens)[i].type == TYPE_PIPE)
			return (false);
		else if ((*tokens)[i].type == TYPE_COMMAND)
			return (true);
		i--;
	}
	if ((*tokens)[i].type == TYPE_COMMAND)
		return (true);
	return (false);
}

/**
 * @brief	The part 1 of add_token_type
 * 
 * @param	tokens A pointer to the token array.
 * @param	i The index of the token to check.
 * @return	true if the type has been added, false otherwise.
 */
static bool	add_simple_type(t_token **tokens, size_t i)
{
	if (*(*tokens)[i].value == '|')
		(*tokens)[i].type = TYPE_PIPE;
	else if (!ft_strncmp((*tokens)[i].value, ">", 2))
		(*tokens)[i].type = TYPE_REDIRECT_OUTPUT;
	else if (!ft_strncmp((*tokens)[i].value, "<", 2))
		(*tokens)[i].type = TYPE_REDIRECT_INPUT;
	else if (!ft_strncmp((*tokens)[i].value, ">>", 3))
		(*tokens)[i].type = TYPE_REDIRECT_APPEND;
	else if (!ft_strncmp((*tokens)[i].value, "<<", 3))
		(*tokens)[i].type = TYPE_HERE_DOC;
	else
		return (false);
	return (true);
}

/**
 * @brief	Add type to each token in the given token array.
 * 
 * @param	tokens A pointer to the token array.
 */
static void	add_token_type(t_token **tokens)
{
	size_t	i;

	i = 0;
	while ((*tokens)[i].value)
	{
		if (add_simple_type(tokens, i))
		{
			i++;
			continue ;
		}
		else if (i && ((*tokens)[i - 1].type == TYPE_REDIRECT_INPUT
			|| (*tokens)[i - 1].type == TYPE_REDIRECT_OUTPUT
			|| (*tokens)[i - 1].type == TYPE_REDIRECT_APPEND
			|| (*tokens)[i - 1].type == TYPE_HERE_DOC))
			(*tokens)[i].type = TYPE_FILE;
		else if (i && (((*tokens)[i - 1].type == TYPE_COMMAND \
			|| (*tokens)[i - 1].type == TYPE_ARG \
			|| (check_command(tokens, i) \
			&& (*tokens)[i - 1].type == TYPE_FILE))))
			(*tokens)[i].type = TYPE_ARG;
		else
			(*tokens)[i].type = TYPE_COMMAND;
		i++;
	}
}

/**
 * @brief	Create a new token array with the given cmd_line who was previously
 * 			trimmed.
 * 
 * The function will create a new token array by tokenizing the given command
 * line. The function will also check for syntax errors and return NULL if any
 * error is found.
 * 
 * The function will iterate over the command line and get all tokens, then
 * it will add type to each token. If a syntax error is found, the function will
 * return NULL and free the allocated memory.
 * 
 * @param	cmd_line The command line to tokenize. 
 * @return	The new token array or NULL if an error occured.
 */
t_token	*tokenize(char *cmd_line)
{
	bool	error;
	t_token	*tokens;

	error = false;
	tokens = NULL;
	if (!cmd_line || *cmd_line == '\0')
		return (NULL);
	tokens = ft_calloc(1, sizeof(t_token));
	if (!tokens)
		return (perror("minishell: "), NULL);
	while (*cmd_line)
	{
		cmd_line = skip_whitespaces(cmd_line);
		if (!cmd_line || !*cmd_line)
			break ;
		cmd_line = get_next_token(cmd_line, &tokens, &error);
		if (error)
			return (free_token(tokens), NULL);
		if (!cmd_line || !*cmd_line)
			break ;
	}
	add_token_type(&tokens);
	if (check_syntax_error(tokens))
		return (free_token(tokens), NULL);
	return (tokens);
}
