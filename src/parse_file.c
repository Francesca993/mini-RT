/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:48:43 by francesca         #+#    #+#             */
/*   Updated: 2025/10/04 15:48:42 by francesca        ###   ########.fr       */
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
/*-- Copia il file fd dentro una matrice, saltando righe vuote e spazi --*/
static char	**alloc_data(size_t num_line, const char *path)
{
	char	**data;
	char	*line;
	size_t	i;
	int		fd;

	i = 0;
	data = ft_calloc(num_line + 1, sizeof(char *));
	if (!data)
		return (NULL);
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("miniRT: open");
		free(data);
		return (NULL);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		if (ft_is_blank(line)) // <-- salta riga vuota o spazi
		{
			free(line);
			continue ; // salta le righe vuote
		}
		// più righe utili del previsto → errore
		// if (i >= num_line)
		// {
		// 	free(line);
		// 	close(fd);
		// 	free_strarray(data);
		// 	return (NULL);
		// }
		chop_newline(line); // Rimuove New line finale e mette '0'
		data[i++] = ft_strdup(line); // copia la riga utile
		free(line);
	}
	if (close(fd) == -1)
	{
		perror("miniRT: close");
		free_array(data);
		return (NULL);
	}
	data[i] = NULL; // terminatore
	return (data);
}

int	parse_file(const char *path, t_scene *scene)
{
	char	**data;

	size_t num_line; // Contatore del numero di riga
	data = NULL;
	if (!path || !scene)
	{
		printf("parse_file: argomenti non validi\n");
		return (1);
	}
	// potrei inserire direttamente qui il controllo del .rt
	scene_reset(scene); // reset contatori
	num_line = count_line(0, path);
	if (num_line == (size_t)-1)
		return (1);
	// vado a mettere i dati nella matrice
	data = alloc_data(num_line, path);
	if (!data)
		return (1);
	/*
	**
	** ORA QUI METTERO UNA FUNZIONE CHE PRENDE DATA E LA T_SCENE E FA TUTTO
	**
	*/
	// DEBUG
	for (size_t i = 0; data && data[i]; i++)
		printf("riga %zu: %s\n", i, data[i]);
	// funzione che serve per check unicità e che ci sia almeno un elemento
	// if (check_startingscene(data) != 0)
	//     return (1);
	return (0);
}
