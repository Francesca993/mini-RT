/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:48:43 by francesca         #+#    #+#             */
/*   Updated: 2025/10/08 11:09:05 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ---- HELPER ---- */
/*
Quello che fa è creare una variabile c di tipo  t_vector,
	assegnargli i valori r, g, b ai rispettivi campi, e restituirla.
*/
static inline t_vector	vector_make(double x, double y, double z)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	v.w = 0.0; /* default: vettore direzione */
	return (v);
}
/*
Quello che fa è creare una variabile c di tipo t_color, assegnargli i valori r,
	g, b ai rispettivi campi, e restituirla.
*/
static inline t_color	color_make(double r, double g, double b)
{
	t_color	c;

	c.r = r;
	c.g = g;
	c.b = b;
	return (c);
}

/* Inizializza/azzera completamente la scena,
	liberando eventuale storage precedente. */
void	scene_reset(t_scene *scene)
{
	/* --- sicurezza memoria dinamica oggetti --- */
	if (scene->objects != NULL)
	{
		free(scene->objects);
		scene->objects = NULL;
	}
	scene->object_count = 0;
	scene->object_cap = 0;
	/* --- Ambient --- */
	scene->amb.ratio = 0.0;
	scene->amb.color = color_make(0.0, 0.0, 0.0);
	scene->amb.present = false;
	/* --- Camera --- */
	scene->cam.pos = vector_make(0.0, 0.0, 0.0);
	scene->cam.dir = vector_make(0.0, 0.0, 1.0);
	/* direzione di default “in avanti” */
	scene->cam.fov_rad = 0.0;
	scene->cam.present = false;
	/* --- Light (spot principale del mandatory) --- */
	scene->light.pos = vector_make(0.0, 0.0, 0.0);
	scene->light.intensity = 0.0;
	scene->light.color = color_make(1.0, 1.0, 1.0);
	scene->light.present = false;
	/* --- contatori logici per validazione (A/C/L una sola volta) --- */
	scene->n_ambient = 0;
	scene->n_camera = 0;
	scene->n_lights = 0;
	/* --- contatori oggetti geometrici --- */
	scene->n_spheres = 0;
	scene->n_planes = 0;
	scene->n_cylinders = 0;
	return ;
}

/*Prende la riga con gnl, salta gia quelle vuote, inizializza scene,
	leva il \n e mette '\0' e fa partire la funzione che mentre legge riga
per irga, fa anche il chekc in "tempo reale" dei requisiti. */
int	parse_file(const char *path, t_scene *scene)
{
	char	*line;
	int		fd;

	if (!path || !scene)
	{
		return (print_err_msg("parse_file: argomenti non validi."));
		// printf("parse_file: argomenti non validi\n");
		// return (1);
	}
	scene_reset(scene); // reset contatori / inizializzazione scena
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("miniRT: open");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		if (ft_is_blank(line)) // <-- salta riga vuota o spazi
		{
			free(line);
			continue ; // salta le righe vuote
		}
		chop_newline(line); // Rimuove New line finale e mette '0'
		/*-------CONTROLLI DIRETTAMENTE SULLA LINEA APPENA LETTA -----------*/
		if (lex_scan_check_and_count(scene, line) != 0)
		{
			free(line);
			close(fd);
			return (1);
		}
		free(line);
	}
	if (close(fd) == -1)
	{
		perror("Error: miniRT: close");
		return (1);
	}
	return (0);
}
