/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:27:58 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 18:03:35 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
funzione da usare per render "completo" e preciso. Disegna pixel per pixel.
array per salvare tutte le intersezioni trovate
inizia dalla prima riga assegnata a questo thread
finché non ha completato la sua zona
percorri tutta la larghezza dell'immagine
resetta colore pixel
calcola e disegna pixel
*/
void	*render_scene(t_worker *worker)
{
	t_intersections	arr;
	int				x;
	int				y;

	y = worker->y_start;
	while (y < worker->y_end)
	{
		x = 0;
		while (x < worker->width)
		{
			set_color(worker, x, y, 0);
			render_pixel(x, y, &arr, worker);
			x += 1;
		}
		y += 1;
	}
	return (NULL);
}

/*
Lancia raggio e calcola colore di un pixel
esegue ray tracing + scrive su immagine
FUNZIONE CENTRALE DEL RAY TRACING
Generazione del raggio
Intersezione con oggetti
Calcolo del colore
Scrittura del colore
*/

t_color	render_pixel(double x, double y, t_intersections *arr, t_worker *worker)
{
	int		shape_idx;
	t_scene	*scene;
	t_ray	ray;
	t_color	color;

	set_color(worker, x, y, 0);
	scene = worker->scene;
	ray_from_cam(&ray, &scene->cam, x + 0.5, y + 0.5);
	shape_idx = -1;
	arr->count = 0;
	while (++shape_idx < scene->n_shapes)
		intersect(&scene->shapes[shape_idx], &ray, arr);
	color = shade_point(arr, scene, &ray);
	set_color(worker, x, y, create_mlx_color(&color));
	return (color);
}

/*void	populate_shapes_array(t_scene *scene)
{
	t_objnode	*node;
	int			i;

	node = scene->obj_head;
	i = 0;
	while (node && i < MAX_SHAPES)
	{
		if (node->type == SPHERE)
		{
			scene->shapes[i].type = SPHERE;
			scene->shapes[i].origin = node->fig.sphere.center;
			default_shape_props(&scene->shapes[i].props);
			scene->shapes[i].props.radius = node->fig.sphere.radius;
			scene->shapes[i].props.radius_squared = pow(node->fig.sphere.radius,
					2);
			scene->shapes[i].props.color = node->fig.sphere.color;
		}
		else if (node->type == PLANE)
		{
			scene->shapes[i].type = PLANE;
			scene->shapes[i].origin = node->fig.plane.point;
			scene->shapes[i].orientation = node->fig.plane.normal;
			normalize_vec(&scene->shapes[i].orientation);
			default_shape_props(&scene->shapes[i].props);
			scene->shapes[i].props.color = node->fig.plane.color;
		}
		else if (node->type == CYLINDER)
		{
			scene->shapes[i].type = CYLINDER;
			scene->shapes[i].origin = node->fig.cylinder.base;
			scene->shapes[i].orientation = node->fig.cylinder.axis;
			normalize_vec(&scene->shapes[i].orientation);
			default_shape_props(&scene->shapes[i].props);
			scene->shapes[i].props.radius = node->fig.cylinder.radius;
			scene->shapes[i].props.height = node->fig.cylinder.height;
			scene->shapes[i].props.color = node->fig.cylinder.color;
			scene->shapes[i].props.radius_squared = pow
				(node->fig.cylinder.radius, 2);
		}
		else
		{
			printf("[Tipo sconosciuto]\n");
		}
		identity_matrix(&scene->shapes[i].added_rots);
		identity_matrix(&scene->shapes[i].transf);
		identity_matrix(&scene->shapes[i].inv_transf);
		identity_matrix(&scene->shapes[i].norm_transform);
		node = node->next;
		i++;
	}
	scene->n_shapes = i;
}*/

/*
Funzione chiamata per disegnare l’intera scena
quella chiamata da MiniRT per iniziare il rendering
inizializza i thread
*/
void	draw_scene(t_scene *scene)
{
	t_worker	workers[NUM_THREADS];

	init_workers(workers, scene);
	run_workers(workers, render_scene);
	mlx_put_image_to_window(scene->disp->mlx, scene->disp->win,
		scene->disp->display_img, 0, 0);
}

/*void	*render_scene(t_worker *worker)
{
	t_intersections	arr;    // array per salvare tutte le intersezioni trovate
	int				x;
	int				y;

	y = worker->y_start;    
		// inizia dalla prima riga assegnata a questo thread
	while (y < worker->y_end)   // finché non ha completato la sua zona
	{
		x = 0;
		while (x < worker->width)   
			// percorri tutta la larghezza dell'immagine
		{
			set_color(worker, x, y, 0);  // resetta colore pixel (opzionale)
			render_pixel(x, y, &arr, worker);  // calcola e disegna pixel
			x += 1;
		}
		y += 1;
	}
	return (NULL);
}*/

/*t_color	render_pixel(double x, double y, t_intersections *arr,
		t_worker *worker)
{
	int		shape_idx;
	t_scene	*scene;
	t_ray	ray;
	t_color	color;

	set_color(worker, x, y, 0);
	scene = worker->scene;
	ray_from_cam(&ray, &scene->cam, x + 0.5, y + 0.5);
	shape_idx = -1;
	arr->count = 0;
	while (++shape_idx < scene->n_shapes)
		intersect(&scene->shapes[shape_idx], &ray, arr);
	color = shade_point(arr, scene, &ray);
	set_color(worker, x, y, create_mlx_color(&color));

	// Debug per la direzione del raggio e il colore calcolato
	printf("Ray origin: (%f, %f, %f), Direction: (%f, %f, %f)\n",
		ray.origin.x, ray.origin.y, ray.origin.z,
		ray.dir.x, ray.dir.y, ray.dir.z);

	printf("Pixel (%f, %f) - Colore calcolato: R: %f, G: %f, B: %f\n",
		x, y, color.r, color.g, color.b);

	return (color);
}*/