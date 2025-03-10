/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:46:26 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/03 20:12:56 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 * @brief	Cound the number of characters in the charset between start and end.
 * 
 * @param	str The string to count.
 * @param	charset The charset to count.
 * @param	start The start index.
 * @param	end The end index.
 * @return	size_t The number of characters in the charset between start and end
 */
size_t	count_charset_n(char *str, char *charset, size_t start, size_t end)
{
	size_t	i;
	size_t	nb_char;

	nb_char = 0;
	i = start;
	while (str[i] && i < end)
	{
		if (ft_strchr(charset, str[i]))
			nb_char++;
		i++;
	}
	return (nb_char);
}

/**
 * @brief	Delete all characters in the charset between start and end.
 * 
 * @param	str The string to clean.
 * @param	charset The charset to delete.
 * @param	start The start index.
 * @param	end The end index.
 * @return	char* The new string.
 */
char	*del_charset_n(char *str, char *charset, size_t start, size_t end)
{
	char	*new;
	size_t	nb_char;
	size_t	i;
	size_t	j;

	nb_char = count_charset_n(str, charset, start, end);
	new = (char *)ft_calloc(ft_strlen(str) - nb_char + 1, sizeof(char));
	if (!new)
		return (perror("minishell"), free(str), NULL);
	i = 0;
	j = 0;
	while (str[j])
	{
		if ((j >= start && j <= end) && ft_strchr(charset, str[j]))
		{
			j++;
			continue ;
		}
		new[i++] = str[j++];
	}
	free(str);
	return (new);
}
