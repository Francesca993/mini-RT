/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 23:12:05 by francesca         #+#    #+#             */
/*   Updated: 2025/10/30 21:18:37 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Sintax expected:
* A <ratio> <R,G,B>
* ratio: double in [0..1]
* R,G,B: int in [0..255]
Retturn 0 success, 1 error
*/
static inline int	parse_ambient(const char **pcursor, t_color *color,
		double *ratio_value)
{
	const char	*cursor;

	cursor = skip_spaces(*pcursor);
	if (!parse_double(&cursor, ratio_value))
		return (print_err_msg("Invalid 'A' ratio format"));
	if (*ratio_value < 0.0 || *ratio_value > 1.0)
		return (print_err_msg("'A' ratio out of range [0..1]"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missing 'A' color (expected R,G,B)"));
	if (!parse_rgb(&cursor, color))
		return (print_err_msg("Invalid 'A' color format"));
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Extra token after color in 'A'"));
	*pcursor = cursor;
	return (0);
}

int	parse_ambient_line(t_scene *scene, char *rest_of_line)
{
	const char	*cursor;
	t_color		color;
	double		ratio_value;

	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Missing parameter for 'A'"));
	if (scene->ambient.present == true)
		return (print_err_msg(" Ambient 'A' defined more than once"));
	scene->n_ambient += 1;
	cursor = skip_spaces(rest_of_line);
	if (parse_ambient(&cursor, &color, &ratio_value) == 1)
		return (1);
	scene->ambient.intensity = ratio_value;
	scene->ambient.color = color;
	scene->ambient.present = true;
	return (0);
}
