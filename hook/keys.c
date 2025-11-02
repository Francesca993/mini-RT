/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 20:08:36 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 18:04:25 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/* Gestione dei tasti aggiuntivi W/A/S/D e frecce */
void	toggle_keys_held2(int key, t_scene *scene, bool on_off)
{
	if (key == KEY_W)
		scene->keys_held.w = on_off;
	if (key == KEY_A)
		scene->keys_held.a = on_off;
	if (key == KEY_S)
		scene->keys_held.s = on_off;
	if (key == KEY_D)
		scene->keys_held.d = on_off;
	if (key == KEY_UP)
		scene->keys_held.up = on_off;
	if (key == KEY_DOWN)
		scene->keys_held.down = on_off;
	if (key == KEY_LEFT)
		scene->keys_held.left = on_off;
	if (key == KEY_RIGHT)
		scene->keys_held.right = on_off;
}

/* Gestione altri tasti */
void	toggle_keys_held(int key, t_scene *scene, bool on_off)
{
	if (key == KEY_PLUS)
		scene->keys_held.plus = on_off;
	if (key == KEY_MINUS)
		scene->keys_held.minus = on_off;
	if (key == KEY_R)
	{
		scene->keys_held.r = on_off;
	}
	toggle_keys_held2(key, scene, on_off);
}

/* Cambia shape selezionata in edit mode */
void	select_shape(int key, t_scene *scene)
{
	if (key == KEY_TAB && scene->settings.edit_mode == true)
	{
		scene->shapes[scene->shape_idx].props.highlighted = false;
		scene->shape_idx = (scene->shape_idx + 1) % scene->n_shapes;
		while (scene->shapes[scene->shape_idx].type == PLANE)
			scene->shape_idx = (scene->shape_idx + 1) % scene->n_shapes;
		scene->shapes[scene->shape_idx].props.highlighted = true;
	}
}

/* Gestione pressione dei tasti */
int	key_press(int key, t_scene *scene)
{
	if (key == KEY_TAB)
	{
		select_shape(key, scene);
	}
	if (is_toggle_key(key, scene))
	{
		toggle_edit_mode(key, scene);
		return (0);
	}
	if (key == KEY_ESC)
	{
		key_hook(key, scene);
		return (0);
	}
	toggle_keys_held(key, scene, true);
	return (0);
}

/* Gestione rilascio dei tasti */
int	key_release(int key, t_scene *scene)
{
	toggle_keys_held(key, scene, false);
	return (0);
}
