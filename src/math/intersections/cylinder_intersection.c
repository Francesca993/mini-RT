/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 18:37:14 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 13:08:14 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Questa funzione controlla se un punto si trova dentro 
il cerchio di base del cilindro cioè il tondo sopra o sotto
*/
//static bool	within_cylinder_radius(const t_ray *ray, double t)
static bool	within_cylinder_radius(const t_ray *ray,
						double t, const t_shape *shape)
{
	double	x;
	double	z;

	x = ray->origin.x + ray->dir.x * t;
	z = ray->origin.z + ray->dir.z * t;
	if ((x * x + z * z) <= shape->props.radius_squared)
		return (true);
	return (false);
}

/*
Controlla se il raggio colpisce uno dei due tappi del cilindro 
cioè le facce sopra e sotto
*/
static bool	check_cylinder_caps(const t_ray *ray, t_shape *shape,
		t_intersections *xs)
{
	bool	intersected;
	double	t;

	intersected = false;
	if (fabs(ray->dir.y) > EPSILON)
	{
		t = (-(shape->props.height / 2) - ray->origin.y) / ray->dir.y;
		if (within_cylinder_radius(ray, t, shape))
		{
			xs->arr[xs->count].time = t;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
		t = ((shape->props.height / 2) - ray->origin.y) / ray->dir.y;
		if (within_cylinder_radius(ray, t, shape))
		{
			xs->arr[xs->count].time = t;
			xs->arr[xs->count].shape = shape;
			xs->count++;
			intersected = true;
		}
	}
	return (intersected);
}

/*
controlla se il raggio entra o esce dal corpo laterale 
del cilindro cioè la parte curva,
Tiene solo i punti che stanno dentro i limiti verticali del cilindro
tra -height/2 e +height/2
*/
static bool	add_cylinder_intersections(t_shape *shape, const t_ray *ray,
		t_intersections *xs, double *ts)
{
	double	y0;
	double	y1;
	bool	intersected;

	intersected = false;
	if (ts[0] > ts[1])
		ft_swapd(&ts[0], &ts[1]);
	y0 = ray->origin.y + ts[0] * ray->dir.y;
	if (y0 > (-shape->props.height / 2) && y0 < (shape->props.height / 2))
	{
		xs->arr[xs->count].time = ts[0];
		xs->arr[xs->count].shape = shape;
		xs->count++;
		intersected = true;
	}
	y1 = ray->origin.y + ts[1] * ray->dir.y;
	if (y1 > (-shape->props.height / 2) && y1 < (shape->props.height / 2))
	{
		xs->arr[xs->count].time = ts[1];
		xs->arr[xs->count].shape = shape;
		xs->count++;
		intersected = true;
	}
	return (intersected);
}
/*
funzione principale che viene chiamata dal ray tracer quando 
vuole sapere se raggio colpisce il cilindro e dpove 
*/

bool	intersect_cylinder(const t_ray *ray, t_shape *shape,
	t_intersections *xs)
{
	bool	intersected;
	double	a;
	double	b;
	double	ts[2];
	double	discriminant;

	intersected = check_cylinder_caps(ray, shape, xs);
	a = ray->dir.x * ray->dir.x + ray->dir.z * ray->dir.z;
	if (fabs(a) < 0.0001)
		return (intersected);
	b = 2 * ray->dir.x * ray->origin.x + 2 * ray->dir.z * ray->origin.z;
	discriminant = b * b - 4 * a * (ray->origin.x * ray->origin.x
			+ ray->origin.z * ray->origin.z - shape->props.radius_squared);
	if (discriminant < 0)
		return (intersected);
	a *= 2;
	b *= -1;
	discriminant = sqrt(discriminant);
	ts[0] = (b - discriminant) / (a);
	ts[1] = (b + discriminant) / (a);
	if (add_cylinder_intersections(shape, ray, xs, ts))
		intersected = true;
	return (intersected);
}

//NOTE PER ME:
/*
L’equazione del cilindro (centrato sull’asse Y) è
x² + z² = 1
Quando ci metti dentro la formula del raggio (x = origin.x + dir.x * t)
ottieni un’equazione di secondo grado in t:
a·t² + b·t + c = 0
che risolvi con il discriminante (b² - 4ac)
Se discriminant < 0 → nessuna intersezione, altrimenti 
calcola i due punti di intersezione t0, t1
*/