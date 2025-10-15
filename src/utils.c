/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:25:56 by francesca         #+#    #+#             */
/*   Updated: 2025/10/15 16:54:54 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
/* Stampa messaggio di errore */
int	print_err_msg(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	return (1);
}

/* Helper per la norm per parse_int controlla overflow */
static inline int check_value_max(int sign, long acc)
{
	if (sign == 1 && acc > (long)INT_MAX)
		return (0);
	if (sign == -1 && -acc < (long)INT_MIN)
			return (0);
	return (1);
}

/* Helper: legge segno, consuma le cifre con overflow-check, aggiorna cursor e out_value */
static int	parse_int_core(const char **cursor, int *out_value)
{
	long	acc;
	int		sign;

	acc = 0;
	sign = 1;
	if (!cursor || !*cursor || !out_value)
		return (0);
	if (**cursor == '+' || **cursor == '-')
	{
		if (**cursor == '-')
			sign = -1;
		(*cursor)++;
	}
	if (!ft_isdigit(**cursor))
		return (0);
	while (ft_isdigit(**cursor))
	{
		acc = acc * 10 + (**cursor - '0');
		if (!check_value_max(sign, acc))
			return (0);
		(*cursor)++;
	}
	*out_value = (int)(acc * sign);
	return (1);
}

int	parse_int(const char **line_pointer, int *out_value)
{
	const char	*cursor;

	if (!line_pointer || !*line_pointer || !out_value)
		return (0);

	cursor = skip_spaces(*line_pointer);
	if (!parse_int_core(&cursor, out_value))
		return (0);

	*line_pointer = cursor;
	return (1);
}

/* Converte da stringa a intero ma restituisce anche il puntatorte alla stringa */
// int	parse_int(const char **line_pointer, int *out_value)
// {
// 	const char	*cursor;
// 	long		accumulator;
// 	int			sign;

// 	if (line_pointer == 0 || *line_pointer == 0 || out_value == 0)
// 		return (0);
// 	cursor = skip_spaces(*line_pointer);
// 	accumulator = 0;
// 	sign = 1;
// 	if (*cursor == '+' || *cursor == '-')
// 	{
// 		if (*cursor == '-')
// 			sign = -1;
// 		cursor++;
// 	}
// 	if (!ft_isdigit(*cursor))
// 		return (0);
// 	while (ft_isdigit(*cursor))
// 	{
// 		accumulator = accumulator * 10 + (*cursor - '0');
// 		if (sign == 1 && accumulator > (long)INT_MAX)
// 			return (0);
// 		if (sign == -1 && -accumulator < (long)INT_MIN)
// 			return (0);
// 		cursor++;
// 	}
// 	*out_value = (int)(accumulator * sign);
// 	*line_pointer = cursor;
// 	return (1);
// }

