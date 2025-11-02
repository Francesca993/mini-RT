/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:28:16 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 13:23:21 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <stdio.h>

/*
scrive il colore color nel pixel (x, y) del buffer immagine
fondamentale per disegnare i pixel nel render
*/
void	set_color(t_worker *worker, int x, int y, int color)
{
	int	bpp;

	if (x >= worker->width || y >= worker->height || x < 0 || y < 0)
		return ;
	bpp = worker->scene->disp->bpp;
	*(int *)(worker->addr + ((y * worker->width) + x) * bpp) = color;
}

/*
limita i canali colore tra 0 e 255
*/
static unsigned int	clamp_color(double color)
{
	color *= 255;
	if (color > 255)
		return (255);
	if (color < 0)
		return (0);
	return (color);
}
/*
converte un t_color in un int compatibile con mlx
restituisce un colore che puo essere usato da mlx
*/

unsigned int	create_mlx_color(t_color *color)
{
	return (clamp_color(color->a) << 24 | clamp_color(color->r) << 16
		| clamp_color(color->g) << 8 | clamp_color(color->b));
}
