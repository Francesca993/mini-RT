/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:51:18 by fmontini          #+#    #+#             */
/*   Updated: 2025/09/29 11:46:29 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/miniRT.h"

int	main(void)
{
	t_app a;

	a.mlx = mlx_init();
	if (!a.mlx)
		return (1);

	a.win = mlx_new_window(a.mlx, 880, 600, "miniRT");
	if (!a.win)
		return (1);

	/* finestra nera: non disegniamo nulla */
	mlx_clear_window(a.mlx, a.win);

	/* hook chiusura */
	mlx_hook(a.win, 17, 0, close_hook, &a);
	mlx_key_hook(a.win, key_hook, &a);

	mlx_loop(a.mlx);
	return (0);
}