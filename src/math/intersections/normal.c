/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:04:37 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 16:18:09 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
/*
calcola la direzione della normale cioè la direzione perpendicolare alla 
superficie nel punto dove il raggio ha colpito il cilindro
e la restituisce trasformata nello spazio del mondo
normale diversa per le caps e per i lati
caps = punta in alto o in basso (cioè verso ±Y)
lati = la normale punta radialmente verso l’esterno, come i raggi di una ruota
*/
t_vector	cylinder_normal(const t_shape *shape, const t_vector *itx_point)
{
	double		distance;
	t_vector	normal;
	t_vector	point;
	t_vector	world_normal;

	mat_vec_multiply(&point, &shape->inv_transf, itx_point);
	point.w = 0;
	distance = point.x * point.x + point.z * point.z;
	ft_bzero(&normal, sizeof(t_vector));
	if (distance < 1 && (point.y >= (shape->props.height / 2) - EPSILON))
		normal.y = 1;
	else if (distance < 1 && (point.y <= -(shape->props.height / 2) + EPSILON))
		normal.y = -1;
	else
	{
		normal.x = point.x;
		normal.z = point.z;
		normalize_vec(&normal);
	}
	mat_vec_multiply(&world_normal, &shape->norm_transf, &normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}

/*
Questa funzione calcola la normale di un piano in un certo punto,
Serve per calcolare riflessi, luci, ombre

*/
//t_vector	plane_normal(const t_shape *shape, const t_vector *itx_point)
/*t_vector	plane_normal(const t_shape *shape)
{
	t_vector	object_normal;
	t_vector	world_normal;

	object_normal.x = 0;
	object_normal.y = 1;
	object_normal.z = 0;
	object_normal.w = 0;
	mat_vec_multiply(&world_normal, &shape->norm_transform, &object_normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}*/

t_vector	plane_normal(const t_shape *shape)
{
	t_vector	world_normal;

	world_normal = shape->orientation;
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}

/*t_vector	sphere_normal(t_vector *normal, const t_shape *shape,
				const t_vector *itx_point)
{
	t_vector	world_normal;

	mat_vec_multiply(normal, &shape->inv_transf, itx_point);
	normalize_vec(normal);
	normal->w = 0;
	mat_vec_multiply(&world_normal, &shape->norm_transform, normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}*/

t_vector	sphere_normal(t_vector *out, const t_shape *sphere,
									const t_vector *point)
{
	sub_vec(out, point, &sphere->origin);
	normalize_vec(out);
	out->w = 0;
	return (*out);
}

/*
Di che oggetto si tratta?
Sfera → chiama sphere_normal()
 → plane_normal()
*/
t_vector	normal_at(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	normal;
	t_vector	world_normal;

	if (shape->type == SPHERE)
		return (sphere_normal(&normal, shape, itx_point));
	else if (shape->type == PLANE)
		return (plane_normal(shape));
	else if (shape->type == CYLINDER)
		return (cylinder_normal(shape, itx_point));
	mat_vec_multiply(&world_normal, &shape->norm_transform, &normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}
