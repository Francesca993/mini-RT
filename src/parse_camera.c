/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:12:18 by francesca         #+#    #+#             */
/*   Updated: 2025/10/08 11:22:25 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <limits.h>

/*
C -50.0,0,20 0,0,1 70
∗ identifier: C
∗ x, y, z coordinates of the viewpoint: -50.0,0,20
∗ 3D normalized orientation vector, in the range [-1,1] for each x, y, z axis:
0.0,0.0,1.0
∗ FOV: Horizontal field of view in degrees in the range [0,180]: 70

*/

/* ============================= parse_int (libft) =========================== */
/* Legge un intero con segno in base-10 e aggiorna il puntatore.
 * Ritorna 1 se ok, 0 se errore (nessuna cifra o overflow). */
int	parse_int(const char **line_pointer, int *out_value)
{
	const char	*cursor;
	long		accumulator;
	int			sign;

	if (line_pointer == 0 || *line_pointer == 0 || out_value == 0)
		return (0);
	cursor = skip_leading_spaces(*line_pointer);
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