/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:10:57 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 18:18:54 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	init_display(t_display *disp, t_settings *settings)
{
	disp->mlx = mlx_init();
	if (disp->mlx == NULL)
	{
		return ;
	}
	disp->win = mlx_new_window(disp->mlx, settings->disp_w, settings->disp_h,
			"MiniRT");
	if (disp->win == NULL)
	{
		return ;
	}
	disp->display_img = mlx_new_image(disp->mlx, settings->render_w,
			settings->render_h);
	if (disp->display_img == NULL)
	{
		return ;
	}
	disp->disp_addr = mlx_get_data_addr(disp->display_img, &disp->bpp,
			&disp->line_length, &disp->endian);
	disp->bpp /= 8;
	mlx_do_key_autorepeatoff(disp->mlx);
}

void	init_settings(t_settings *settings)
{
	settings->render_w = 1920;
	settings->render_h = 1080;
	settings->disp_w = 1920;
	settings->disp_h = 1080;
	settings->reflection_depth = 1;
}
