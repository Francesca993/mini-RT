/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:51:18 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/04 15:56:05 by francesca        ###   ########.fr       */
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
    //Al momento il parser legge il file rt e manda le righe non vuote in una matrice
    //su cui poi farò i vari test
	if (parse_file(argv[1], scene) != 0) 
	{
        fprintf(stderr, "Parsing fallito per '%s'\n", argv[1]);
        free(scene);
        return 1;
    }
	//     // Per ora solo feedback:
    // printf("OK: A=%d C=%d L=%d sp=%d pl=%d cy=%d\n",
    //        scene->n_ambient, scene->n_camera, scene->n_lights,
    //        scene->n_spheres, scene->n_planes, scene->n_cylinders);

    // TODO: render / mlx init ...

    free(scene);
    return 0;
}