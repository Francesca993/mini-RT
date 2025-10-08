/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:12:18 by francesca         #+#    #+#             */
/*   Updated: 2025/10/08 14:09:45 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
C -50.0,0,20 0,0,1 70
∗ identifier: C
∗ x, y, z coordinates of the viewpoint: -50.0,0,20
∗ 3D normalized orientation vector, in the range [-1,1] for each x, y, z axis:
0.0,0.0,1.0
∗ FOV: Horizontal field of view in degrees in the range [0,180]: 70
*/


/* controllo normalizzazione SENZA sqrt: |x^2+y^2+z^2 - 1| <= eps2
è per controllare che il vettore direzione della camera sia davvero normalizzato,
cioè che la sua lunghezza (modulo) sia uguale a 1 */
static int	is_normalized_len2(double x, double y, double z, double eps2)
{
	double	len2;
	double	diff;

	len2 = x * x + y * y + z * z;
	diff = len2 - 1.0;
	if (diff < 0.0)
		diff = -diff;
	return (diff <= eps2);
}

/* ============================ parse_camera_line ============================ */
/* Check se ambiente fosse già presente, parse dei primi vettori,
e check che riesca,
la posizione si sposta quindi imposto il valore w. Vado avanti assicurandomi che 
ci sia almeno uno spazio,converto la direzione,
che non avrà w. Fa il check che i valori siano nei range del subbject.
Controlla che il vettore sia normalizzato, fa di nuovo il check su gli spazi,
converte il FOV, ne controlla il range che sia nei
limiti del subject,e poi lo converte in radiani. controlla che sia fine stringa 
o solo spazi dopo, e salva tutto nella t_scene.
*/
int	parse_camera_line(t_scene *scene, char *rest_of_line)
{
	const char	*cursor;
	t_vector	position_value;
	t_vector	direction_value;
	int			fov_degrees_integer;
	double		pi_const;

	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Error: Parametri mancanti per 'C'"));
	if (scene->cam.present == true)
		return (print_err_msg("Error: Camera 'C' definita più di una volta"));
	cursor = skip_spaces(rest_of_line);
	/* 1) Posizione: x,y,z (CSV senza spazi) */
	if (!parse_vec3(&cursor, &position_value))
		return (print_err_msg("Error: Posizione camera non valida (atteso x,y,z senza spazi)"));
	/* la posizione è un punto → w=1.0  perchè si sposta*/
	position_value.w = 1.0;
	/* almeno uno spazio prima del prossimo token */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: Direzione camera mancante (atteso nx,ny,nz)"));
	/* 2) Direzione: nx,ny,nz (CSV senza spazi) */
	if (!parse_vec3(&cursor, &direction_value))
		return (print_err_msg("Error: Direzione camera non valida (atteso nx,ny,nz senza spazi)"));
	/* direzione è un vettore → w=0.0 perchè NON si sposta*/
	direction_value.w = 0.0;
	// /* 2.a) Componenti in [-1,1] */
	if (!check_vec3direction(&direction_value))
		// if (direction_value.x < -1.0 || direction_value.x > 1.0
		// 	|| direction_value.y < -1.0 || direction_value.y > 1.0
		// 	|| direction_value.z < -1.0 || direction_value.z > 1.0)
		return (print_err_msg("Error: Componenti direzione fuori range [-1,1]"));
	/* 2.b) Vettore normalizzato (|n|=1) con tolleranza su len² */
	if (!is_normalized_len2(direction_value.x, direction_value.y,
			direction_value.z, 1e-6))
		return (print_err_msg("Error: Direzione camera non normalizzata (|n| deve essere 1)"));
	/* almeno uno spazio prima del prossimo token */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Error: FOV della camera mancante"));
	/* 3) FOV in gradi [0..180] (orizzontale) */
	if (!parse_int(&cursor, &fov_degrees_integer))
		return (print_err_msg("Error: FOV della camera non valido (atteso intero)"));
	if (fov_degrees_integer < 0 || fov_degrees_integer > 180)
		return (print_err_msg("Error: FOV della camera fuori range [0..180]"));
	/* 4) Niente token extra */
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg("Error: Token extra dopo il FOV della camera"));
	/* 5) Salvataggio in scena */
	scene->cam.pos = position_value;
	scene->cam.dir = direction_value;
	scene->cam.fov_deg = (double)fov_degrees_integer;
	pi_const = 3.14159265358979323846;
	scene->cam.fov_rad = scene->cam.fov_deg * (pi_const / 180.0);
	scene->cam.present = true;
	scene->n_camera += 1;
	return (0);
}
/*
🌍 Contesto: coordinate omogenee
In grafica 3D, un vettore 3D (x, y, z) viene a volte esteso a 4 componenti
 (x, y, z, w) per poter usare le matrici 4×4 (trasformazioni affini, proiezioni, ecc.).
Questo si chiama sistema di coordinate omogenee.
📐 Differenza tra punto e direzione
La camera ha:
una posizione → dove si trova nel mondo (es: sul tavolo, a 1 metro dal pavimento),
una direzione → dove guarda (es: verso la porta).
Tipo	Coordinate	Significato di w
Punto (posizione nello spazio)	(x, y, z, 1)	w = 1.0 indica che si sposta con le traslazioni
Vettore (direzione o velocità)	(x, y, z, 0)	w = 0.0 indica che non è influenzato dalle traslazioni
Quando moltiplichi un vettore o un punto per una matrice di trasformazione 4×4, w determina se 
la traslazione della matrice deve applicarsi o no.
Mettere w = 1.0 per i punti e w = 0.0 per i vettori:
Campo	Tipo	w	Effetto
scene->cam.pos	posizione nello spazio	1.0	si sposta con la camera
scene->cam.dir	direzione di vista	0.0	non si sposta, solo ruota
light.pos	punto nello spazio	1.0	si sposta
normal	vettore direzione	0.0	non si sposta
*/