/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:25:56 by francesca         #+#    #+#             */
/*   Updated: 2025/10/08 14:09:31 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	print_usage(const char *prog)
{
	fprintf(stderr, "Usage: %s <scene.rt>\n", prog);
}

int	print_err_msg(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	return (1);
}

/* Rimuove New_line finale */
void	chop_newline(char *s)

{
	size_t n;

	if (!s)
		return ;
	n = ft_strlen(s);
	if (n > 0 && s[n - 1] == '\n')
		s[n - 1] = '\0';
	return ;
}

/* helper interno: consuma una virgola con spazi opzionali attorno e ritorna 0 se non trova la virgola*/
int	skip_comma(const char **input_ptr)
{
	const char	*cursor;

	if (input_ptr == NULL || *input_ptr == NULL)
		return (0);
	cursor = *input_ptr;
	if (*cursor != ',')
		return (0);
	cursor++;
	*input_ptr = cursor;
	return (1);
}
/* Converte da stringa a intero ma restituisce anche il puntatorte alla stringa */
int	parse_int(const char **line_pointer, int *out_value)
{
	const char	*cursor;
	long		accumulator;
	int			sign;

	if (line_pointer == 0 || *line_pointer == 0 || out_value == 0)
		return (0);
	cursor = skip_spaces(*line_pointer);
	accumulator = 0;
	sign = 1;
	if (*cursor == '+' || *cursor == '-')
	{
		if (*cursor == '-')
			sign = -1;
		cursor++;
	}
	if (!ft_isdigit(*cursor))
		return (0);
	while (ft_isdigit(*cursor))
	{
		accumulator = accumulator * 10 + (*cursor - '0');
		if (sign == 1 && accumulator > (long)INT_MAX)
			return (0);
		if (sign == -1 && -accumulator < (long)INT_MIN)
			return (0);
		cursor++;
	}
	*out_value = (int)(accumulator * sign);
	*line_pointer = cursor;
	return (1);
}
