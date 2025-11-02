/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_spaces.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:55:12 by francesca         #+#    #+#             */
/*   Updated: 2025/10/30 20:04:34 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Return 1 if the character is one of the expected ones
*/
int	ft_is_space_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f');
}

/*
Return 1 if the string is empty/spaces
*/
int	ft_is_blank(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	while (s[i])
	{
		if (!ft_is_space_char(s[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
Skip the spaces
*/
const char	*skip_spaces(const char *p)
{
	while (*p && ft_is_space_char(*p))
		p++;
	return (p);
}
