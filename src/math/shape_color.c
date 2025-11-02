/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:39:21 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 14:33:14 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Imposta valori predefinifi per le proprietà di un oggetto,
crea nuovo oggetto e inizializza i campi con parametri base 
*/
void	default_shape_props(t_shape_props *props)
{
	ft_bzero(&props->color, sizeof(t_color));
	props->diffuse = 0.9;
	props->radius = 0.7;
	props->radius_squared = 0.7 * 0.7;
	props->scale = (t_vector){1, 1, 1, 0};
	props->reflectiveness = 0.2;
	props->shininess = 50;
	props->specular = 0.8;
}
/*
In base all’intersezione (il punto dove il raggio ha colpito),
dice che colore ha quell’oggetto lì
Usa semplicemente il colore base dello shape
*/

t_color	get_shape_color(t_intersection *itx)
{
	return (itx->shape->props.color);
}

/*
calcola quanta luce colpisce un punto su un oggetto 
sia diffusa che riflessa
IMPLEMENTA POI LA SPOT LIGHT
quanto la luce punta nella stessa direzione della superficie
il vettore che va dal punto colpito alla luce
vettore riflesso della luce (serve per calcolare il brillio)
capiamo da dove arriva la luce rispetto a quel punto.
normalizziamo per facilitare calcoli
la normale non ha componente w
Calcoliamo quanto la luce sta puntando nella direzione della superficie, 
> 0 → la luce illumina il punto, 0 → la luce è di lato, 
< 0  → la luce arriva da dietro
se la luce arriva da dietro, o c’è un altro oggetto davanti
punto non illuminato
Calcoliamo la luce diffusa, cioè quella che si spalma sul punto
la luce diffusa un po’ meno forte 80%
Invertiamo la direzione della luce er calcolare come si 
riflette la luce verso l’occhio
Calcoliamo la direzione del riflesso della luce, come se fosse uno specchio
Calcoliamo quanto è brillante il riflesso su questo punto
*/
bool	get_specular_and_diffuse(t_scene *scene, int light_idx,
	t_intersection *itx, t_phong *phong)
{
	double		light_dot_normal;
	t_vector	light_v;
	t_vector	reflect_v;

	sub_vec(&light_v, &scene->lights[light_idx].position, &itx->over_point);
	normalize_vec(&light_v);
	itx->normal.w = 0;
	light_dot_normal = dot_product(&light_v, &itx->normal);
	if (light_dot_normal < 0 || is_shadowed(scene, light_idx, &itx->over_point))
		return (false);
	mult_color(&phong->diffuse, &phong->effective_color, light_dot_normal
		* itx->shape->props.diffuse * scene->lights[light_idx].intensity);
	mult_color(&phong->diffuse, &phong->diffuse, 0.8);
	negate_vec(&light_v, &light_v);
	reflect_vector(&reflect_v, &light_v, &itx->normal);
	calculate_specular(&reflect_v, itx, phong, &scene->lights[light_idx]);
	return (true);
}
// NOTE PER ME 
/*
FORMULA LUCE DIFFUSA 
colore finale =
    colore dell’oggetto *
    quanta luce arriva (dot) *
    quanto l’oggetto è 'diffuso' (tipo stoffa) *
    intensità della luce
*/