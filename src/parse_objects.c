/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:31:04 by francesca         #+#    #+#             */
/*   Updated: 2025/10/10 16:59:21 by francesca        ###   ########.fr       */
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
	int         red;
	int         green;
	int         blue;
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
	if (!parse_rgb(&cursor, &red, &green, &blue))
		return (print_err_msg("Error: Formato colore di 'sp' non valido (atteso R,G,B)"));
	/* Non devono esserci token extra dopo il colore */
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Error: Token extra dopo il colore in 'sp'"));
    /* Sphere */
    payload.sphere.center = point;
    payload.sphere.radius = diameter_value;
    payload.sphere.color.r = (double)red / 255.0;
    payload.sphere.color.g = (double)green / 255.0;
    payload.sphere.color.b = (double)blue / 255.0;
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
    int         r;
	int         g;
	int         b;
    double len = 0;

	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Error: Parametri mancanti per 'pl'"));
	cursor = skip_spaces(rest_of_line);
	/* 1) Posizione: x,y,z (CSV senza spazi) */
	if (!parse_vec3(&cursor, &point))
		return (print_err_msg("Error: Posizione piano non valida (atteso x,y,z senza spazi)"));
	point.w = 1.0; /* punto nello spazio: traslazioni applicate */
	/* almeno uno spazio prima del prossimo token */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: 3D normalized normal vector plane mancante (atteso nx,ny,nz)"));
	/* 2) Normale: nx,ny,nz */
	if (!parse_vec3(&cursor, &normal))
		return (print_err_msg("Error: 3D normalized normal vector plane non valida (atteso nx,ny,nz senza spazi)"));
	/* direzione è un vettore → w=0.0 perchè NON si sposta*/
	normal.w = 0.0; /* vettore: traslazioni NON applicate */
	// /* 2.a) Componenti in [-1,1] */
	if (!check_vec3direction(&normal))
		// if (normal.x < -1.0 || normal.x > 1.0
		// 	|| normal.y < -1.0 || normal.y > 1.0
		// 	|| normal.z < -1.0 || normal.z > 1.0)
		return (print_err_msg("Error: 3D normalized normal vector plane fuori range [-1,1]"));
    /* Controllo vettore nullo e normalizzazione */
    len = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    if (len <= 0.0)
        return print_err_msg("Error: Normale del piano nulla (0,0,0) non valida");
    normal.x /= len;
    normal.y /= len;
    normal.z /= len;
	/* almeno uno spazio prima del prossimo token */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: colore del piano mancante"));
    	/* Leggi R,G,B nel range [0..255] */
	if (!parse_rgb(&cursor, &r, &g, &b))
		return (print_err_msg("Error: Formato colore di 'pl' non valido (atteso R,G,B)"));
	/* Non devono esserci token extra dopo il colore */
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Error: Token extra dopo il colore in 'pl'"));
    /* Sphere */
    payload.plane.point = point;
    payload.plane.normal = normal;
    payload.plane.color.r = (double)r / 255.0;
    payload.plane.color.g = (double)g / 255.0;
    payload.plane.color.b = (double)b / 255.0;
    /* 7) Append alla lista oggetti */
    if (object_list_append(scene, OBJ_PLANE, payload) != 0)
        return 1; /* object_list_append ha già stampato l'errore */
	return (0);
}




