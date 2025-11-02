/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:29:04 by francesca         #+#    #+#             */
/*   Updated: 2025/11/02 14:16:24 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <string.h>

/*
Check if the file has the extenctrion .rt
*/
int has_rt_extension(const char *path)
{
	size_t len;

	if (path == NULL)
		return (0);
	len = ft_strlen(path);
	if (len < 3)
		return (0);
	if (ft_strcmp(path + len - 3, ".rt") == 0)
		return (1);
	return (0);
}

/*
Return 1 if id is one of the tokens allowed in mandatory.
*/
int is_valid_identifier(const char *id)
{
    if (!id || !*id) return 0;
    if (ft_strcmp(id, "A") == 0)  return 1;
    if (ft_strcmp(id, "C") == 0)  return 1;
    if (ft_strcmp(id, "L") == 0)  return 1;
    if (ft_strcmp(id, "sp") == 0) return 1;
    if (ft_strcmp(id, "pl") == 0) return 1;
    if (ft_strcmp(id, "cy") == 0) return 1;
    return 0;
}

/*
Verify that the camera’s orientation vector (x, y, z) is normalized
and that each component is in the range [-1, 1]..
*/
int check_vec3direction(t_vector *direction_value)
{
	if (direction_value->x < -1.0 || direction_value->x > 1.0
		|| direction_value->y < -1.0 || direction_value->y > 1.0
		|| direction_value->z < -1.0 || direction_value->z > 1.0)
		{
			print_err_msg("Normalized Vector axis of cylinder out range [-1,1]");
			return (0);
		}
	if (direction_value->x == 0 && direction_value->y == 0 && direction_value->z == 0)
	{
		print_err_msg("Normalized Vector null");
		return (0);
	}
	normalize_vec(direction_value);
    return (1);
}
/* 
Check for presence and uniqueness.
Minimum required: A, C, L.
No duplicates (subjects: uppercase tokens must appear exactly once).
At least one object must be present.
 */
int check_presence(t_scene *scene)
{
	if (scene == NULL)
		return (print_err_msg("Error: Invalid scene"));
	if (scene->ambient.present == false || scene->n_ambient == 0)
		return (print_err_msg("Error: Missing 'A' ambient"));
	if (scene->cam.present == false || scene->n_camera == 0)
		return (print_err_msg("Error: Missing 'C' camera"));
	if (scene->lights->present == false || scene->n_lights == 0)
		return (print_err_msg("Error: Missing 'L' Light "));
	if (scene->n_ambient > 1)
		return (print_err_msg("Error: Ambient 'A' defined more than once"));
	if (scene->n_camera > 1)
		return (print_err_msg("Error: Camera 'C' defined more than once"));
	if (scene->n_lights > 1)
		return (print_err_msg("Error: Light 'L' defined more than once"));
	if (scene->n_planes + scene->n_spheres + scene->n_cylinders == 0)
		return(print_err_msg("Error: There must be at least one 'cy', 'pl' or sp"));
	return (0);
}
