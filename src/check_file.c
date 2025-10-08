/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:29:04 by francesca         #+#    #+#             */
/*   Updated: 2025/10/08 14:09:05 by francesca        ###   ########.fr       */
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

/* Controlla che i Valori delle coordinate della camra siano nei range ammessi
dal subject:
3D normalized orientation vector, 
in the range [-1,1] 
for each x, y, z axis: 0.0,0.0,1.0 */
int check_vec3direction(t_vector *direction_value)
{
    /* 2.a) Componenti in [-1,1] */
	if (direction_value->x < -1.0 || direction_value->x > 1.0
		|| direction_value->y < -1.0 || direction_value->y > 1.0
		|| direction_value->z < -1.0 || direction_value->z > 1.0)
		return (0);
    return (1);
}