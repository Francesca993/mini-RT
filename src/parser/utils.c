/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:25:56 by francesca         #+#    #+#             */
/*   Updated: 2025/11/03 16:54:32 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Internal helper: consumes a comma with optional spaces around it and returns
0 if it does not find the comma
*/
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

/*
Print error message
*/
int	print_err_msg(const char *msg)
{
	fprintf(stderr, "Error\n");
	fprintf(stderr, "%s\n", msg);
	return (1);
}

/*
Print specifc error message
*/
int	err_msg(char *type, int num, const char *msg)
{
	fprintf(stderr, "%s numero: %d, %s\n", type, num, msg);
	return (1);
}

/*
Creates a t_vector variable, assigns the x, y, z and w values to its fields,
and returns it.
*/
t_vector	vector_make(double x, double y, double z, double w)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return (v);
}
