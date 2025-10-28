/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:06:02 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/24 13:51:33 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (i < n)
	{
		if ((unsigned char) str[i] == (unsigned char) c)
		{
			//return ((char *) &s[i]);
			return ((char *)(s + i));
		}
		i++;
	}
	return (NULL);
}
