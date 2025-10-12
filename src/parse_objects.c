/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:31:04 by francesca         #+#    #+#             */
/*   Updated: 2025/10/11 17:37:12 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#import "miniRT.h"

/*
Sphere:
sp 0.0,0.0,20.6 12.6 10,0,255
∗ identifier: sp
∗ x, y, z coordinates of the sphere center: 0.0,0.0,20.6
∗ the sphere diameter: 12.6
∗ R,G,B colors in the range [0-255]: 10, 0, 255
*/
int parse_sphere(t_scene *scene, char *rest_of_line)
{
    const char  *cursor;
    t_vector    point;
    double      diameter_value;
	t_color		color;
    t_figures    payload;

        /* Controlli */
    if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Error: Parametro mancante per 'sp'"));
	cursor = skip_spaces(rest_of_line);
    /* Posizione: x,y,z (senza spazi) */
	if (!parse_vec3(&cursor, &point))
		return (print_err_msg("Error: Posizione Sphere non valida (atteso x,y,z senza spazi)"));
	/* Punto nello spazio: w = 1.0 */
	point.w = 1.0;
	/* almeno uno spazio prima del prossimo token */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: Diametro mancante (atteso [0.0])"));
    /* Leggi double per il Diametro */
	if (!parse_double(&cursor, &diameter_value))
		return (print_err_msg("Error: Formato Diametro non valido"));
    /* non può essere negativo no? */
    if (diameter_value <= 0.0)
        return (print_err_msg("Error: Valore Diametro negativo)"));
	/* Atteso almeno uno spazio prima del colore */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: Colore di 'sp' mancante (atteso R,G,B)"));
	/* Leggi R,G,B nel range [0..255] */
	if (!parse_rgb(&cursor, &color))
		return (print_err_msg("Error: Formato colore di 'sp' non valido (atteso R,G,B)"));
	/* Non devono esserci token extra dopo il colore */
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Error: Token extra dopo il colore in 'sp'"));
    /* Sphere */
    payload.sphere.center = point;
    payload.sphere.radius = diameter_value;
    payload.sphere.color = color;
    /* 7) Append alla lista oggetti */
    if (object_list_append(scene, OBJ_SPHERE, payload) != 0)
        return 1; /* object_list_append ha già stampato l'errore */
	return (0);
}

/*
∗ identifier: pl
∗ x, y, z coordinates of a point in the plane: 0.0,0.0,-10.0
∗ 3D normalized normal vector, in the range [-1,1] for each x, y, z axis:
0.0,1.0,0.0
∗ R,G,B colors in the range [0-255]: 0,0,225
*/
int parse_plane(t_scene *scene, char *rest_of_line)
{
    const char	*cursor;
	t_vector	point;
	t_vector	normal;
    t_figures   payload;
    t_color		color;

	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Error: Parametri mancanti per 'pl'"));
	cursor = skip_spaces(rest_of_line);
	/* 1) Posizione: x,y,z */
	if (!parse_vec3(&cursor, &point))
		return (print_err_msg("Error: Posizione piano non valida (atteso x,y,z senza spazi)"));
	point.w = 1.0; /* punto nello spazio: traslazioni applicate */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: 3D normalized normal vector plane mancante (atteso nx,ny,nz)"));
	/* Normale: nx,ny,nz */
	if (!parse_vec3(&cursor, &normal))
		return (print_err_msg("Error: 3D normalized normal vector plane non valida (atteso nx,ny,nz senza spazi)"));
	normal.w = 0.0; /* vettore: traslazioni NON applicate */
	/* Componenti in [-1,1] */
	if (!check_vec3direction(&normal))
		return (print_err_msg("Error: 3D normalized normal vector plane fuori range [-1,1]"));
    /* Controllo vettore nullo e normalizzazione  	//VIENE FATTO DOPO NELLA PARTE MATH */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: colore del piano mancante"));
    /* Leggi R,G,B nel range [0..255] */
	if (!parse_rgb(&cursor, &color))
		return (print_err_msg("Error: Formato colore di 'pl' non valido (atteso R,G,B)"));
	/* Non devono esserci token extra dopo il colore */
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Error: Token extra dopo il colore in 'pl'"));
    /* Sphere */
    payload.plane.point = point;
    payload.plane.normal = normal;
    payload.plane.color = color;
    /* 7) Append alla lista oggetti */
    if (object_list_append(scene, OBJ_PLANE, payload) != 0)
        return 1; /* object_list_append ha già stampato l'errore */
	return (0);
}

/*
 Cylinder:
cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255
∗ identifier: cy
∗ x, y, z coordinates of the center of the cylinder: 50.0,0.0,20.6
∗ 3D normalized vector of axis of cylinder, in the range [-1,1] for each x, y,
z axis: 0.0,0.0,1.0
∗ the cylinder diameter: 14.2
∗ the cylinder height: 21.42
∗ R, G, B colors in the range [0,255]: 10, 0, 255

typedef struct s_cylinder
{
	t_vector base;   // punto base lungo l'asse (convenz.
	t_vector axis;   // direzione normalizzata
	double radius; // diametro
	double height; // altezza finita
	t_color color; // 0..1
}			t_cylinder;
*/
int parse_cylinder(t_scene *scene, char *rest_of_line)
{
	const char *cursor;
	t_vector base;
	t_vector axis; // direzione normalizzata
	double radius; 
	double height;
	t_color color;
	t_figures   payload;

	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Error: Parametri mancanti per 'cy'"));
	cursor = skip_spaces(rest_of_line);
	/* Posizione: x,y,z */
	if (!parse_vec3(&cursor, &base))
		return (print_err_msg("Error: Posizione cilidro non valida (atteso x,y,z)"));
	base.w = 1.0; /* punto nello spazio: traslazioni applicate */
	cursor = skip_spaces(cursor); // Salto gli spazi
	if (*cursor == '\0')
		return (print_err_msg("Error: 3D normal vector cylinder mancante (atteso nx,ny,nz)"));
	if (!parse_vec3(&cursor, &axis))/* Normale: nx,ny,nz */
		return (print_err_msg("Error: 3D normal vector of axis of cylinder non valida (atteso nx,ny,nz senza spazi)"));
	axis.w = 0.0; /* vettore: traslazioni NON applicate */ //LO IMPOSTO QUI O NEL MATH?
	if (!check_vec3direction(&axis))// /* 2.a) Componenti in [-1,1] */
		return (print_err_msg("Error: 3D normal vector of axis of cylinder fuori range [-1,1]"));
    /* Controllo vettore nullo e normalizzazione */ // VIENE FATTO DOPO NEL MATH 
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: diametro del cilindro mancante"));
	/* Leggi diametro */
	if (!parse_double(&cursor, &radius))
		return (print_err_msg("Error: Formato diametro di 'cy' non valido"));
	if (radius <= 0.0)
		return print_err_msg("Error: Il diametro del cilindro deve essere > 0");
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
	return (print_err_msg("Error: altezza del cilindro mancante"));
	/* Leggi altezza */
	if (!parse_double(&cursor, &height))
		return (print_err_msg("Error: Formato altezza di 'cy' non valido"));
	if (height <= 0.0)
		return print_err_msg("Error: L'altezza del cilindro deve essere > 0");
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: colore del cilindro mancante"));
	/* Leggi R,G,B nel range [0..255] */
	if (!parse_rgb(&cursor, &color))
		return (print_err_msg("Error: Formato colore di 'cy' non valido (atteso R,G,B)"));
	/* Non devono esserci token extra dopo il colore */
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Error: Token extra dopo il colore in 'cy'"));
    /* Cylinder*/
	payload.cylinder.base = base;
	payload.cylinder.axis = axis;
	payload.cylinder.color = color;
	payload.cylinder.radius = radius;
	payload.cylinder.height = height;
    /* 7) Append alla lista oggetti */
    if (object_list_append(scene, OBJ_CYLINDER, payload) != 0)
        return 1; /* object_list_append ha già stampato l'errore */
	return (0);
}

