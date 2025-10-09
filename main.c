/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:51:18 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/09 11:48:08 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/miniRT.h"

int	main(int argc, char **argv)
{
	t_scene	*scene;

	if (argc != 2)
	{
		print_usage(argv[0]);
		return (1);
	}
	/** Check che il file abbia estensione .rt */
	if (!has_rt_extension(argv[1]))
	{
		print_usage(argv[0]);
		return (1);
	}
	scene = ft_calloc(1, sizeof(*scene));
	if (!scene)
	{
		perror("calloc");
		// free(scene);
		return (1);
	}
	if (parse_file(argv[1], scene) != 0)
	{
		fprintf(stderr, "Parsing fallito per '%s'\n", argv[1]);
		scene_free(scene);
		free(scene);
		return (1);
	}
	// TODO: render / mlx init ...
	// if (scene != NULL)
	// 	debug_print_scene(scene, "DEBUG SULLA FINE DEL MAIN");
	/*
	La struttura t_scene stessa viene allocata in main con ft_calloc(
	La chiamata a free(scene) a fine main è necessaria,
	indipendentemente da come vada il parsing.
	I dati dinamici interni alla scena (in particolare l’array scene->objects): questi vanno liberati
	prima di liberare la struttura, altrimenti ci sarà una perdita di memoria.
	*/
	scene_free(scene);
	free(scene);
	return (0);
}
