/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:31:44 by francesca         #+#    #+#             */
/*   Updated: 2025/10/07 11:36:36 by francesca        ###   ########.fr       */
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
	// Secondo me non devo toccare la line qui per eitare di free(are) male dopo
	//  if (*line)
	//  {
	//      *line = '\0';       /* termina il token in place */
	//      line++;             /* spostati dopo il terminatore */
	//  }
	//  salta gli spazi dopo l'id e restituisci il resto
	while (*line && ft_is_space_char(*line))
		line++;
	// if (rest_out)
	if (*line)
		*rest_out = line;
	return (id);
}

/*
** Analizza UNA riga già letta dal .rt:
** - estrae l'id
** - aggiorna i contatori nella scena
** - controlla duplicati per A/C/L come da subject:
**   “Gli elementi definiti da lettera maiuscola possono essere dichiarati una sola volta”. :contentReference[oaicite:1]{index=1}
** Ritorna 0 ok, 1 errore.
*/
int	lex_scan_check_and_count(t_scene *scene, char *line)
{
	size_t i;
	char *id;
	char *rest_out;

	i = 0;
	id = NULL;
	if (!line || !scene)
		return (1);
	while (line[i] != '\0')
	{
		rest_out = NULL;
		id = check_first_token(line, &rest_out);
		// controlla il primo spezzone di line, controlla l'id e ritorna l'id, 
		// e nel rest_out il resto dopo l'id
		if (!id)
			return (1);
		if (ft_strcmp(id, "A") == 0)
		{
			/* verifica duplicato e parsing completo dell'ambiente */
			if (scene->amb.present)
			{
				fprintf(stderr,
					"Error\nAmbiente 'A' definito più di una volta\n");
				free(id);
				return (1);
			}
			if (parse_ambient_line(scene, rest_of_line) != 0)
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
			/* qui poi chiamerò parse_camera_line(scene, rest_of_line) */
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
			/* qui poi chiamerò parse_light_line(scene, rest_of_line) */
		}
		else if (ft_strcmp(id, "sp") == 0)
		{
			scene->n_spheres += 1;
			/* qui poi chiamerò parse_sphere_line(scene, rest_of_line) */
		}
		else if (ft_strcmp(id, "pl") == 0)
		{
			scene->n_planes += 1;
			/* qui poi chiamerò parse_plane_line(scene, rest_of_line) */
		}
		else if (ft_strcmp(id, "cy") == 0)
		{
			scene->n_cylinders += 1;
			/* qui poi chiamerò parse_cylinder_line(scene, rest_of_line) */
		}
		else
		{
			fprintf(stderr, "Error\nIdentificatore sconosciuto: '%s'\n", id);
			free(id);
			return (1);
		}
		free(id);
		return (0);
	}
