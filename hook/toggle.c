/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toggle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 20:08:27 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/29 20:08:32 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Controlla se il tasto premuto è un toggle valido qui solo SPACE per edit mode
bool	is_toggle_key(int key, t_scene *scene)
{
	(void)scene;
	return (key == KEY_SPACE);
}

// Attiva/disattiva la modalità edit
void	toggle_edit_mode(int key, t_scene *scene)
{
	(void)scene;
	if (key == KEY_SPACE)
		scene->settings.edit_mode = !scene->settings.edit_mode;
}

// Cambia il tipo della shape in ciclo SPHERE -> CYLINDER -> PLANE -> SPHERE
void	toggle_shape_properties(t_shape *shape)
{
	if (shape->type == SPHERE)
		shape->type = CYLINDER;
	else if (shape->type == CYLINDER)
		shape->type = PLANE;
	else if (shape->type == PLANE)
		shape->type = SPHERE;
}

// Applica reset base e toggle del tipo
void	toggle_shape(t_scene *scene)
{
	t_shape	*shape;

	shape = &scene->shapes[scene->shape_idx];
	shape->props.radius = fmax(shape->props.radius, 0.5);
	shape->props.radius_squared = shape->props.radius * shape->props.radius;
	shape->props.height = fmax(shape->props.height, 0.5);
	shape->props.scale.x = shape->props.radius;
	shape->props.scale.y = shape->props.radius;
	shape->props.scale.z = shape->props.radius;
	ft_bzero(&shape->orientation, sizeof(t_vector));
	shape->orientation.y = 1;
	identity_matrix(&shape->added_rots);
	toggle_shape_properties(shape);
}
