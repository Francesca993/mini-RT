/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:29:04 by francesca         #+#    #+#             */
/*   Updated: 2025/10/06 18:36:10 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <string.h>

//Controlla che il file sia .rt
//Check if the file has the extenctrion .rt
int has_rt_extension(const char *path)
{
	size_t len;

	if (path == NULL)
		return (0);
	len = ft_strlen(path);
	if (len < 3)
		return (0);
	if (ft_strcmp(path + len - 3, ".rt") == 0)
		return (1);
	return (0);
}


 /* --- check --- *
** Presenze obbligatorie e unicità:
A una sola volta.
C una sola volta.
L una sola volta nel mandatory (nel bonus potranno essere più luci).
** Almeno un oggetto (sp/pl/cy) deve essere presente per avere qualcosa da renderizzare 
(se non c’è nulla → errore logico di scena vuota).
*/
int check_startingscene(t_scene *scene)
{
    if (scene->n_ambient != 1)
    {
        printf("Errore: atteso 1 'A', trovato %d.\n", scene->n_ambient);
        return 1;
    }
    if (scene->n_camera != 1) 
    {
        printf("Errore: atteso 1 'C', trovato %d.\n", scene->n_camera);
        return 1;
    }
    if (scene->n_lights != 1) 
    {
        printf("Errore: atteso 1 'L', trovato %d.\n", scene->n_lights);
        return 1;
    }
    if ((scene->n_spheres + scene->n_planes + scene->n_cylinders) == 0) 
    {
        printf("Errore: nessun oggetto nella scena.\n");
        return 1;
    }
    return 0; /*SUCCESSO */
}

/* Ritorna 1 se id è uno dei token ammessi nel mandatory */
int is_valid_identifier(const char *id)
{
    if (!id || !*id) return 0;
    if (ft_strcmp(id, "A") == 0)  return 1;
    if (ft_strcmp(id, "C") == 0)  return 1;
    if (ft_strcmp(id, "L") == 0)  return 1;
    if (ft_strcmp(id, "sp") == 0) return 1;
    if (ft_strcmp(id, "pl") == 0) return 1;
    if (ft_strcmp(id, "cy") == 0) return 1;
    return 0;
}
