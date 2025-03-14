/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:15:13 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/10 13:42:36 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*get_special_var_value(t_shell shell, char *value, \
	size_t *i, bool in_quotes)
{
	char	*var_value;

	var_value = NULL;
	if (value[*i + 1] == '?')
		var_value = get_split_var_value(shell, "?", in_quotes);
	else if (!ft_strncmp(value, "~", 1) && (value[1] == '/' || !value[1]))
		var_value = get_split_var_value(shell, "HOME", in_quotes);
	if (!var_value)
		return (perror("minishell"), NULL);
	return (var_value);
}

char	*get_var_name(char *value, size_t *name_size)
{
	*name_size = get_var_size(value);
	if (*value == '?')
		*name_size = 1;
	if (!*name_size && (*value != '\'' && *value != '"'))
		return (NULL);
	return (ft_substr(value, 0, *name_size));
}

bool	check_and_do_expand_var(t_shell shell, char **value, \
	size_t *i, bool in_dquote)
{
	if (!in_dquote || ((*value)[*i + 1] != '\"' && (*value)[*i + 1] != '\''))
	{
		if (!expand_var(shell, value, i, in_dquote))
			return (false);
	}
	else
		(*i)++;
	return (true);
}
