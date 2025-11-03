/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_units_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 11:44:55 by francesca         #+#    #+#             */
/*   Updated: 2025/11/03 16:52:31 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static inline void	init_parameters(t_numparse *n)
{
	n->sign = 1;
	n->val = 0.0;
	n->frac = 0.0;
	n->scale = 1.0;
	n->ndigits = 0;
	n->nfrac = 0;
}

static inline void	parse_numdoub(t_numparse *n)
{
	if (*n->cursor == '+' || *n->cursor == '-')
	{
		if (*n->cursor == '-')
			n->sign = -1;
		n->cursor++;
	}
	while (ft_isdigit((unsigned char)*n->cursor))
	{
		n->val = n->val * 10.0 + (double)(*n->cursor - '0');
		n->cursor++;
		n->ndigits++;
	}
	if (*n->cursor == '.')
	{
		n->cursor++;
		while (ft_isdigit((unsigned char)*n->cursor))
		{
			n->frac = n->frac * 10.0 + (double)(*n->cursor - '0');
			n->scale = n->scale * 10.0;
			n->cursor++;
			n->nfrac++;
		}
	}
}

int	parse_double(const char **line, double *out)
{
	t_numparse	n;
	double		fractional;

	fractional = 0.0;
	n.cursor = skip_spaces(*line);
	init_parameters(&n);
	parse_numdoub(&n);
	if (n.ndigits == 0 && n.nfrac == 0)
		return (0);
	if (n.nfrac > 0)
		fractional = n.frac / n.scale;
	else
		fractional = 0.0;
	*out = n.sign * (n.val + fractional);
	*line = n.cursor;
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
