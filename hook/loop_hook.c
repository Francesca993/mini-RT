/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 20:08:48 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 12:14:08 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vector	*sphere_to_xyz(t_vector *vec, double phi, double theta, double r)
{
	vec->x = r * sin(phi) * cos(theta);
	vec->z = r * sin(phi) * sin(theta);
	vec->y = r * cos(phi);
	vec->w = 0;
	return (vec);
}

void	scale_object(t_scene *scene, t_shape *shape)
{
	double	amount;

	amount = 0.04;
	if (scene->keys_held.minus)
		amount *= -1;
	if (shape->type == SPHERE || shape->type == CYLINDER)
	{
		if (shape->props.radius > 0.3 || !scene->keys_held.minus)
			shape->props.radius += amount;
		shape->props.scale.x = shape->props.radius;
		shape->props.scale.y = 1;
		shape->props.scale.z = shape->props.radius;
	}
	shape->props.radius_squared = shape->props.radius * shape->props.radius;
}

void	change_height(t_scene *scene, t_shape *shape)
{
	if (scene->keys_held.plus)
		shape->props.height += 0.04;
	if (scene->keys_held.minus && shape->props.height > 0.2)
		shape->props.height -= 0.04;
}

bool	is_loop_hook_key(t_scene *scene)
{
	return (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
		|| scene->keys_held.d || scene->keys_held.up || scene->keys_held.down
		|| scene->keys_held.left || scene->keys_held.right
		|| scene->keys_held.plus || scene->keys_held.minus
		|| scene->keys_held.r);
}

/*int render_loop(t_scene *scene)
{
	if (scene->settings.edit_mode)
	{
		transform_object(scene);
	}

	if (scene->keys_held.o && scene->settings.edit_mode 
					&& !scene->look_at.trigger)
		look_at(scene, &scene->shapes[scene->shape_idx]);

	if (scene->settings.edit_mode && is_loop_hook_key(scene))
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}

	return 0;
}*/

int	render_loop(t_scene *scene)
{
	static bool	scene_updated = false;

	if (scene->settings.edit_mode)
	{
		transform_object(scene);
		scene_updated = true;
	}
	if (scene->keys_held.o && scene->settings.edit_mode
		&& !scene->look_at.trigger)
	{
		look_at(scene, &scene->shapes[scene->shape_idx]);
	}
	if (scene->settings.edit_mode && is_loop_hook_key(scene))
	{
		if (scene_updated)
		{
			calculate_transforms(scene);
			draw_scene(scene);
			scene_updated = false;
		}
	}
	return (0);
}
