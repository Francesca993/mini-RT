/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:37:23 by francesca         #+#    #+#             */
/*   Updated: 2025/10/09 11:07:54 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
if (parse_light_line(scene, rest_out) != 0)
// Light (L pos, intensità [0..1], colore)
typedef struct s_light
{
	t_vector	pos;
	double intensity; // [0..1]
	t_color color;    // normalizzato 0..1
	bool	present;
}			t_light;
L -40.0,50.0,0.0    0.6    10,0,255
∗ identifier: L
∗ x, y, z coordinates of the light point: -40.0,50.0,0.0
∗ the light brightness ratio in the range [0.0,1.0]: 0.6
∗ (unused in mandatory part) R, G, B colors in the range [0-255]: 10, 0,
255
*/

int	parse_light_line(t_scene *scene, char *rest_of_line)
{
    const char  *cursor;
    t_vector    position_value;
    double      light_ratiovalue;
	int         red;
	int         green;
	int         blue;

    /* Controlli */
    if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Error: Parametro mancante per 'L'"));
    if (scene->light.present == true)
		return (print_err_msg("Error: Luce 'L' definito più di una volta"));
	cursor = skip_spaces(rest_of_line);
    /* Posizione: x,y,z (senza spazi) */
	if (!parse_vec3(&cursor, &position_value))
		return (print_err_msg("Error: Posizione Luce non valida (atteso x,y,z senza spazi)"));
	/* la posizione è → w=1.0  perchè si muove */
	position_value.w = 1.0;
	/* almeno uno spazio prima del prossimo token */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: Luminosità luce <ratio> mancante (atteso [0.0,1.0])"));
    /* Leggi ratio */
	if (!parse_double(&cursor, &light_ratiovalue))
		return (print_err_msg("Error: Formato ratio di 'L' non valido"));
    /* check range ratio */
	if (light_ratiovalue < 0.0 || light_ratiovalue > 1.0)
		return (print_err_msg("Error: Ratio di 'L' fuori range [0..1]"));
	/* Atteso almeno uno spazio prima del colore */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: Colore di 'L' mancante (atteso R,G,B)"));
	/* Leggi R,G,B nel range [0..255] */
	if (!parse_rgb(&cursor, &red, &green, &blue))
		return (print_err_msg("Error: Formato colore di 'L' non valido (atteso R,G,B)"));
	/* Non devono esserci token extra dopo il colore */
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Error: Token extra dopo il colore in 'L'"));
	/* Salva nella scena (normalizza il colore in [0..1]) */
    scene->light.pos = position_value;
    scene->light.intensity = light_ratiovalue;
    scene->light.color.r = (double)red   / 255.0;
    scene->light.color.g = (double)green / 255.0;
    scene->light.color.b = (double)blue / 255.0;
    scene->light.present   = true;
    scene->n_lights += 1;
	return (0);
}