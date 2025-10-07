/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:31:44 by francesca         #+#    #+#             */
/*   Updated: 2025/10/07 22:09:56 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Ritorna: id allocato (max 2 char + '\0') da free()-are.
// *rest_out: punta al resto della riga (dopo l'id e gli spazi).
// Non modifica 'line'.
static char	*check_first_token(char *line, char **rest_out)
{
	char	*id;
	int		i;

	i = 0;
	id = ft_calloc(10, sizeof(char));
	// max 10 char (per sicurezza anche se al massimo sono due,
	//per essere sicura di non tagliare qualcosa di piu lungo
	//	+ terminatore 
	if (!id) return (NULL);
	if (rest_out)
		*rest_out = NULL;
	/* Skippa spazi iniziali */
	while (*line && ft_is_space_char(*line))
		line++;
	/* token = [^space/tab]...  */
	// copia massimo 10 caratteri finché non trova spazio o fine riga
	while (*line && !ft_is_space_char(*line) && i < 9)
		id[i++] = *line++;
	id[i] = '\0'; // terminatore già garantito da calloc, ma chiudiamo
	if (!is_valid_identifier(id))
	/* is_valid_identifier Ritorna 1 se id è uno dei token ammessi nel mandatory */
	{
		fprintf(stderr, "Error\nIdentificatore non valido\n");
		return (NULL);
	}
	while (*line && ft_is_space_char(*line))
		line++;
	if (*line)
		*rest_out = line;
	return (id);
}

/*
** Analizza UNA riga già letta dal .rt:
** - estrae l'id
** - aggiorna i contatori nella scena
** - controlla duplicati per A/C/L come da subject:
**   “Gli elementi definiti da lettera maiuscola 
** possono essere dichiarati una sola volta”.
** Ritorna 0 ok, 1 errore.
*/
int	lex_scan_check_and_count(t_scene *scene, char *line)
{
	char *id;
	char *rest_out;

	id = NULL;
	if (!line || !scene)
		return (1);
	/* Ogni riga del file .rt deve contenere ESATTAMENTE un identificatore seguito
	   dal suo payload. Parsiamo solo il primo token (id) e passiamo il resto
	   (rest_out) al relativo parser. Non re-parsare il payload come se fosse
	   un altro identificatore. */
	rest_out = NULL;
	id = check_first_token(line, &rest_out);
	if (!id)
		return (1);
	if (ft_strcmp(id, "A") == 0)
	{
		if (parse_ambient_line(scene, rest_out) != 0)
		{
			free(id);
			return (1);
		}
		scene->n_ambient += 1;
	}
	else if (ft_strcmp(id, "C") == 0)
	{
		scene->n_camera += 1;
		if (scene->n_camera > 1)
		{
			fprintf(stderr,
				"Error\nCamera 'C' definita più di una volta\n");
			free(id);
			return (1);
		}
		/* parse_camera_line(scene, rest_out) */
	}
	else if (ft_strcmp(id, "L") == 0)
	{
		scene->n_lights += 1;
		if (scene->n_lights > 1)
		{
			fprintf(stderr, "Error\nLuce 'L' definita più di una volta\n");
			free(id);
			return (1);
		}
		/* parse_light_line(scene, rest_out) */
	}
	else if (ft_strcmp(id, "sp") == 0)
	{
		scene->n_spheres += 1;
		/* parse_sphere_line(scene, rest_out) */
	}
	else if (ft_strcmp(id, "pl") == 0)
	{
		scene->n_planes += 1;
		/* parse_plane_line(scene, rest_out) */
	}
	else if (ft_strcmp(id, "cy") == 0)
	{
		scene->n_cylinders += 1;
		/* parse_cylinder_line(scene, rest_out) */
	}
	else
	{
		fprintf(stderr, "Error\nIdentificatore sconosciuto: '%s'\n", id);
		free(id);
		return (1);
	}
	free(id);
	// --------- DEBUG   ---------------
	debug_print_scene(scene, "DEBUG SUL LEXER");
	return (0);
}