/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:31:04 by francesca         #+#    #+#             */
/*   Updated: 2025/10/31 12:24:47 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
 Sintax expected Cylinder:
cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255
∗ identifier: cy
∗ x, y, z coordinates of the center of the cylinder: 50.0,0.0,20.6
∗ 3D normalized vector of axis of cylinder, in the range [-1,1] for each x, y,
z axis: 0.0,0.0,1.0
∗ the cylinder diameter: 14.2
∗ the cylinder height: 21.42
∗ R, G, B colors in the range [0,255]: 10, 0, 255
*/
static inline int	cylinder(const char **pcursor, t_vector *axis,
		t_vector *base, double *radius)
{
	const char	*cursor;

	cursor = skip_spaces(*pcursor);
	if (!parse_vec3(&cursor, base, 1.0))
		return (print_err_msg("Invalid cylinder position (expected x,y,z)"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missing cylinder axis normal vector"));
	if (!parse_vec3(&cursor, axis, 0.0))
		return (print_err_msg("Invalid cylinder axis normal vector"));
	if (!check_vec3direction(axis))
		return (1);
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missing cylinder diameter"));
	if (!parse_double(&cursor, radius))
		return (print_err_msg("Invalid 'cy' diameter format"));
	if (*radius <= 0.0)
		return (print_err_msg("Cylinder diameter must be > 0"));
	cursor = skip_spaces(cursor);
	*pcursor = cursor;
	return (0);
}

static inline int	cylinder_2(const char **pcursor, double *height,
		t_color *color)
{
	const char	*cursor;

	cursor = skip_spaces(*pcursor);
	if (*cursor == '\0')
		return (print_err_msg("Missing cylinder height"));
	if (!parse_double(&cursor, height))
		return (print_err_msg("Invalid 'cy' height format"));
	if (*height <= 0.0)
		return (print_err_msg("Cylinder height must be > 0"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missing cylinder color"));
	if (!parse_rgb(&cursor, color))
		return (print_err_msg("Invalid 'cy' color format"));
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Token extra after color in 'cy'"));
	return (0);
}

int	parse_cylinder(t_scene *scene, char *rest_of_line)
{
	const char	*cursor;
	t_figures	payload;

	if (!scene || !rest_of_line)
		return (err_msg("Error\nCYLINDER", 0, "Missing parameters"));
	scene->n_cylinders += 1;
	cursor = skip_spaces(rest_of_line);
	if (cylinder(&cursor,
			&payload.cylinder.axis,
			&payload.cylinder.base,
			&payload.cylinder.radius) == 1)
		return (err_msg("CYLINDER", scene->n_cylinders, ""));
	if (cylinder_2(&cursor,
			&payload.cylinder.height,
			&payload.cylinder.color) == 1)
		return (err_msg("CYLINDER", scene->n_cylinders, ""));
	if (object_list_append(scene, CYLINDER, payload) != 0)
		return (1);
	return (0);
}

/*
int	parse_cylinder(t_scene *scene, char *rest_of_line)
{
	const char	*cursor;
	t_vector	base;
	t_vector	axis;
	double		radius;
	double		height;
	t_color		color;
	t_figures	payload;

	scene->n_cylinders += 1;
	if (scene == NULL || rest_of_line == NULL)
		return (err_msg("Error\nCYLINDER", scene->n_cylinders,
				"Missing parameters"));
	cursor = skip_spaces(rest_of_line);
	if (cylinder(&cursor, &axis, &base, &radius) == 1)
		return (err_msg("CYLINDER", scene->n_cylinders, ""));
	if (cylinder_2(&cursor, &height, &color) == 1)
		return (err_msg("CYLINDER", scene->n_cylinders, ""));
	payload.cylinder.base = base;
	payload.cylinder.axis = axis;
	payload.cylinder.color = color;
	payload.cylinder.radius = radius;
	payload.cylinder.height = height;
	if (object_list_append(scene, CYLINDER, payload) != 0)
		return (1);
	return (0);
}
*/