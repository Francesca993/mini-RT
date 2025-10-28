/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:51:18 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/27 16:22:38 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"


void	init_display(t_display *disp, t_settings *settings)
{
	printf("Inizializzazione display...\n");

	disp->mlx = mlx_init();
	if (disp->mlx == NULL)
	{
		printf("Errore: MLX non inizializzato.\n");
		return;
	}
	printf("MLX inizializzato correttamente.\n");

	disp->win = mlx_new_window(disp->mlx, settings->disp_w,
			settings->disp_h, "MiniRT");

	if (disp->win == NULL)
	{
		printf("Errore: finestra MLX non creata.\n");
		return;
	}
	printf("Finestra creata con successo.\n");

	disp->display_img = mlx_new_image(disp->mlx, settings->render_w,
			settings->render_h);

	if (disp->display_img == NULL)
	{
		printf("Errore: immagine MLX non creata.\n");
		return;
	}
	printf("Immagine creata con successo.\n");

	disp->disp_addr = mlx_get_data_addr(disp->display_img,
			&disp->bpp, &disp->line_length, &disp->endian);

	disp->bpp /= 8;
	printf("Indirizzo dell'immagine impostato.\n");
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

void setup_hooks(t_scene *scene)
{
    mlx_key_hook(scene->disp->win, key_hook, scene);  
    mlx_hook(scene->disp->win, 17, 0, close_hook, scene);
}

void	print_usage(const char *prog)
{
	fprintf(stderr, "Usage: %s <scene.rt>\n", prog);
}

int	main(int argc, char **argv)
{
	t_scene		*scene;
	t_display	display;

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
	//print_shapes(scene); 
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
	
	printf("Posizionamento automatico della camera sull’oggetto principale...\n");
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
}
