/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transforms.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 19:03:27 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 13:38:15 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Calcola di quanto ruotare un oggetto per farlo 
“guardare” nella giusta direzione ORIENTATION
Ogni oggetto nasce con l’asse Y verso l’alto
Prende il vettore “up” = (0,1,0)
Confronta up con shape->orientation
Se sono diversi, calcola l’angolo tra i due e l’asse attorno a cui ruotare
Usa axis_angle per creare la matrice di rotazione
*/

void	calculate_orientation(t_mat4 *rot_transform, t_shape *shape)
{
	t_vector	up;
	t_vector	ax;
	double		angle;

	if (shape->orientation.x == 0 && fabs(shape->orientation.y - 1) < 0.001
		&& shape->orientation.z == 0)
		return (rotation_matrix_x(rot_transform, 0));
	if (shape->orientation.x == 0 && fabs(shape->orientation.y + 1) < 0.001
		&& shape->orientation.z == 0)
		return (rotation_matrix_x(rot_transform, -M_PI));
	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;
	ax.w = 0;
	normalize_vec(&up);
	normalize_vec(&shape->orientation);
	cross_product(&ax, &up, &shape->orientation);
	normalize_vec(&ax);
	angle = acos(dot_product(&shape->orientation, &up));
	axis_angle(rot_transform, &ax, angle);
}

/*
Combina tutte le trasformazioni (scala, rotazione, traslazione) 
in una sola matricetrasformazioni si applicano in ordine:
Scala → Rotazione → Traslazione
Moltiplica le tre matrici scale, rot, translate
per ottenere la trasformazione finale dell’oggetto
NOTA
inv_transf = inversa della matrice
serve per trasformare i raggi nel sistema locale dell’oggetto
*/
void	multiply_transforms(t_shape *shape, t_mat4 *scale, t_mat4 *rot,
	t_mat4 *translate)
{
	t_mat4	temp;

	mat_multiply(&shape->transf, translate, &shape->added_rots);
	ft_memcpy(&temp, &shape->transf, sizeof(t_mat4));
	mat_multiply(&shape->transf, &temp, rot);
	ft_memcpy(translate, &shape->transf, sizeof(t_mat4));
	mat_multiply(&shape->transf, translate, scale);
	mat_inverse(&shape->inv_transf, &shape->transf);
	ft_memcpy(&shape->norm_transf, &shape->inv_transf, sizeof(t_mat4));
	transpose_matrix(&shape->norm_transf);
}
/*
Prepara le matrici di scala, rotazione e traslazione per un singolo oggetto
Crea 3 matrici base: scale, rot, translate
Imposta quanto scalare
Crea la matrice di traslazione
Combina tutto con multiply_transforms salva tutto in dentro shape->transf
*/

void	calculate_shape_transforms(t_shape *shape)
{
	t_mat4	scale;
	t_mat4	rot;
	t_mat4	translate;

	identity_matrix(&shape->transf);
	identity_matrix(&scale);
	identity_matrix(&rot);
	identity_matrix(&translate);
	if (shape->type == SPHERE)
		scaling_matrix(&scale, shape->props.scale.x,
			shape->props.scale.y, shape->props.scale.z);
	if (shape->type == CYLINDER)
		scaling_matrix(&scale, shape->props.scale.x,
			1, shape->props.scale.z);
	if (shape->type == PLANE || shape->type == CYLINDER)
		calculate_orientation(&rot, shape);
	translate_matrix(&translate, shape->origin.x,
		shape->origin.y, shape->origin.z);
	multiply_transforms(shape, &scale, &rot, &translate);
}

/*
Applica tutte le trasformazioni a tutta la scena
Calcola la trasformazione della camera calculate_camera_transform(scene)
Cicla su tutti gli oggetti (scene->shapes) e chiama calculate_shape_transforms
Aggiorna anche la direzione delle luci se ruotano ,ma da capire nel mandatory
*/
void	calculate_transforms(t_scene *scene)
{
	int		i;

	calculate_camera_transform(scene);
	i = 0;
	while (i < scene->n_shapes)
	{
		calculate_shape_transforms(&scene->shapes[i]);
		i++;
	}
	i = 0;
	while (i < scene->n_lights)
	{
		mat_vec_multiply(&scene->lights[i].direction,
			&scene->lights[i].added_rots, &scene->lights[i].init_direction);
		i++;
	}
}
