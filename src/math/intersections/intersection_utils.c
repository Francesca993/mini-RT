/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:03:10 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 16:51:25 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
prende una collisione e calcola tutte le cose utili per luci e riflessioni
Prende il raggio, calcola la posizione al tempo t dell’intersezione, 
e la salva come punto di impatto nello spazio 3D
Chiama una funzione che restituisce la normale dell’oggetto nel punto colpito
L’eye vector è il verso opposto alla direzione del raggio, 
per illuminare dobbiamo sapere dall’oggetto verso la camera CONTRARIO
Calcola over_point punto leggermente spostato fuori dall’oggetto, 
in direzione della normale per evitare problemi di precisione
Usa la direzione del raggio e la normale per calcolare in che direzione 
rimbalzerebbe la luce
*/
/*void	prepare_computations(t_intersection *intersection,
			t_ray *ray)
{
	ray_position(&intersection->point, ray,
                intersection->time);                    
	intersection->normal = normal_at(intersection->shape, &intersection->point);
	negate_vec(&intersection->eye, &ray->dir);
	intersection->eye.w = 0;
	intersection->inside = false; 
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	scale_vec(&intersection->over_point, &intersection->normal, EPSILON);
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect_vector(&intersection->reflect_vec, &ray->dir,
		&intersection->normal);
}*/

/*
prende la lista di intersezioni e restituisce l'intersezione più vicina con t > 0
ignora quelle dietro alla camera
*/
t_intersection	*hit(t_intersections *xs)
{
	double	min_time;
	int		i;
	int		idx;

	i = 0;
	idx = 0;
	min_time = INFINITY;
	while (i < xs->count)
	{
		if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time)
		{
			min_time = xs->arr[i].time;
			idx = i;
		}
		i++;
	}
	if (min_time == INFINITY)
		return (NULL);
	return (&xs->arr[idx]);
}
/*
Calcola la posizione lungo un raggio al tempo t.
Punto = Origine + t × Direzione 
P = O + t * D
*/
/*void	ray_position(t_vector *pos, const t_ray *ray, double time)
{
	pos->x = ray->dir.x * time + ray->origin.x;
	pos->y = ray->dir.y * time + ray->origin.y;
	pos->z = ray->dir.z * time + ray->origin.z;
	pos->w = 1;
}*/
/*
Serve per trasformare un raggio da mondo a object space, 
usando la matrice inversa della shape
Si moltiplicano sia il punto di partenza (origin) 
che la direzione (dir) per la matrice inversa della shape
*/
/*void	transform_ray(t_ray *transformed_ray, const t_ray *ray,
	const t_shape *shape)
{
	mat_vec_multiply(&transformed_ray->origin, &shape->inv_transf,
		&ray->origin);
	mat_vec_multiply(&transformed_ray->dir, &shape->inv_transf,
		&ray->dir);
}*/

void	ray_from_cam(t_ray *ray, const t_camera *cam, double x, double y)
{
	double		world_x;
	double		world_y;
	t_vector	pixel;
	t_vector	world_point;
	t_vector	center;

	world_x = cam->half_width - (x) * cam->pixel_size;
	world_y = cam->half_height - (y) * cam->pixel_size;
	world_point.x = world_x;
	world_point.y = world_y;
	world_point.z = -1;
	world_point.w = 1;
	mat_vec_multiply(&pixel, &cam->inv_trans, &world_point);
	ft_bzero(&center, sizeof(t_vector));
	center.w = 1;
	mat_vec_multiply(&ray->origin, &cam->inv_trans, &center);
	sub_vec(&ray->dir, &pixel, &ray->origin);
	ray->dir.w = 0;
	normalize_vec(&ray->dir);
}

void	prepare_computations(t_intersection *intersection, t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);
	intersection->normal = normal_at(intersection->shape, &intersection->point);
	negate_vec(&intersection->eye, &ray->dir);
	intersection->eye.w = 0;
	intersection->inside = false;
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	scale_vec(&intersection->over_point, &intersection->normal, EPSILON);
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect_vector(&intersection->reflect_vec, &ray->dir,
		&intersection->normal);
}
/*t_intersection *hit(t_intersections *xs)
{
	double	min_time;
	int		i;
	int		idx;

	i = 0;
	idx = 0;
	min_time = INFINITY;  // Inizializza il tempo minimo a infinito
	while (i < xs->count)  // Scorri tutte le intersezioni
	{
		printf("Checking intersection %d: time = %f\n", i, xs->arr[i].time);
		if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time)
		{
			min_time = xs->arr[i].time;
			idx = i;
		}
		i++;
	}
	if (min_time == INFINITY)
	{
		printf("No valid intersection found\n");
		return (NULL);
	}
	printf("Closest intersection at time: %f, index: %d\n", min_time, idx);
}*/

void	ray_position(t_vector *pos, const t_ray *ray, double time)
{
	pos->x = ray->dir.x * time + ray->origin.x;
	pos->y = ray->dir.y * time + ray->origin.y;
	pos->z = ray->dir.z * time + ray->origin.z;
	pos->w = 1;
}

void	transform_ray(t_ray *transformed_ray, const t_ray *ray,
		const t_shape *shape)
{
	mat_vec_multiply(&transformed_ray->origin, &shape->inv_transf,
		&ray->origin);
	mat_vec_multiply(&transformed_ray->dir, &shape->inv_transf, &ray->dir);
}

/*void ray_from_cam(t_ray *ray, const t_camera *cam, double x, double y)
{
    double world_x;
    double world_y;
    t_vector pixel;
    t_vector world_point;
    t_vector center;

    // Calcolo delle coordinate mondiali
    world_x = cam->half_width - (x) * cam->pixel_size;
    world_y = cam->half_height - (y) * cam->pixel_size;
    
    printf("Pixel size: %f, Half width: %f, Half height: %f\n", 
    cam->pixel_size, cam->half_width, cam->half_height);
    printf("World Coordinates (x, y): (%f, %f)\n", world_x, world_y);

    world_point.x = world_x;
    world_point.y = world_y;
    world_point.z = -1;
    world_point.w = 1;

    // Trasforma delle coordinate mondiali in coordinate fotocamera
    mat_vec_multiply(&pixel, &cam->inv_trans, &world_point);

    // Debug: Verifica i valori di pixel dopo la trasformazione
    printf("World Point: (%f, %f, %f)\n", world_point.x,
    world_point.y, world_point.z);
    printf("Pixel After Transformation: (%f, %f, %f)\n", pixel.x,
    pixel.y, pixel.z);

    // Calcolo dell'origine del raggio
    ft_bzero(&center, sizeof(t_vector));
    center.w = 1;
    mat_vec_multiply(&ray->origin, &cam->inv_trans, &center);

    // Debug: Verifica la pos dell'origine del raggio dopo la trasfor
    printf("Ray Origin After Transformation: (%f, %f, %f)\n", ray->origin.x,
    ray->origin.y, ray->origin.z);

    // Calcolo della direzione del raggio
    sub_vec(&ray->dir, &pixel, &ray->origin);

    // Debug: Verifica la direzione del raggio prima della normalizzazione
    printf("Pixel: (%f, %f, %f), Ray Origin: (%f, %f, %f)\n", 
        pixel.x, pixel.y, pixel.z, ray->origin.x, ray->origin.y, ray->origin.z);

    // Aggiungi il controllo sulla normalizzazione del vettore direzionale
    if (ray->dir.x != 0.0f || ray->dir.y != 0.0f || ray->dir.z != 0.0f) {
        ray->dir.w = 0;  // Imposta w a 0, poiché è un vettore direzionale
        normalize_vec(&ray->dir);
        // Debug: Verifica la direzione del raggio dopo la normalizzazione
        printf("Ray Direction After Normalization: (%f, %f, %f)\n",
        ray->dir.x, ray->dir.y, ray->dir.z);
    } else {
        // Messaggio di avviso se la direzione del raggio è nulla
        printf("Warning: Ray direction is zero, skipping normalization\n");
    }
}*/
