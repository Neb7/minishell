/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_simple_quote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:11:17 by llemmel           #+#    #+#             */
/*   Updated: 2025/02/14 14:31:40 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/**
 *	This function skips simple quotes in the value string
 *	
 *	@param value: the string to skip simple quotes in
 *	@param i: the address of the index to start skipping from
*/
void	skip_simple_quote(char *value, size_t *i)
{
	if (!value || !i)
		return ;
	if (value[*i] == '\'')
		*i += 1;
	while (value[*i])
	{
		if (value[*i] == '\'')
		{
			*i += 1;
			return ;
		}
		*i += 1;
	}
}
