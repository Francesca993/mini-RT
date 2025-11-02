/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:35:09 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 17:39:36 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	populate_shapes_array(t_scene *scene)
{
	t_objnode	*node;
	int			i;

	node = scene->obj_head;
	i = 0;
	while (node && i < MAX_SHAPES)
	{
		if (node->type == SPHERE)
			fill_sphere(&scene->shapes[i], &node->fig.sphere);
		else if (node->type == PLANE)
			fill_plane(&scene->shapes[i], &node->fig.plane);
		else if (node->type == CYLINDER)
			fill_cylinder(&scene->shapes[i], &node->fig.cylinder);
		else
			printf("[Tipo sconosciuto]\n");
		identity_matrix(&scene->shapes[i].added_rots);
		identity_matrix(&scene->shapes[i].transf);
		identity_matrix(&scene->shapes[i].inv_transf);
		identity_matrix(&scene->shapes[i].norm_transform);
		node = node->next;
		i++;
	}
	scene->n_shapes = i;
}

void	fill_sphere(t_shape *shape, t_sphere *sphere)
{
	shape->type = SPHERE;
	shape->origin = sphere->center;
	default_shape_props(&shape->props);
	shape->props.radius = sphere->radius;
	shape->props.radius_squared = pow(sphere->radius, 2);
	shape->props.color = sphere->color;
}

void	fill_plane(t_shape *shape, t_plane *plane)
{
	shape->type = PLANE;
	shape->origin = plane->point;
	shape->orientation = plane->normal;
	normalize_vec(&shape->orientation);
	default_shape_props(&shape->props);
	shape->props.color = plane->color;
}

void	fill_cylinder(t_shape *shape, t_cylinder *cylinder)
{
	shape->type = CYLINDER;
	shape->origin = cylinder->base;
	shape->orientation = cylinder->axis;
	normalize_vec(&shape->orientation);
	default_shape_props(&shape->props);
	shape->props.radius = cylinder->radius;
	shape->props.height = cylinder->height;
	shape->props.color = cylinder->color;
	shape->props.radius_squared = pow(cylinder->radius, 2);
}
