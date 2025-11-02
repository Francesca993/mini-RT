/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   look_at.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 14:25:23 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 13:26:43 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	sphere_lookat_pos(t_vector *cam_to_object, t_look_at *look_at,
		t_shape *shape)
{
	t_vector	*final_pos;

	final_pos = &look_at->final_pos;
	if (cam_to_object->x < 0)
		final_pos->x = -5 * shape->props.radius + shape->origin.x;
	else
		final_pos->x = 5 * shape->props.radius + shape->origin.x;
	if (cam_to_object->z < 0)
		final_pos->z = -5 * shape->props.radius + shape->origin.z;
	else
		final_pos->z = 5 * shape->props.radius + shape->origin.z;
	final_pos->y = shape->origin.y;
	final_pos->w = 1;
}
/*
cylinder_lookat_pos() calcola dove deve andare la camera 
per guardare bene un cilindro
Guarda dove si trova la camera rispetto al cilindro cam_to_object
Posiziona la camera davanti o dietro al cilindro asse X e Z
Mantiene la stessa altezza asse Y
Imposta quella posizione come look_at->final_pos
*/

void	cylinder_lookat_pos(t_vector *cam_to_object, t_look_at *look_at,
		t_shape *shape)
{
	t_vector	*final_pos;

	final_pos = &look_at->final_pos;
	if (cam_to_object->x < 0)
		final_pos->x = -2 * max(shape->props.height, shape->props.radius)
			+ shape->origin.x;
	else
		final_pos->x = 2 * max(shape->props.height, shape->props.radius)
			+ shape->origin.x;
	if (cam_to_object->z < 0)
		final_pos->z = -2 * max(shape->props.height, shape->props.radius)
			+ shape->origin.z;
	else
		final_pos->z = 2 * max(shape->props.height, shape->props.radius)
			+ shape->origin.z;
	final_pos->y = shape->origin.y;
	final_pos->w = 1;
}

void	look_at(t_scene *scene, t_shape *shape)
{
	t_vector	cam_to_object;

	sub_vec(&cam_to_object, &shape->origin, &scene->cam.pos);
	negate_vec(&cam_to_object, &cam_to_object);
	if (vec_magnitude(&cam_to_object) < 2)
		return ;
	if (shape->type == SPHERE)
		sphere_lookat_pos(&cam_to_object, &scene->look_at, shape);
	if (shape->type == CYLINDER)
		cylinder_lookat_pos(&cam_to_object, &scene->look_at, shape);
	sub_vec(&scene->look_at.final_dir, &shape->origin,
		&scene->look_at.final_pos);
	normalize_vec(&scene->look_at.final_dir);
	scene->look_at.current_dir = scene->cam.dir;
	scene->look_at.initial_orientation = scene->cam.dir;
	scene->look_at.step_num = 0;
	sub_vec(&scene->look_at.pos_diff, &scene->look_at.final_pos,
		&scene->cam.pos);
	sub_vec(&scene->look_at.dir_diff, &scene->look_at.final_dir,
		&scene->cam.dir);
	scene->look_at.step_amount = (vec_magnitude(&scene->look_at.pos_diff)
			+ vec_magnitude(&scene->look_at.dir_diff)) + 2;
	scene->look_at.trigger = true;
}
