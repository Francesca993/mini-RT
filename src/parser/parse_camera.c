/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:12:18 by francesca         #+#    #+#             */
/*   Updated: 2025/10/30 21:28:25 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Sintax expected:
C -50.0,0,20 0,0,1 70
∗ identifier: C
∗ x, y, z coordinates of the viewpoint: -50.0,0,20
∗ 3D normalized orientation vector, in the range [-1,1]
for each x, y, z axis: 0.0,0.0,1.0
∗ FOV: Horizontal field of view in degrees in the range [0,180]: 70
*/
static inline int	parse_camera(const char **pcursor, t_vector *position_value,
		t_vector *direction_value, int *fov_degrees_integer)
{
	const char	*cursor;

	cursor = skip_spaces(*pcursor);
	if (!parse_vec3(&cursor, position_value, 1.0))
		return (print_err_msg("Invalid Camera position (expected x,y,z)"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missied Camera direction (expected nx,ny,nz)"));
	if (!parse_vec3(&cursor, direction_value, 0.0))
		return (print_err_msg("Invalid Camera direction (expected nx,ny,nz)"));
	if (!check_vec3direction(direction_value))
		return (err_msg("Camera", 1, ""));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Missied Camera FOV"));
	if (!parse_int(&cursor, fov_degrees_integer))
		return (print_err_msg("Invalid Camera FOV (expected int)"));
	if (*fov_degrees_integer < 0 || *fov_degrees_integer > 180)
		return (print_err_msg("Camera FOV out of range [0..180]"));
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Token extra after camera FOV"));
	return (0);
}

int	parse_camera_line(t_scene *scene, char *rest_of_line)
{
	const char	*cursor;
	t_vector	position_value;
	t_vector	direction_value;
	int			fov_degrees_integer;

	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Missing Parameters for 'C'"));
	if (scene->cam.present == true)
		return (print_err_msg("Camera 'C' definided  more than once"));
	cursor = skip_spaces(rest_of_line);
	if (parse_camera(&cursor, &position_value, &direction_value,
			&fov_degrees_integer) == 1)
		return (1);
	scene->cam.pos = position_value;
	scene->cam.dir = direction_value;
	scene->cam.fov_deg = fov_degrees_integer;
	scene->cam.present = true;
	scene->n_camera += 1;
	return (0);
}
