/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_units_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 11:44:55 by francesca         #+#    #+#             */
/*   Updated: 2025/11/02 14:21:57 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

//SE FUNZIONA LA CANCELLI

int	parse_double(const char **line, double *out)
{
	const char	*n_line;
	int			sign;
	double		val;
	double		frac;
	double		scale;
	int			ndigits;
	int			nfrac;
	double		fractional_part;

	n_line = skip_spaces(*line);
	sign = 1;
	val = 0.0;
	frac = 0.0;
	scale = 1.0;
	ndigits = 0;
	nfrac = 0;
	if (*n_line == '+' || *n_line == '-')
	{
		if (*n_line == '-')
			sign = -1;
		n_line++;
	}
	while (ft_isdigit(*n_line))
	{
		val = val * 10.0 + (double)(*n_line - '0');
		n_line++;
		ndigits++;
	}
	if (*n_line == '.')
	{
		n_line++;
		while (ft_isdigit(*n_line))
		{
			frac = frac * 10.0 + (double)(*n_line - '0');
			scale *= 10.0;
			n_line++;
			nfrac++;
		}
	}
	if (ndigits == 0 && nfrac == 0)
		return (0);
	if (nfrac == 0)
		fractional_part = 0.0;
	else
		fractional_part = frac / scale;
	*out = sign * (val + fractional_part);
	*line = n_line;
	return (1);
}

/*
Parse ‘x,y,z’ without spaces, using parse_double,
if int is 1 then set w to 1 otherwise set it to 0
*/
int	parse_vec3(const char **input_ptr, t_vector *out_vec, double normalized)
{
	const char	*cursor;
	double		x;
	double		y;
	double		z;

	if (input_ptr == NULL || *input_ptr == NULL || out_vec == NULL)
		return (0);
	cursor = *input_ptr;
	if (!parse_double(&cursor, &x))
		return (0);
	if (!skip_comma(&cursor))
		return (0);
	if (!parse_double(&cursor, &y))
		return (0);
	if (!skip_comma(&cursor))
		return (0);
	if (!parse_double(&cursor, &z))
		return (0);
	out_vec->x = x;
	out_vec->y = y;
	out_vec->z = z;
	out_vec->w = normalized;
	*input_ptr = cursor;
	return (1);
}
