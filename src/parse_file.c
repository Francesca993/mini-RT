/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:48:43 by francesca         #+#    #+#             */
/*   Updated: 2025/10/06 21:51:02 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	scene_reset(t_scene *scene)
{
	scene->n_ambient = 0;
	scene->n_camera = 0;
	scene->n_lights = 0;
	scene->n_spheres = 0;
	scene->n_planes = 0;
	scene->n_cylinders = 0;
	return ;
}

int	parse_file(const char *path, t_scene *scene)
{
	char	**data;	char	*line;
	ssize_t	i;
	int		fd;

	if (!path || !scene)
	{
		printf("parse_file: argomenti non validi\n");
		return (1);
	}
	
	i = 0;
	scene_reset(scene); // reset contatori
	data = ft_calloc(num_line + 1, sizeof(char *));
	if (!data)
		return (1);
	
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("miniRT: open");
		free(data);
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		if (ft_is_blank(line)) // <-- salta riga vuota o spazi
		{
			free(line);
			continue ; // salta le righe vuote
		}
		chop_newline(line); // Rimuove New line finale e mette '0'
		/*-------	QUI POTREI INSERIRE I CONTROLLI DIRETTAMENTE SULLA LINEA APPENA LETTA -----------*/
		/*---------- INVECE CHE COPIARLI NELLA MATRICE -------------- */
		if (lex_scan_check_and_count(scene, line) == -1)
			return(1);
	}
	if (close(fd) == -1)
	{
		perror("miniRT: close");
		free_array(data);
		return (1);
	}
	return (0);
}
