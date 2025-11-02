/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:26:54 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 14:44:12 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// L'intersezione della sfera non necessita del raggio per essere trasformato
// ray: raggio in world space (origine + direzione)
// xs: struttura dove appendere le intersezioni trovate
// sphere: puntatore allo shape
// vector da centro sfera C a origine raggio O (O - C)

/*
Funzione per vedere se un raggio colpisce una sfera, usando formula quadrica
Calcola il discriminante h:
Se h < 0 → non colpisce.
Se h >= 0 → due punti di intersezione: -b ± sqrt(h)
(prodotto scalare tra oc e la direzione del raggio D) , 
ci dice a che distanza il raggio colpisce
è la distanza al quadrato tra origine raggio e centro sfera, 
ci dice quanto il raggio è “lontano” dal centro
*/
bool	intersect_sphere_fast(const t_ray *ray, t_intersections *xs,
	t_shape *sphere)
{
	t_vector	oc;
	double		b;
	double		c;
	double		h;

	sub_vec(&oc, &ray->origin, &sphere->origin);
	b = dot_product(&oc, &ray->dir);
	c = dot_product(&oc, &oc) - sphere->props.radius_squared;
	h = b * b - c;
	if (h < 0.0)
		return (false);
	h = sqrt(h);
	xs->arr[xs->count].time = -b - h;
	xs->arr[xs->count].shape = sphere;
	xs->arr[xs->count + 1].time = -b + h;
	xs->arr[xs->count + 1].shape = sphere;
	xs->count += 2;
	return (true);
}
/*
Questa funzione calcola l'intersezione raggio–piano
denom = D · N (direzione del raggio per la normale del piano)
Se denom è molto vicino a 0 il raggio è parallelo al piano
 → nessuna intersezione utile, 
ritorna false. La soglia 0.00001 evita divisione per 
valori molto piccoli (problemi numerici)

// prodotto scalare tra direzione del raggio e la normale del piano
// dice quanto il raggio è inclinato rispetto al piano
// Se denom ≈ 0 → il raggio è parallelo al piano 
→ non lo colpisce, fabs rende il valore assoluto
// Si calcola il valore t → cioè quando il raggio colpisce il piano,
// e si salva l’intersezione in xs 
*/

bool	intersect_plane_fast(const t_ray *ray, t_shape *plane,
		t_intersections *xs)
{
	double	denom;

	denom = dot_product(&ray->dir, &plane->orientation);
	if (fabs(denom) < 0.00001)
		return (false);
	xs->arr[xs->count].time = -(dot_product(&ray->origin, &plane->orientation)
			- plane->props.distance_from_origin) / denom;
	xs->arr[xs->count].shape = plane;
	xs->count++;
	return (true);
}
/*
controlla che tipo di oggetto è, e chiama la funzione giusta
Funzione dispatcher che decide quale routine di intersezione 
chiamare in base al tipo di shape.
Prima cosa: se xs è pieno (>=200) non aggiunge 
altro e ritorna false per sicurezza.

Se lo shape è una sfera o un piano, 
chiama le versioni “fast” che non trasformano il raggio: 
assumono che la sfera/piano siano espressi in world space
Se hai già trovato 200 intersezioni l’array è pieno, blocchi
transf_ray per portare il cilindro al "centro" fare calcoli e invertirlo di nuovo
*/

bool	intersect(t_shape *shape, const t_ray *ray, t_intersections *xs)
{
	t_ray	transf_ray;

	if (xs->count >= 200)
		return (false);
	if (shape->type == SPHERE)
		return (intersect_sphere_fast(ray, xs, shape));
	if (shape->type == PLANE)
		return (intersect_plane_fast(ray, shape, xs));
	if (shape->type == CYLINDER)
		transform_ray(&transf_ray, ray, shape);
	return (intersect_cylinder(&transf_ray, shape, xs));
	return (false);
}

//NOTE
/*
FORMULA QUADRICA
t = (-b ± √(b² - 4ac)) / 2a
E il pezzo importante è Δ = b² - 4ac, che si chiama discriminante.
*/ 