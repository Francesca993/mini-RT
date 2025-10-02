/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:51:18 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/02 09:51:12 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/miniRT.h"

int	main(int argc, char **argv)
{
	t_scene *sc;
	
	if (argc != 2)
		return (1, print_usage(argv[0]));
	
	// io vorrei fare una funzione che fa un check generale su tutto il file subito in realta 
	//ma per ora mettto questa qui
	if (!has_rt_extension(argv[1])) 
		return (1, print_usage(argv[0]));
	
	sc = ft_calloc(1, sizeof(*sc));
    if (!sc)
        return (perror("calloc"), 1);
	if (parse_file(argv[1], sc) != 0) 
	{
        fprintf(stderr, "Parsing fallito per '%s'\n", argv[1]);
        free(sc);
        return 1;
    }
	    // Per ora solo feedback:
    printf("OK: A=%d C=%d L=%d sp=%d pl=%d cy=%d\n",
           sc->n_ambient, sc->n_camera, sc->n_lights,
           sc->n_spheres, sc->n_planes, sc->n_cylinders);

    // TODO: render / mlx init ...

    free(sc);
    return 0;
}