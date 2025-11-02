/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:46:03 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 13:55:03 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*A 0.1 255,255,255
C 0,1,-8 0,0,1 60
L -10,10,-10 0.8 128,128,153
pl 20,0,0 -1,0,0 255,230,230
sp -0.5,1,0.5 1.6 0,0,0         							
sp -3.5,1,0.5 1 0,0,0
sp -2.5,2,1 1.5 26,255,127
Calcola la luce ambientale che colpisce un punto
Moltiplica il colore dell'oggetto (patter_color) 
per quanto è forte la luce ambientale (intensity)
Mescola il colore ambientale ottenuto con il colore reale della luce ambientale
*/

t_color	get_ambient(t_scene *scene, t_color patter_color)
{
	t_color	ambient;

	ambient.a = 0;
	mult_color(&ambient, &patter_color,
		scene->ambient.intensity);
	blend_colors(&ambient, &ambient, &scene->ambient.color);
	return (ambient);
}

/*
Calcola il riflesso (quella parte brillante che si 
vede solo da certe angolazioni)
Calcola quanto il riflesso va nella direzione degli occhi dell’osservatore
se il riflesso non è visibile → metti a 0 la luce riflessa
specular = colore della luce
Moltipli per quanto l'oggetto riflette specular
Molt per quanto è lucido shininess pow per far sembrare i riflessi 
più piccoli e realistici
Moltipli per l’intensità della luce
*/
void	calculate_specular(t_vector *reflect_v, t_intersection *itx,
			t_phong *phong, t_light *light)
{
	double	reflect_dot_eye;

	reflect_dot_eye = dot_product(reflect_v, &itx->eye);
	if (reflect_dot_eye <= 0)
		ft_bzero(&phong->specular, sizeof(t_color));
	else
		mult_color(&phong->specular, &light->color,
			itx->shape->props.specular
			* pow(reflect_dot_eye, itx->shape->props.shininess)
			* light->intensity);
}
/*
Controlla se un oggetto sta facendo ombra su questo punto
Crea un raggio che va dal punto sulla superficie alla luce
Calcola la distanza tra il punto e la luce
Normalizza il raggio lungo 1
Imposta il punto di partenza del raggio nel punto sulla superficie
Per ogni oggetto nella scena, controlla se interseca 
il raggio che va verso la luce
Salva tutte le intersezioni trovate
Se c'è un'intersezione prima della luce, allora c'è 
un oggetto davanti , il punto è in ombra.
*/

bool	is_shadowed(t_scene *scene, int light_idx, t_vector *itx_point)
{
	double			distance;
	int				i;
	t_ray			ray;
	t_intersections	arr;
	t_intersection	*itx;

	sub_vec(&ray.dir, &scene->lights[light_idx].position, itx_point);
	distance = vec_magnitude(&ray.dir);
	scale_vec(&ray.dir, &ray.dir, 1 / distance);
	ray.origin = *itx_point;
	i = -1;
	arr.count = 0;
	while (++i < scene->n_shapes)
		intersect(&scene->shapes[i], &ray, &arr);
	itx = hit(&arr);
	if (itx && itx->time < distance)
		return (true);
	return (false);
}

/*
calcola il colore finale di un punto colpito da luce combinando:
luce ambientale
luce diffusa
luce riflessa
calcolo quanto è distante il punto dalla luce, piu lontano = meno luce
mescolo il colore dell’oggetto con il colore della luce
Se il punto non è illuminato xé è in ombra, allora ritorna solo luce ambientale
Altrimenti, inizia con la luce ambientale come base
Se la luce è molto distante, il punto riceve meno luce diffusa e riflessa
quindi le abbassiamo con l’attenuazione
Somma tutti i colori, Ambientale, Diffusa, Riflessa (speculare)
Ritorna il colore finale del punto sulla superficie
*/
t_color	phong(t_intersection *itx, t_scene *scene, int light_idx)
{
	t_phong			phong;
	t_color			result;
	t_color			shape_color;
	const double	light_dist = vec_distance(&itx->point,
			&scene->lights[light_idx].position);
	const double	attenuation = (100 * scene->lights[light_idx].intensity
			- light_dist) / (100 * scene->lights[light_idx].intensity - 1);

	shape_color = get_shape_color(itx);
	blend_colors(&phong.effective_color, &shape_color,
		&scene->lights[light_idx].color);
	if (get_specular_and_diffuse(scene, light_idx, itx, &phong) == false)
		return (get_ambient(scene, shape_color));
	result = get_ambient(scene, shape_color);
	if (attenuation < 0)
		return (result);
	else if (attenuation > 0 && attenuation <= 1)
	{
		mult_color(&phong.diffuse, &phong.diffuse, attenuation);
		mult_color(&phong.specular, &phong.specular, attenuation);
	}
	result.r += phong.diffuse.r + phong.specular.r;
	result.g += phong.diffuse.g + phong.specular.g;
	result.b += phong.diffuse.b + phong.specular.b;
	return (result);
}

t_color	shade_point(t_intersections *arr, t_scene *scene, t_ray *ray)
{
	t_intersection	*itx;
	int				light_idx;
	t_color			final_color;
	t_color			surface_color;
	t_color			reflected;

	itx = hit(arr);
	ft_bzero(&final_color, sizeof(t_color));
	if (itx != NULL)
	{
		prepare_computations(itx, ray);
		light_idx = 0;
		ft_bzero(&reflected, sizeof(t_color));
		while (light_idx < scene->n_lights)
		{
			surface_color = phong(itx, scene, light_idx);
			add_colors(&final_color, &final_color, &surface_color);
			add_colors(&final_color, &final_color, &reflected);
			light_idx++;
		}
	}
	return (final_color);
}

// NOTE PER ME
/*
pow è una funzione della libreria matematica del C (math.h)
Serve a fare le potenze:
pow(base, esponente)

Phong è il nome di un modello di illuminazione 3D
Quando un raggio di luce colpisce un oggetto, 
il colore finale del punto è fatto da 3 componenti:
Ambient = Luce "di base" che c'è ovunque, anche all’ombra
Diffuse = Luce che illumina direttamente la superficie
Specular = Luce che riflette e crea riflessi brillanti
Formula semplificata:
Colore finale = Ambient + Diffuse + Specular
*/