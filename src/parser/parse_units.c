/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_units.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 13:46:16 by francesca         #+#    #+#             */
/*   Updated: 2025/10/31 11:46:15 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
do not leave spaces BEFORE the number:
the cursor must already be on the first digit
*/
static int	parse_rgb_component(const char **input_ptr, int *out_value)
{
	const char	*cursor;
	int			value;
	int			digit;

	if (input_ptr == NULL || *input_ptr == NULL || out_value == NULL)
		return (0);
	cursor = *input_ptr;
	if (*cursor == '+' || *cursor == '-' || !ft_isdigit(*cursor))
		return (0);
	value = 0;
	while (ft_isdigit(*cursor))
	{
		digit = (int)(*cursor - '0');
		value = value * 10 + digit;
		if (value > 255)
			return (0);
		cursor++;
	}
	*out_value = value;
	*input_ptr = cursor;
	return (1);
}

int	parse_rgb(const char **input_ptr, t_color *out_color)
{
	const char	*cursor;
	int			r;
	int			g;
	int			b;

	if (!input_ptr || !*input_ptr || !out_color)
		return (0);
	cursor = *input_ptr;
	if (!parse_rgb_component(&cursor, &r))
		return (0);
	if (!skip_comma(&cursor))
		return (0);
	if (!parse_rgb_component(&cursor, &g))
		return (0);
	if (!skip_comma(&cursor))
		return (0);
	if (!parse_rgb_component(&cursor, &b))
		return (0);
	out_color->r = (double)r / 255.0;
	out_color->g = (double)g / 255.0;
	out_color->b = (double)b / 255.0;
	*input_ptr = cursor;
	return (1);
}

/*
check overflow
*/
static inline int	check_value_max(int sign, long acc)
{
	if (sign == 1 && acc > (long)INT_MAX)
		return (0);
	if ((sign == -1) && (-acc < (long)INT_MIN))
		return (0);
	return (1);
}

/*
Helper: read sign, consume digits with
overflow check,update cursor and out_value
*/
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

/*
Converts from string to integer but
also returns the pointer back to the string
*/
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
