/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:26:27 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/03 20:01:03 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*ft_strtrim_llemmel(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_strchr(set, s1[start]) != NULL)
		start++;
	while (end > start && ft_strchr(set, s1[start]) != NULL)
		end--;
	if (ft_strchr(set, s1[start]) != NULL)
		end--;
	return (ft_substr(s1, start, (end - start + 1)));
}

/**
 * @brief	Skip all whitespaces in the given string.
 * 
 * @param	str The string to skip whitespaces in.
 * @return	char* A pointer to the first non-whitespace character.
 */
char	*skip_whitespaces(char *str)
{
	while (*str && ft_strchr(WSPACE, *str))
		str++;
	if (!*str)
		return (NULL);
	return (str);
}

/**
 * @brief	Return the index of the next character c in the string str.
 * 
 * @param	str The string to search in.
 * @param	c The character to search.
 * @return	size_t The index of the character c in the string str.
 */
size_t	ft_strlen_char(char *str, char c)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	if (str[len] == c && c != '\0')
		len++;
	return (len);
}

/**
 * @brief	Get the size of the next token in the command line.
 * 
 * @param	cmd_line A pointer to the command line.
 * @return	size_t The size of the next token.
 */
size_t	get_token_size(char *cmd_line)
{
	size_t	token_size;

	token_size = 0;
	if (!ft_strncmp(cmd_line, ">>", 2) || !ft_strncmp(cmd_line, "<<", 2))
		return (2);
	else if (ft_strchr("><|", *cmd_line))
		return (1);
	while (cmd_line[token_size] && !ft_strchr(SEPARATORS, cmd_line[token_size]))
	{
		if (cmd_line[token_size] == '\"')
			token_size += ft_strlen_char(cmd_line + token_size + 1, '\"') + 1;
		else if (cmd_line[token_size] == '\'')
			token_size += ft_strlen_char(cmd_line + token_size + 1, '\'') + 1;
		else
			token_size++;
	}
	return (token_size);
}

/**
 * @brief	Add a new token to the array of tokens.
 * 
 * @param	tokens A pointer to the array of tokens.
 * @param	new_token The new token to add.
 * @param	error A pointer to a boolean to set to true if an error occured.
 */
void	add_token(t_token **tokens, t_token new_token, bool *error)
{
	t_token	*new_token_tab;
	size_t	token_tab_size;

	if (!*tokens || !new_token.value)
		return ;
	token_tab_size = 0;
	while ((*tokens)[token_tab_size].value)
		token_tab_size++;
	new_token_tab = (t_token *)ft_calloc(token_tab_size + 2, sizeof(t_token));
	if (!new_token_tab)
		return (perror("minishell"), (void)(*error = true));
	ft_memcpy(new_token_tab, *tokens, sizeof(t_token) * token_tab_size);
	new_token_tab[token_tab_size] = new_token;
	new_token_tab[token_tab_size + 1].type = TYPE_UNKNOW;
	new_token_tab[token_tab_size + 1].value = NULL;
	free(*tokens);
	*tokens = new_token_tab;
}
