/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:29:54 by francesca         #+#    #+#             */
/*   Updated: 2025/10/10 18:52:13 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_STRUCT_H
# define MINI_RT_STRUCT_H

# include <stdbool.h>
 #include <stddef.h>

/* --- Coordinate --- */
typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
	double	w;
}			t_vector;

/* --- Colori --- */
// Colori interni in spazio lineare [0..1]
/*
PERCHE' RGB IN DOUBLE
Quando fai i calcoli di luce, riflessione, ombreggiature, ecc.,
il programma deve moltiplicare e sommare i colori —
e deve farlo con precisione decimale, non con numeri interi.
Se hai un’illuminazione del 40% (ratio = 0.4)
e un colore rosso 255,0,0
allora il colore illuminato sarà: 255 * 0.4 = 102 
Ma se tu usassi solo interi (0 o 1), perderesti tutta la sfumatura.
*/
typedef struct s_color
{
	double	r;
	double	g;
	double	b;
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
	t_vector	pos;
	t_vector 	dir;     // normalizzata
	int    fov_deg;    // come da file .rt (per debug/log)
    double fov_rad;    // per i calcoli
	bool	present;
}			t_camera;

/* --- Luce --- */
// Light (L pos, intensità [0..1], colore)
typedef struct s_light
{
	t_vector	pos;
	double intensity; // [0..1]
	t_color color;    // normalizzato 0..1
	bool	present;
}			t_light;

/* --- Oggetti (solo struct, niente logica) --- */
typedef enum e_objtype
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER
}			t_objtype;

/* --- Sphere --- */
typedef struct s_sphere
{
	t_vector center; // centro
	double radius; // diametro/2, precomputato dal parser
	t_color color; // 0..1
}			t_sphere;

/* --- Plane --- */
typedef struct s_plane
{
	t_vector point;  // un punto sul piano
	t_vector normal; // normalizzata
	t_color color; // 0..1
}			t_plane;

/* --- Cylinder --- */
typedef struct s_cylinder
{
	t_vector base;   // punto base lungo l'asse (convenz.
	t_vector axis;   // direzione normalizzata
	double radius; // diametro/2
	double height; // altezza finita
	t_color color; // 0..1
}			t_cylinder;

/* ───────────── Contenitore generico ───────────── */

typedef union u_figures
{
	t_sphere			sphere;
	t_plane				plane;
	t_cylinder			cylinder;
}						t_figures;

/* Vector di oggetti con due array paralleli */
typedef struct s_objnode 
{
    t_objtype         type;   /* OBJ_SPHERE / OBJ_PLANE / OBJ_CYLINDER */
    t_figures         fig;    /* union con i dati dell’oggetto */
    struct s_objnode *next;
} t_objnode;

/* ───────────── Scena ───────────── */
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
	size_t		object_count; // quanti oggetti totali
	t_objnode *obj_head; /* testa */
    t_objnode *obj_end; /* coda per append O(1) */
}			t_scene;

#endif