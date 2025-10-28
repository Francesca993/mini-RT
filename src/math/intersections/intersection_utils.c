/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:03:10 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/24 14:29:38 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
prende una collisione e calcola tutte le cose utili per luci e riflessioni
*/
/*void	prepare_computations(t_intersection *intersection,
			t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);                    // Prende il raggio, calcola la posizione al tempo t dell’intersezione, e la salva come punto di impatto nello spazio 3D
	intersection->normal = normal_at(intersection->shape, &intersection->point);	// Chiama una funzione che restituisce la normale dell’oggetto nel punto colpito
	negate_vec(&intersection->eye, &ray->dir);										// L’eye vector è il verso opposto alla direzione del raggio, per illuminare dobbiamo sapere dall’oggetto verso la camera CONTRARIO
	intersection->eye.w = 0;														// 0 perché è un vettore, non un punto
	intersection->inside = false;													// Checj se l'intersezione è interna, se il raggio partiva da dentro 
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	scale_vec(&intersection->over_point, &intersection->normal, EPSILON);			// Calcola over_point punto leggermente spostato fuori dall’oggetto, in direzione della normale per evitare problemi di precisione
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect_vector(&intersection->reflect_vec, &ray->dir,							// Usa la direzione del raggio e la normale per calcolare in che direzione rimbalzerebbe la luce
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
	min_time = INFINITY;											// significa “valore molto grande”
	while (i < xs->count)											// mi scorro tutte le intersezioni cercando quellq con il time minore , cioè piu vicina
	{
		if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time)
		{
			min_time = xs->arr[i].time;
			idx = i;
		}
		i++;
	}
	if (min_time == INFINITY)
		return (NULL);												// Se nessuna intersezione è valida, ritorna NULL
	return (&xs->arr[idx]);											// Altrimenti, restituisce l’intersezione migliore
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
Serve per trasformare un raggio da mondo a object space, usando la matrice inversa della shape
Si moltiplicano sia il punto di partenza (origin) che la direzione (dir) per la matrice inversa della shape
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
void prepare_computations(t_intersection *intersection, t_ray *ray)
{
    ray_position(&intersection->point, ray, intersection->time);  // Calcola la posizione dell'intersezione
    printf("Intersection Point: (%f, %f, %f)\n", intersection->point.x, intersection->point.y, intersection->point.z);  // Debug

    intersection->normal = normal_at(intersection->shape, &intersection->point);  // Ottieni la normale
    printf("Normal at intersection: (%f, %f, %f)\n", intersection->normal.x, intersection->normal.y, intersection->normal.z);  // Debug

    negate_vec(&intersection->eye, &ray->dir);  // Calcola l'eye vector (verso opposto del raggio)
    intersection->eye.w = 0;  // 0 perché è un vettore
    printf("Eye vector: (%f, %f, %f)\n", intersection->eye.x, intersection->eye.y, intersection->eye.z);  // Debug

    intersection->inside = false;  // Verifica se l'intersezione è interna
    if (dot_product(&intersection->normal, &intersection->eye) < 0)
    {
        intersection->inside = true;
        negate_vec(&intersection->normal, &intersection->normal);  // Inverti la normale se dentro
    }

    printf("Inside: %d\n", intersection->inside);  // Debug

    scale_vec(&intersection->over_point, &intersection->normal, EPSILON);  // Calcola over_point
    add_vec(&intersection->over_point, &intersection->point, &intersection->over_point);
    printf("Over-point: (%f, %f, %f)\n", intersection->over_point.x, intersection->over_point.y, intersection->over_point.z);  // Debug

    reflect_vector(&intersection->reflect_vec, &ray->dir, &intersection->normal);  // Calcola il vettore di riflessione
    printf("Reflection Vector: (%f, %f, %f)\n", intersection->reflect_vec.x, intersection->reflect_vec.y, intersection->reflect_vec.z);  // Debug
}
/*t_intersection *hit(t_intersections *xs)
{
    double min_time;
    int i;
    int idx;

    i = 0;
    idx = 0;
    min_time = INFINITY;  // Inizializza il tempo minimo a infinito
    while (i < xs->count)  // Scorri tutte le intersezioni
    {
        printf("Checking intersection %d: time = %f\n", i, xs->arr[i].time);  // Debug

        if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time)  // Confronta il tempo per trovare il più vicino
        {
            min_time = xs->arr[i].time;
            idx = i;
        }
        i++;
    }
    if (min_time == INFINITY)
    {
        printf("No valid intersection found\n");  // Debug se non ci sono intersezioni valide
        return NULL;
    }

    printf("Closest intersection at time: %f, index: %d\n", min_time, idx);  // Debug
    return &xs->arr[idx];  // Restituisce l'intersezione più vicina
}*/
void ray_position(t_vector *pos, const t_ray *ray, double time)
{
    pos->x = ray->dir.x * time + ray->origin.x;
    pos->y = ray->dir.y * time + ray->origin.y;
    pos->z = ray->dir.z * time + ray->origin.z;
    pos->w = 1;
    printf("Ray Position: (%f, %f, %f)\n", pos->x, pos->y, pos->z);  // Debug
}

void transform_ray(t_ray *transformed_ray, const t_ray *ray, const t_shape *shape)
{
    mat_vec_multiply(&transformed_ray->origin, &shape->inv_transf, &ray->origin);
    mat_vec_multiply(&transformed_ray->dir, &shape->inv_transf, &ray->dir);
    printf("Transformed Ray Origin: (%f, %f, %f)\n", transformed_ray->origin.x, transformed_ray->origin.y, transformed_ray->origin.z);  // Debug
    printf("Transformed Ray Direction: (%f, %f, %f)\n", transformed_ray->dir.x, transformed_ray->dir.y, transformed_ray->dir.z);  // Debug
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
    
    printf("Pixel size: %f, Half width: %f, Half height: %f\n", cam->pixel_size, cam->half_width, cam->half_height);
    printf("World Coordinates (x, y): (%f, %f)\n", world_x, world_y);

    world_point.x = world_x;
    world_point.y = world_y;
    world_point.z = -1;
    world_point.w = 1;

    // Trasformazione delle coordinate mondiali in coordinate fotocamera
    mat_vec_multiply(&pixel, &cam->inv_trans, &world_point);

    // Debug: Verifica i valori di pixel dopo la trasformazione
    printf("World Point: (%f, %f, %f)\n", world_point.x, world_point.y, world_point.z);
    printf("Pixel After Transformation: (%f, %f, %f)\n", pixel.x, pixel.y, pixel.z);

    // Calcolo dell'origine del raggio
    ft_bzero(&center, sizeof(t_vector));
    center.w = 1;
    mat_vec_multiply(&ray->origin, &cam->inv_trans, &center);

    // Debug: Verifica la posizione dell'origine del raggio dopo la trasformazione
    printf("Ray Origin After Transformation: (%f, %f, %f)\n", ray->origin.x, ray->origin.y, ray->origin.z);

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
        printf("Ray Direction After Normalization: (%f, %f, %f)\n", ray->dir.x, ray->dir.y, ray->dir.z);
    } else {
        // Messaggio di avviso se la direzione del raggio è nulla
        printf("Warning: Ray direction is zero, skipping normalization\n");
    }
}*/
