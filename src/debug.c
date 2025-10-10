/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:26:12 by francesca         #+#    #+#             */
/*   Updated: 2025/10/10 15:54:25 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* debug_scene.c */
#include <stdio.h>
#include "miniRT.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:26:12 by francesca         #+#    #+#             */
/*   Updated: 2025/10/10 10:22:00 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "miniRT.h"

/* ---------- helper di stampa di base ---------- */

static void debug_print_bool(const char *label, bool value)
{
	if (label)
		printf("%s: %s\n", label, value ? "true" : "false");
}

static void debug_print_vector(const char *label, t_vector v)
{
	if (label)
		printf("%s: (x=%.6f, y=%.6f, z=%.6f)\n", label, v.x, v.y, v.z);
}

static void debug_print_color_linear01(const char *label, t_color c)
{
	if (label)
		printf("%s: (r=%.6f, g=%.6f, b=%.6f) [0..1]\n", label, c.r, c.g, c.b);
}

static void debug_print_color_255(const char *label, t_color c)
{
	int r255 = (int)(c.r * 255.0 + 0.5);
	int g255 = (int)(c.g * 255.0 + 0.5);
	int b255 = (int)(c.b * 255.0 + 0.5);

	if (r255 < 0)   r255 = 0;
	if (g255 < 0)   g255 = 0;
	if (b255 < 0)   b255 = 0;
	if (r255 > 255) r255 = 255;
	if (g255 > 255) g255 = 255;
	if (b255 > 255) b255 = 255;

	if (label)
		printf("%s: (R=%d, G=%d, B=%d) [0..255]\n", label, r255, g255, b255);
}

/* ---------- stampa oggetto singolo (nodo lista) ---------- */

static void debug_print_object_node(const t_objnode *node, size_t index_in_list)
{
	if (!node)
		return;

	printf("  - Object[%zu]\n", index_in_list);

	switch (node->type)
	{
		case OBJ_SPHERE:
			printf("    type: SPHERE\n");
			debug_print_vector("    center", node->fig.sphere.center);
			printf("    radius: %.6f\n", node->fig.sphere.radius);
			debug_print_color_linear01("    color linear", node->fig.sphere.color);
			debug_print_color_255("    color 255", node->fig.sphere.color);
			break;

		case OBJ_PLANE:
			printf("    type: PLANE\n");
			debug_print_vector("    point",  node->fig.plane.point);
			debug_print_vector("    normal", node->fig.plane.normal);
			debug_print_color_linear01("    color linear", node->fig.plane.color);
			debug_print_color_255("    color 255", node->fig.plane.color);
			break;

		case OBJ_CYLINDER:
			printf("    type: CYLINDER\n");
			debug_print_vector("    base", node->fig.cylinder.base);
			debug_print_vector("    axis", node->fig.cylinder.axis);
			printf("    radius: %.6f\n", node->fig.cylinder.radius);
			printf("    height: %.6f\n", node->fig.cylinder.height);
			debug_print_color_linear01("    color linear", node->fig.cylinder.color);
			debug_print_color_255("    color 255", node->fig.cylinder.color);
			break;

		default:
			printf("    type: <unknown>\n");
			break;
	}
}

/* ---------- stampa completa scena ---------- */

void debug_print_scene(const t_scene *scene_ptr, const char *title)
{
	const t_objnode *it;
	size_t           i = 0;

	if (!scene_ptr)
	{
		printf("debug_print_scene: scena nulla\n");
		return;
	}

	/* intestazione */
	if (title)
		printf("========== %s ==========\n", title);
	else
		printf("========== Scene Debug ==========\n");

	/* --- Ambient --- */
	printf("[Ambient]\n");
	debug_print_bool("present", scene_ptr->amb.present);
	printf("ratio: %.6f\n", scene_ptr->amb.ratio);
	debug_print_color_linear01("color linear", scene_ptr->amb.color);
	debug_print_color_255("color 255", scene_ptr->amb.color);
	printf("\n");

	/* --- Camera --- */
	printf("[Camera]\n");
	debug_print_bool("present", scene_ptr->cam.present);
	debug_print_vector("pos", scene_ptr->cam.pos);
	debug_print_vector("dir (normalized)", scene_ptr->cam.dir);
	printf("fov_rad: %.6f\n", scene_ptr->cam.fov_rad);
	printf("\n");

	/* --- Light (mandatory: una sola) --- */
	printf("[Light]\n");
	debug_print_bool("present", scene_ptr->light.present);
	debug_print_vector("pos", scene_ptr->light.pos);
	printf("intensity: %.6f\n", scene_ptr->light.intensity);
	debug_print_color_linear01("color linear", scene_ptr->light.color);
	debug_print_color_255("color 255", scene_ptr->light.color);
	printf("\n");

	/* --- Contatori di validazione --- */
	printf("[Counts for validation]\n");
	printf("n_ambient: %d\n", scene_ptr->n_ambient);
	printf("n_camera: %d\n", scene_ptr->n_camera);
	printf("n_lights: %d\n", scene_ptr->n_lights);
	printf("n_spheres: %d\n", scene_ptr->n_spheres);
	printf("n_planes: %d\n", scene_ptr->n_planes);
	printf("n_cylinders: %d\n", scene_ptr->n_cylinders);
	printf("\n");

	/* --- Oggetti (lista) --- */
	printf("[Objects list]\n");
	printf("objects_count: %zu\n", scene_ptr->object_count);
	if (scene_ptr->obj_head == NULL)
	{
		printf("  (empty)\n");
	}
	else
	{
		it = scene_ptr->obj_head;
		while (it)
		{
			debug_print_object_node(it, i);
			it = it->next;
			++i;
		}
	}
	printf("=================================\n");
}

