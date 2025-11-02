/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 19:11:03 by francesca         #+#    #+#             */
/*   Updated: 2025/11/02 14:22:49 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ---- HELPER ---- */
/*
Creates a t_vector variable, assigns the x, y, z and w values to its fields,
and returns it.
*/
static inline t_vector	vector_make(double x, double y, double z, double w)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return (v);
}

/*
Creates a t_color with the given r, g, b, a components and returns it.
*/
static inline t_color	color_make(double r, double g, double b, double t)
{
	t_color	c;

	c.r = r;
	c.g = g;
	c.b = b;
	c.a = t;
	return (c);
}

static inline void	camera_make(t_scene *scene)
{
	scene->cam.pos = vector_make(0.0, 0.0, 0.0, 0.0);
	scene->cam.dir = vector_make(0.0, 0.0, 0.0, 0.0);
	scene->cam.fov = 0.0;
	scene->cam.fov_deg = 0;
	scene->cam.present = false;
	scene->cam.pixel_size = 0.0;
	scene->cam.half_width = 0.0;
	scene->cam.half_height = 0.0;
	ft_bzero(&scene->cam.transform, sizeof(scene->cam.transform));
	ft_bzero(&scene->cam.inv_trans, sizeof(scene->cam.inv_trans));
}

static inline void	settings_make(t_scene *scene)
{
	scene->settings.render_w = 0;
	scene->settings.render_h = 0;
	scene->settings.edit_w = 0;
	scene->settings.edit_h = 0;
	scene->settings.reflection_depth = 0;
}

static inline int	light_init(t_scene *scene)
{
	if (scene->lights)
	{
		free(scene->lights);
		scene->lights = NULL;
	}
	scene->lights = ft_calloc(1, sizeof(*scene->lights));
	if (!scene->lights)
		return (print_err_msg("allocazione luce fallita"));
	scene->lights[0].position = vector_make(0.0, 0.0, 0.0, 0.0);
	scene->lights[0].intensity = 0.0;
	scene->lights[0].color = color_make(0.0, 0.0, 0.0, 0.0);
	scene->lights[0].present = false;
	return (0);
}
/* Initializes/resets the entire scene,
   freeing any previously allocated storage. */
int	scene_reset(t_scene *scene)
{
	objlist_destroy(scene);
	objlist_init(scene);
	scene->ambient.intensity = 0.0;
	scene->ambient.color = color_make(0.0, 0.0, 0.0, 0.0);
	scene->ambient.present = false;
	camera_make(scene);
	if (light_init(scene) == 1)
		return (0);
	settings_make(scene);
	return (1);
}
