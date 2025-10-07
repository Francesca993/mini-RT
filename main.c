/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:51:18 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/07 16:06:07 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/miniRT.h"

int	main(int argc, char **argv)
{
	t_scene *scene;
	
    if (argc != 2)
    {
        print_usage(argv[0]);
        return (1);
    }
	
	// io vorrei fare una funzione che fa un check generale su tutto il file subito in realta 
	//ma per ora mettto questa qui
    if (!has_rt_extension(argv[1]))
    {
        print_usage(argv[0]);
        return (1);
    }
	
    scene = ft_calloc(1, sizeof(*scene));
    if (!scene)
    {
        perror("calloc");
        return (1);
    }
	if (parse_file(argv[1], scene) != 0) 
	{
        fprintf(stderr, "Parsing fallito per '%s'\n", argv[1]);
        free(scene);
        return (1);
    }
    // TODO: render / mlx init ...
    if (scene != NULL)
        debug_print_scene(scene, "DEBUG SULLA FINE DEL MAIN");
    free(scene);
    return (0);
}