/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 13:28:00 by francesca         #+#    #+#             */
/*   Updated: 2025/11/02 14:17:26 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	scene_free(t_scene *scene)
{
	if (!scene)
		return ;
	objlist_destroy(scene);
	scene->object_count = 0;
	if (scene->lights)
	{
		free(scene->lights);
		scene->lights = NULL;
		scene->n_lights = 0;
	}
	if (scene->disp)
	{
		if (scene->disp->display_img && scene->disp->mlx)
		{
			mlx_destroy_image(scene->disp->mlx, scene->disp->display_img);
			scene->disp->display_img = NULL;
		}
		if (scene->disp->win && scene->disp->mlx)
		{
			mlx_destroy_window(scene->disp->mlx, scene->disp->win);
			mlx_destroy_display(scene->disp->mlx);
			free(scene->disp->mlx);
			scene->disp->win = NULL;
		}
		//mlx_loop_end(scene->disp->mlx);
	}
}

/* 
For free di OBJECTS_TYPES
*/
void	free_array(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
