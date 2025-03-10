/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:06:41 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/03 20:17:53 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Remove quotes at the start and end of the value.
 * 
 * @param	value The value containing the quotes.
 * @param	start The index of the opening quote.
 * @param	end The index of the closing quote.
 * @return	char* The new value.
 */
static char	*remove_outer_quote(char *value, size_t start, size_t end)
{
	char	*new;
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	j = 0;
	len = ft_strlen(value);
	new = ft_calloc(len - 1, sizeof(char));
	if (!new)
		return (perror("minishell"), free(value), NULL);
	while (i < len)
	{
		if (i == start || i == end)
		{
			i++;
			continue ;
		}
		new[j++] = value[i++];
	}
	new[j] = '\0';
	free(value);
	return (new);
}

/**
 * @brief	Get the next who is not preceded by a backslash.
 * 
 * @param	value The value containing the quotes.
 * @param	i The index where the opening quote is.
 * @return	size_t The index of the closing quote.
 */
static size_t	get_next_quote(char *value, size_t i)
{
	size_t	next_quote;

	next_quote = i + 1;
	while (value[next_quote])
	{
		if (value[next_quote] == value[i] && value[next_quote - 1] != '\\')
			break ;
		next_quote++;
	}
	return (next_quote);
}

/**
 * @brief	Expand quotes in a token.
 * 
 * @param	value A pointer to the token value.
 * @return	true if the quotes are successfully expanded,
 * 			false if an error occurs.
 */
bool	expand_quote(char **value)
{
	size_t	i;
	size_t	next_quote;

	i = 0;
	while (value && *value && (*value)[i])
	{
		if ((*value)[i] == '\'' || (*value)[i] == '\"')
		{
			if (i == 0 || (*value)[i - 1] != '\\')
			{
				next_quote = get_next_quote(*value, i);
				if ((*value)[next_quote] != (*value)[i])
					return (false);
				*value = remove_outer_quote(*value, i, next_quote);
				if (!*value)
					return (false);
				i = next_quote - 1;
			}
			else if ((*value)[i])
				i++;
		}
		else if ((*value)[i])
			i++;
	}
	return (true);
}
