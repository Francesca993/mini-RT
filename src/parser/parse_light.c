/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:37:23 by francesca         #+#    #+#             */
/*   Updated: 2025/10/31 12:13:37 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Sintax expected:
L -40.0,50.0,0.0    0.6    10,0,255
∗ identifier: L
∗ x, y, z coordinates of the light point: -40.0,50.0,0.0
∗ the light brightness ratio in the range [0.0,1.0]: 0.6
∗ (unused in mandatory part) R, G, B colors in the range [0-255]: 10, 0,
255
*/

static inline int	parse_light(const char **pcursor, t_vector *position_out,
		double *ratio_out, t_color *color_out)
{
	const char	*cursor;

	if (!pcursor || !*pcursor || !position_out || !ratio_out || !color_out)
		return (print_err_msg("Missing parameters for 'L'"));
	cursor = skip_spaces(*pcursor);
	if (!parse_vec3(&cursor, position_out, 1.0))
		return (print_err_msg("Light Position invalid(expected x,y,z )"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Light brightness <ratio> missing ([0.0,1.0])"));
	if (!parse_double(&cursor, ratio_out))
		return (print_err_msg("Invalid ratio format for 'L'"));
	if (*ratio_out < 0.0 || *ratio_out > 1.0)
		return (print_err_msg("Ratio of “L” out of range [0..1]"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missing colour of 'L' (expected R,G,B)"));
	if (!parse_rgb(&cursor, color_out))
		return (print_err_msg("Invalid colour format for 'L'(expected R,G,B)"));
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Extra token after the colour in 'L'"));
	*pcursor = cursor;
	return (0);
}

int	parse_light_line(t_scene *scene, char *rest_of_line)
{
	const char	*cursor;
	t_vector	position_value;
	double		light_ratiovalue;
	t_color		color;

	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Missing parameter for 'L'"));
	if (scene->n_lights >= 1 || scene->lights[0].present == true)
		return (print_err_msg("Light “L” defined more than once"));
	cursor = skip_spaces(rest_of_line);
	if (parse_light(&cursor, &position_value, &light_ratiovalue, &color) == 1)
		return (1);
	scene->lights[0].position = position_value;
	scene->lights[0].intensity = light_ratiovalue;
	scene->lights[0].color = color;
	scene->lights[0].present = true;
	scene->n_lights += 1;
	return (0);
}
