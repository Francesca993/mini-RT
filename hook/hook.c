/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:29:08 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/25 13:44:32 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

int key_hook(int keycode, void *param)
{
    t_scene *scene = (t_scene *)param;

    if (keycode == KEY_ESC)  
    {
        if (scene->disp->win)
        {
            mlx_destroy_window(scene->disp->mlx, scene->disp->win);
            scene->disp->win = NULL;
        }
        exit(0);
    }
    return (0);
}

int close_hook(void *param)
{
    t_scene *scene = (t_scene *)param;
    if (scene->disp->win)
        mlx_destroy_window(scene->disp->mlx, scene->disp->win);
    exit(0);
    return (0);
}

