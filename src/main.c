/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:51:18 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/31 13:41:15 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*void	init_display(t_display *disp, t_settings *settings)
{
	printf("Inizializzazione display...\n");
	disp->mlx = mlx_init();
	if (disp->mlx == NULL)
	{
		printf("Errore: MLX non inizializzato.\n");
		return ;
	}
	printf("MLX inizializzato correttamente.\n");
	disp->win = mlx_new_window(disp->mlx, settings->disp_w,
			settings->disp_h, "MiniRT");
	if (disp->win == NULL)
	{
		printf("Errore: finestra MLX non creata.\n");
		return ;
	}
	printf("Finestra creata con successo.\n");
	disp->display_img = mlx_new_image(disp->mlx, settings->render_w,
			settings->render_h);
	if (disp->display_img == NULL)
	{
		printf("Errore: immagine MLX non creata.\n");
		return ;
	}
	printf("Immagine creata con successo.\n");
	disp->disp_addr = mlx_get_data_addr(disp->display_img,
			&disp->bpp, &disp->line_length, &disp->endian);
	disp->bpp /= 8;
	printf("Indirizzo dell'immagine impostato.\n");
	mlx_do_key_autorepeatoff(disp->mlx);
}

void	init_settings(t_settings *settings)
{
	printf("Inizializzazione impostazioni...\n");
	settings->render_w = 1920;
	settings->render_h = 1080;
	settings->disp_w = 1920;
	settings->disp_h = 1080;
	settings->reflection_depth = 1;
	printf("Impostazioni inizializzate: risoluzione %dx%d\n",
		settings->render_w, settings->render_h);
}

void	setup_hooks(t_scene *scene)
{
	int	i;

	i = 0;
	while (i < scene->n_shapes && scene->shapes[i].type == PLANE)
		i++;
	if (i < scene->n_shapes)
	{
		scene->shapes[i].props.highlighted = true;
		scene->shape_idx = i;
	}
	else
		scene->shape_idx = -1;
		printf("Hook per la pressione dei tasti attivato.\n");
	mlx_hook(scene->disp->win, 2, (1L << 0), key_press, scene);
	mlx_hook(scene->disp->win, 3, (1L << 1), key_release, scene);
	mlx_hook(scene->disp->win, 17, 0, close_hook, scene);
	mlx_loop_hook(scene->disp->mlx, render_loop, scene);
}

void	print_usage(const char *prog)
{
	fprintf(stderr, "Usage: %s <scene.rt>\n", prog);
}

int	main(int argc, char **argv)
{
	t_scene		*scene;
	t_display	display;
	int			i;

	printf("Avvio del programma...\n");
	if (argc != 2 || !has_rt_extension(argv[1]))
	{
		print_usage(argv[0]);
		return (1);
	}
	printf("Allocazione memoria per la scena...\n");
	scene = ft_calloc(1, sizeof(*scene));
	if (!scene)
	{
		perror("calloc");
		return (1);
	}
	printf("Memoria allocata per la scena.\n");
	printf("Parsing del file %s...\n", argv[1]);
	if (parse_file(argv[1], scene) != 0)
	{
		fprintf(stderr, "Parsing fallito per '%s'\n", argv[1]);
		scene_free(scene);
		free(scene);
		return (1);
	}
	printf("Parsing completato con successo.\n");
	populate_shapes_array(scene);
	calculate_transforms(scene);
	printf("Inizializzazione delle impostazioni...\n");
	init_settings(&scene->settings);
	print_shapes(scene);
	printf("Inizializzazione del display...\n");
	init_display(&display, &scene->settings);
	if (!display.mlx)
	{
		fprintf(stderr, "Errore inizializzazione MLX\n");
		scene_free(scene);
		free(scene);
		return (1);
	}
	scene->disp = &display;
	printf("Inizializzazione della fotocamera...\n");
	camera_init(&scene->cam, scene);
	printf("Disegnando la scena...\n");
	printf("Pos automatico della cam sull’oggetto principale...\n");
	look_at(scene, &scene->shapes[0]);
	printf("Camera orientata verso la sfera!\n");
	draw_scene(scene);
	setup_hooks(scene);
	printf("Avvio del loop di MLX...\n");
	mlx_loop(display.mlx);
	scene_free(scene);
	free(scene);
	printf("Fine del programma.\n");
	return (0);
}*/
void	setup_hooks(t_scene *scene)
{
	int	i;

	i = 0;
	while (i < scene->n_shapes && scene->shapes[i].type == PLANE)
		i++;
	if (i < scene->n_shapes)
	{
		scene->shapes[i].props.highlighted = true;
		scene->shape_idx = i;
	}
	else
		scene->shape_idx = -1;
	mlx_hook(scene->disp->win, 2, (1L << 0), key_press, scene);
	mlx_hook(scene->disp->win, 3, (1L << 1), key_release, scene);
	mlx_hook(scene->disp->win, 17, 0, close_hook, scene);
	mlx_loop_hook(scene->disp->mlx, render_loop, scene);
}

void	print_usage(const char *prog)
{
	fprintf(stderr, "Usage: %s <scene.rt>\n", prog);
}

static int	init_scene(t_scene **scene, char *file)
{
	*scene = ft_calloc(1, sizeof(**scene));
	if (!*scene)
		return (perror("calloc"), 1);
	if (parse_file(file, *scene) != 0)
	{
		fprintf(stderr, "Parsing fallito per '%s'\n", file);
		scene_free(*scene);
		free(*scene);
		return (1);
	}
	populate_shapes_array(*scene);
	calculate_transforms(*scene);
	init_settings(&(*scene)->settings);
	return (0);
}

static int	init_camera_display(t_scene *scene, t_display *display)
{
	init_display(display, &scene->settings);
	if (!display->mlx)
	{
		fprintf(stderr, "Errore inizializzazione MLX\n");
		scene_free(scene);
		free(scene);
		return (1);
	}
	scene->disp = display;
	camera_init(&scene->cam, scene);
	look_at(scene, &scene->shapes[0]);
	return (0);
}

int	main(int argc, char **argv)
{
	t_scene		*scene;
	t_display	display;

	if (argc != 2 || !has_rt_extension(argv[1]))
		return (print_usage(argv[0]), 1);
	if (init_scene(&scene, argv[1]))
		return (1);
	if (init_camera_display(scene, &display))
		return (1);
	draw_scene(scene);
	setup_hooks(scene);
	printf("Avvio del loop di MLX...\n");
	mlx_loop(display.mlx);
	if (scene)
	{
		scene_free(scene);
		free(scene);
	}
	printf("Fine del programma.\n");
	return (0);
}
