/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 23:12:05 by francesca         #+#    #+#             */
/*   Updated: 2025/10/07 21:56:55 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* --- Ambiente --- */
/*
 * parse_ambient_line:
 *   Sintassi attesa (subject):
 *     A <ratio> <R,G,B>
 *   - ratio: double in [0..1]
 *   - R,G,B: interi in [0..255]
 *   Il puntatore 'rest_of_line' punta subito dopo l'ID 'A'.
 *   Ritorna 0 su successo, 1 su errore di formato o di range.
 *
typedef struct s_ambient
{
	double ratio;  // [0..1]
	t_color color; // normalizzato 0..1
	bool present;  // per validare che A compaia una sola volta
}	t_ambient;
*/
int	parse_ambient_line(t_scene *scene, char *rest_of_line)
{
	const char  *cursor;
	double      ratio_value;
	int         red;
	int         green;
	int         blue;
	
	if (scene == NULL || rest_of_line == NULL)
	{
		fprintf(stderr, "Error\nParametro mancante per 'A'\n");
		return (1);
	}
	if (scene->amb.present == true)
	{
		fprintf(stderr, "Error\nAmbiente 'A' definito più di una volta\n");
		return (1);
	}

	cursor = skip_spaces(rest_of_line);

	/* 1) Leggi ratio */
	if (!parse_double(&cursor, &ratio_value))
	{
		fprintf(stderr, "Error\nFormato ratio di 'A' non valido\n");
		return (1);
	}
	if (ratio_value < 0.0 || ratio_value > 1.0)
	{
		fprintf(stderr, "Error\nRatio di 'A' fuori range [0..1]\n");
		return (1);
	}

	/* 2) Atteso almeno uno spazio prima del colore */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
	{
		fprintf(stderr, "Error\nColore di 'A' mancante (atteso R,G,B)\n");
		return (1);
	}

	/* 3) Leggi R,G,B nel range [0..255] */
	if (!parse_rgb(&cursor, &red, &green, &blue))
	{
		fprintf(stderr, "Error\nFormato colore di 'A' non valido (atteso R,G,B)\n");
		return (1);
	}

	/* 4) Non devono esserci token extra dopo il colore */
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
	{
		fprintf(stderr, "Error\nToken extra dopo il colore in 'A'\n");
		return 1;
	}

	/* 5) Salva nella scena (normalizza il colore in [0..1]) */
	scene->amb.ratio     = ratio_value;
	scene->amb.color.r   = (double)red   / 255.0;
	scene->amb.color.g   = (double)green / 255.0;
	scene->amb.color.b   = (double)blue / 255.0;
	scene->amb.present   = true;

	return 0;
}
