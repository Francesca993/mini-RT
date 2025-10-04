/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_spaces.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:55:12 by francesca         #+#    #+#             */
/*   Updated: 2025/10/04 14:43:15 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* --- utilità carattere (niente <ctype.h>) --- */
static int ft_is_space_char(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\r' || c == '\v' || c == '\f');
}
/* Ritorna 1 se tutta la stringa è vuota/spaziario */
int ft_is_blank(const char *s)
{
    int i = 0;
    if (!s) return 1;
    while (s[i])
    {
        if (!ft_is_space_char(s[i]))
            return 0;
        i++;
    }
    return 1;
}
