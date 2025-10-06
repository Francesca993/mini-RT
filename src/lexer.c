/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:31:44 by francesca         #+#    #+#             */
/*   Updated: 2025/10/06 21:51:11 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Ritorna: id allocato (max 2 char + '\0') da free()-are.
// *rest_out: punta al resto della riga (dopo l'id e gli spazi).
// Non modifica 'line'.
static char *borrow_first_token(char *line, char **rest_out)
{
    char *start;
    char	*id;
    
    id = ft_calloc(10, sizeof(char)); // max 2 char + terminatore
	if (!id)
		return (NULL);
     if (rest_out) 
        *rest_out = NULL;
    
    /* Skippa spazi iniziali (già eviti righe vuote prima, ma è robusto) */
    while (*line && ft_is_space_char(*line)) 
        line++;
    /* token = [^space/tab]...  */
	// copia massimo 10 caratteri finché non trova spazio o fine riga
	while (*line && !ft_is_space_char(*p) && i < 9)
		id[i++] = *p++;
    id[i] = '\0'; // terminatore già garantito da calloc, ma chiudiamo
	if (!is_valid_identifier(id))
    /* is_valid_identifier Ritorna 1 se id è uno dei token ammessi nel mandatory */
	{
		fprintf(stderr, "Error\nIdentificatore non valido\n");
		return (NULL);
	}
    if (*line)
    {
        *line = '\0';       /* termina il token in place */
        line++;             /* spostati dopo il terminatore */
    }
    // salta gli spazi dopo l'id e restituisci il resto
    while (*line && ft_is_space_char(*line))
        line++;
    if (rest_out)
        *rest_out = line;
    returtn (id);
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
		id = borrow_first_token(line, &rest_out);
        rest_out = NULL;
		if (!id)
			return (1);
		if (ft_strcmp(id, "A") == 0)
		{
			scene->n_ambient++;
			if (scene->n_ambient > 1)
			{
				fprintf(stderr,
					"Error\nAmbiente 'A' definito più di una volta (riga
					%zu)\n", i + 1);
				return (1);
			}
			scene->amb.present = true;
		}
		else if (ft_strcmp(id, "C") == 0)
		{
			scene->n_camera++;
			if (scene->n_camera > 1)
			{
				fprintf(stderr,
					"Error\nCamera 'C' definita più di una volta (riga %zu)\n",
					i + 1);
				return (1);
			}
			scene->cam.present = true;
		}
		else if (ft_strcmp(id, "L") == 0)
		{
			scene->n_lights++;
			if (scene->n_lights > 1)
			{
				fprintf(stderr, "Error\nLuce 'L' definita più di una volta (riga
					%zu)\n", i + 1);
				return (1);
			}
			scene->light.present = true;
		}
		else if (ft_strcmp(id, "sp") == 0)
		{
			scene->n_spheres++;
		}
		else if (ft_strcmp(id, "pl") == 0)
		{
			scene->n_planes++;
		}
		else if (ft_strcmp(id, "cy") == 0)
		{
			scene->n_cylinders++;
		}
		free(id);
		i++;
	}
	return (0);
}
