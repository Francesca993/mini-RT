/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:26:12 by francesca         #+#    #+#             */
/*   Updated: 2025/10/08 12:04:51 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* debug_scene.c */
#include <stdio.h>
#include "miniRT.h"

/* ---------- helper di stampa di base ---------- */

static void debug_print_bool(const char *label, bool value)
{
	if (label != NULL)
		printf("%s: %s\n", label, value ? "true" : "false");
}

static void debug_print_vector(const char *label, t_vector v)
{
	if (label != NULL)
		printf("%s: (x=%.6f, y=%.6f, z=%.6f)\n", label, v.x, v.y, v.z);
}

static void debug_print_color_linear01(const char *label, t_color c)
{
	if (label != NULL)
		printf("%s: (r=%.6f, g=%.6f, b=%.6f) [0..1]\n", label, c.r, c.g, c.b);
}

static void debug_print_color_255(const char *label, t_color c)
{
	int r255;
	int g255;
	int b255;

	r255 = (int)(c.r * 255.0 + 0.5);
	g255 = (int)(c.g * 255.0 + 0.5);
	b255 = (int)(c.b * 255.0 + 0.5);
	if (r255 < 0) r255 = 0;
	if (g255 < 0) g255 = 0;
	if (b255 < 0) b255 = 0;
	if (r255 > 255) r255 = 255;
	if (g255 > 255) g255 = 255;
	if (b255 > 255) b255 = 255;

	if (label != NULL)
		printf("%s: (R=%d, G=%d, B=%d) [0..255]\n", label, r255, g255, b255);
}

/* ---------- stampa oggetto singolo ---------- */

static void debug_print_object(const t_object *object_ptr, size_t index_in_array)
{
	if (object_ptr == NULL)
		return;

	printf("  - Object[%zu]\n", index_in_array);

	/* Attenzione: nel tuo header hai avuto una piccola incongruenza tra t_objtype e t_object_type.
	   Qui assumo che l’enum usi i simboli: OBJ_SPHERE, OBJ_PLANE, OBJ_CYLINDER
	   e che il campo si chiami 'type'. Assicurati che i nomi combacino. */

	if (object_ptr->type == OBJ_SPHERE)
	{
		printf("    type: SPHERE\n");
		debug_print_vector("    center", object_ptr->as.sphere.center);
		printf("    radius: %.6f\n", object_ptr->as.sphere.radius);
		debug_print_color_linear01("    color linear", object_ptr->as.sphere.color);
		debug_print_color_255("    color 255", object_ptr->as.sphere.color);
	}
	else if (object_ptr->type == OBJ_PLANE)
	{
		printf("    type: PLANE\n");
		debug_print_vector("    point", object_ptr->as.plane.point);
		debug_print_vector("    normal", object_ptr->as.plane.normal);
		debug_print_color_linear01("    color linear", object_ptr->as.plane.color);
		debug_print_color_255("    color 255", object_ptr->as.plane.color);
	}
	else if (object_ptr->type == OBJ_CYLINDER)
	{
		printf("    type: CYLINDER\n");
		debug_print_vector("    base", object_ptr->as.cylinder.base);
		debug_print_vector("    axis", object_ptr->as.cylinder.axis);
		printf("    radius: %.6f\n", object_ptr->as.cylinder.radius);
		printf("    height: %.6f\n", object_ptr->as.cylinder.height);
		debug_print_color_linear01("    color linear", object_ptr->as.cylinder.color);
		debug_print_color_255("    color 255", object_ptr->as.cylinder.color);
	}
	else
	{
		printf("    type: <unknown>\n");
	}
}

/* ---------- stampa completa scena ---------- */

void debug_print_scene(const t_scene *scene_ptr, const char *title)
{
	size_t i;

	if (scene_ptr == NULL)
	{
		printf("debug_print_scene: scena nulla\n");
		return;
	}

	if (title != NULL)
	{
		printf("========== %s ==========\n", title);
	}
	else
	{
		printf("========== Scene Debug ==========\n");
	}

	/* --- Sezione Ambient --- */
	printf("[Ambient]\n");
	debug_print_bool("present", scene_ptr->amb.present);
	printf("ratio: %.6f\n", scene_ptr->amb.ratio);
	debug_print_color_linear01("color linear", scene_ptr->amb.color);
	debug_print_color_255("color 255", scene_ptr->amb.color);
	printf("\n");

	/* --- Sezione Camera --- */
	printf("[Camera]\n");
	debug_print_bool("present", scene_ptr->cam.present);
	debug_print_vector("pos", scene_ptr->cam.pos);
	debug_print_vector("dir (normalized)", scene_ptr->cam.dir);
	printf("fov_rad: %.6f\n", scene_ptr->cam.fov_rad);
	printf("\n");

	/* --- Sezione Light (mandatory: una sola) --- */
	printf("[Light]\n");
	debug_print_bool("present", scene_ptr->light.present);
	debug_print_vector("pos", scene_ptr->light.pos);
	printf("intensity: %.6f\n", scene_ptr->light.intensity);
	debug_print_color_linear01("color linear", scene_ptr->light.color);
	debug_print_color_255("color 255", scene_ptr->light.color);
	printf("\n");

	/* --- Sezione contatori di validazione --- */
	printf("[Counts for validation]\n");
	printf("n_ambient: %d\n", scene_ptr->n_ambient);
	printf("n_camera: %d\n", scene_ptr->n_camera);
	printf("n_lights: %d\n", scene_ptr->n_lights);
	printf("n_spheres: %d\n", scene_ptr->n_spheres);
	printf("n_planes: %d\n", scene_ptr->n_planes);
	printf("n_cylinders: %d\n", scene_ptr->n_cylinders);
	printf("\n");

	/* --- Sezione oggetti --- */
	printf("[Objects array]\n");
	printf("objects_count: %zu\n", scene_ptr->object_count);
	printf("objects_capacity: %zu\n", scene_ptr->object_cap);
	for (i = 0; i < scene_ptr->object_count; i++)
	{
		debug_print_object(&scene_ptr->objects[i], i);
	}
	printf("=================================\n");
}
