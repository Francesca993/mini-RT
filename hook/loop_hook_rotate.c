/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook_rotate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 20:09:10 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 11:59:26 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/* Rotazioni oggetto */
void	rotate_x(t_scene *scene, t_mat4 *rot_mat, double deg)
{
	t_mat4		rot;
	t_vector	ax;
	t_vector	up;
	t_mat4		mat_copy;

	up = (t_vector){0, 1, 0, 0};
	cross_product(&ax, &up, &scene->cam.dir);
	normalize_vec(&ax);
	if (scene->keys_held.down)
		axis_angle(&rot, &ax, -deg);
	else
		axis_angle(&rot, &ax, deg);
	ft_memcpy(&mat_copy, rot_mat, sizeof(t_mat4));
	mat_multiply(rot_mat, &rot, &mat_copy);
}

void	rotate_y(t_scene *scene, t_mat4 *rot_mat, double deg)
{
	t_mat4	rot;
	t_mat4	mat_copy;

	if (scene->keys_held.left)
		rotation_matrix_y(&rot, deg);
	else
		rotation_matrix_y(&rot, -deg);
	ft_memcpy(&mat_copy, rot_mat, sizeof(t_mat4));
	mat_multiply(rot_mat, &rot, &mat_copy);
}

void	rotate_z(t_scene *scene, t_mat4 *rot_mat, double deg)
{
	t_mat4	rot;
	t_mat4	mat_copy;

	if (scene->keys_held.r == true)
		axis_angle(&rot, &scene->cam.dir, deg);
	else
		axis_angle(&rot, &scene->cam.dir, -deg);
	ft_memcpy(&mat_copy, rot_mat, sizeof(t_mat4));
	mat_multiply(rot_mat, &rot, &mat_copy);
}

/* Gestisce le rotazioni in base ai tasti freccia */
void	rotate_object(t_scene *scene)
{
	if (scene->keys_held.left || scene->keys_held.right)
		rotate_y(scene, &scene->shapes[scene->shape_idx].added_rots, DEG_TO_RAD
			* 5);
	if (scene->keys_held.up || scene->keys_held.down)
		rotate_x(scene, &scene->shapes[scene->shape_idx].added_rots, DEG_TO_RAD
			* 5);
}

/* Gestione degli spostamenti */
void	transform_object(t_scene *scene)
{
	if (scene->keys_held.w || scene->keys_held.s)
		move_object_fwd(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.a || scene->keys_held.d)
		move_object_h(scene, &scene->shapes[scene->shape_idx]);
	rotate_object(scene);
	if (scene->keys_held.plus || scene->keys_held.minus)
		scale_object(scene, &scene->shapes[scene->shape_idx]);
}
