/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook_move.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 20:09:01 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 11:50:45 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include "../include/mathematical.h"

void	move_object_fwd(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.w)
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, 0.2);
	if (scene->keys_held.s)
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, -0.2);
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	move_object_h(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.a)
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta + M_PI_2, 0.2);
	if (scene->keys_held.d)
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta - M_PI_2, 0.2);
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	move_object_v(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.w)
		offset.y = 0.2;
	if (scene->keys_held.s)
		offset.y = -0.2;
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	reset_look_at(t_scene *scene)
{
	if (scene->cam.dir.x > 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x);
	else if (scene->cam.dir.x < 0 && scene->cam.dir.z >= 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x) + M_PI;
	else if (scene->cam.dir.x < 0 && scene->cam.dir.z < 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x) - M_PI;
	scene->cam.phi = acos(scene->cam.dir.y);
	scene->look_at.trigger = false;
	scene->look_at.step_num = 0;
}
