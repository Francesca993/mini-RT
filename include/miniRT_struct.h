/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:29:54 by francesca         #+#    #+#             */
/*   Updated: 2025/10/02 20:43:06 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_STRUCT_H
# define MINI_RT_STRUCT_H

/* --- Coordinate --- */
typedef struct s_vec3
{
	double x;
	double y;
	double z;
}			t_vec3;

/* --- Colori --- */
// Colori interni in spazio lineare [0..1]
typedef struct s_color
{
	double r;
	double g;
	double b;
}			t_color;

/* --- Ambiente --- */
// Ambient lighting (A ratio [0..1], RGB [0..255] -> normalizzati)
typedef struct s_ambient
{
	double ratio;  // [0..1]
	t_color color; // normalizzato 0..1
	bool present;  // per validare che A compaia una sola volta
}			t_ambient;

/* --- Camera --- */
// Camera (C pos, dir normalizzata, FOV in gradi [0..180])
typedef struct s_camera
{
	t_vec3	pos;
	t_vec3 dir;     // normalizzata
	double fov_deg; // (0,180)
	bool	present;
}			t_camera;

/* --- Luce --- */
// Light (L pos, intensità [0..1], colore)
typedef struct s_light
{
	t_vec3	pos;
	double intensity; // [0..1]
	t_color color;    // normalizzato 0..1
	bool	present;
}			t_light;

// --- scene container ---
typedef struct s_scene
{
	// istanze reali (da riempire via parser)
	t_ambient amb; // A (una sola nel mandatory)
	t_camera cam;  // C (una sola nel mandatory)
	t_light light; // L (una sola nel mandatory)
	// contatori utili durante il parsing/validazione
	int n_ambient; // per rilevare doppie dichiarazioni di A
	int n_camera;  // per C
	int n_lights;  // per L (bonus: servirà un array)
	// oggetti (per ora solo contatori; poi aggiungerai array/vettore)
	int		n_spheres;
	int		n_planes;
	int		n_cylinders;
}			t_scene;

#endif