/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:29:08 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/31 17:37:37 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

int	key_hook(int keycode, void *param)
{
	t_scene	*scene;

	scene = (t_scene *)param;
	if (keycode == KEY_ESC)
	{
		if (scene)
		{
			scene_free(scene);
			free(scene);
		}
		exit(0);
	}
	return (0);
}

int	close_hook(void *param)
{
	t_scene	*scene;

	scene = (t_scene *)param;
	if (scene)
	{
		scene_free(scene);
		free(scene);
	}
	exit(0);
	return (0);
}
