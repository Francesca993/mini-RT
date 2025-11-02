/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 16:25:15 by francesca         #+#    #+#             */
/*   Updated: 2025/10/31 12:10:46 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Sphere:
sp 0.0,0.0,20.6 12.6 10,0,255
∗ identifier: sp
∗ x, y, z coordinates of the sphere center: 0.0,0.0,20.6
∗ the sphere diameter: 12.6
∗ R,G,B colors in the range [0-255]: 10, 0, 255
*/
static inline int	sphere(const char **pcursor, t_vector *point,
		double *diameter_value, t_color *color)
{
	const char	*cursor;

	cursor = skip_spaces(*pcursor);
	if (!parse_vec3(&cursor, point, 1.0))
		return (print_err_msg("Invalid sphere position (expected x,y,z"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missing Diameter (expected [0.0])"));
	if (!parse_double(&cursor, diameter_value))
		return (print_err_msg("Invalid diameter format"));
	if (*diameter_value <= 0.0)
		return (print_err_msg("Negative diameter value"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missing 'sp' color (expected R,G,B)"));
	if (!parse_rgb(&cursor, color))
		return (print_err_msg("invalid Color format 'sp'(expected R,G,B)"));
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Token extra after color in 'sp'"));
	return (0);
}

int	parse_sphere(t_scene *scene, char *rest_of_line)
{
	const char	*cursor;
	t_vector	point;
	double		diameter_value;
	t_color		color;
	t_figures	payload;

	scene->n_spheres += 1;
	if (scene == NULL || rest_of_line == NULL)
	{
		return (err_msg("Error\nSPHERE", scene->n_spheres,
				"Missing parameters"));
	}
	cursor = skip_spaces(rest_of_line);
	if (sphere(&cursor, &point, &diameter_value, &color) == 1)
		return (err_msg("SPHERE", scene->n_spheres, ""), 1);
	payload.sphere.center = point;
	payload.sphere.radius = diameter_value;
	payload.sphere.color = color;
	if (object_list_append(scene, SPHERE, payload) != 0)
		return (1);
	return (0);
}
