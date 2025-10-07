/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 13:28:00 by francesca         #+#    #+#             */
/*   Updated: 2025/10/07 13:28:25 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* Libera  gli objects della t_scena*/
void	scene_free(t_scene *scene)
{
	free(scene->objects);
	scene->objects = NULL;
	scene->object_count = 0;
	scene->object_cap = 0;
}
