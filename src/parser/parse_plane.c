/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:11:55 by francesca         #+#    #+#             */
/*   Updated: 2025/11/02 14:21:32 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
∗ identifier: pl
∗ x, y, z coordinates of a point in the plane: 0.0,0.0,-10.0
∗ 3D normalized normal vector, in the range [-1,1] for each x, y, z axis:
0.0,1.0,0.0
∗ R,G,B colors in the range [0-255]: 0,0,225
*/
static inline int plane(const char **pcursor, t_vector *point, t_vector *normal, t_color *color)
{
	const char *cursor;

	cursor = skip_spaces(*pcursor);
	
	if (!parse_vec3(&cursor, point, 1.0))
		return (print_err_msg("Invalid position"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("3D normalized normal vector mancante (atteso nx,ny,nz)"));
	if (!parse_vec3(&cursor, normal, 0.0))
		return (print_err_msg("missing 3D normalized normal vector"));
	if (!check_vec3direction(normal))
		return (print_err_msg("3D normalized normal vector out range [-1,1]"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missing color"));
	if (!parse_rgb(&cursor, color))
		return (print_err_msg("Invalid format color"));
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg ("Token extra after color."));
	return (0);
}

int parse_plane(t_scene *scene, char *rest_of_line)
{
    const char	*cursor;
	t_vector	point;
	t_vector	normal;
    t_figures   payload;
    t_color		color;

	scene->n_planes += 1;
	
	if (scene == NULL || rest_of_line == NULL)
		return (err_msg("Error\nPLANE", scene->n_planes, "Missing parameters"));
	cursor = skip_spaces(rest_of_line);
	if (plane(&cursor, &point, &normal, &color) == 1)
		return (err_msg("PLANE", scene->n_planes, ""), 1);
    payload.plane.point = point;
    payload.plane.normal = normal;
    payload.plane.color = color;
    if (object_list_append(scene, PLANE, payload) != 0)
	{
        return (1);
	}
	return (0);
}
