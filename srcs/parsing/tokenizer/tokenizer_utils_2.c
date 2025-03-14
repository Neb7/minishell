/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:55:19 by capi              #+#    #+#             */
/*   Updated: 2025/03/10 01:30:27 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	get_last_token(t_token *tokens)
{
	size_t	i;

	i = 0;
	while (tokens[i].value)
		i++;
	if (i == 0)
		return (tokens[0]);
	return (tokens[i - 1]);
}

t_token	get_token_bf(t_token *tokens, size_t i)
{
	while (i > 0)
	{
		if (tokens[i].type == TYPE_EMPTY)
		{
			i--;
			continue ;
		}
		else
			return (tokens[i]);
		i--;
	}
	return (tokens[i]);
}

bool	check_and_do_expand_token(t_shell *shell, t_token *new_token, \
	t_token *ltoken, bool *error)
{
	bool	quote;

	if (ft_strchr(new_token->value, '\'') || ft_strchr(new_token->value, '\"'))
		quote = true;
	else
		quote = false;
	if (ltoken->type != TYPE_HERE_DOC && \
		!expand_token(*shell, &new_token->value))
		return (free(new_token->value), *error = true, false);
	if (!quote && !is_redirection(*ltoken) \
		&& new_token->value[0] == '\0')
		new_token->type = TYPE_EMPTY;
	new_token->raw_value = NULL;
	return (true);
}
