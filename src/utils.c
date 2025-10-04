/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:25:56 by francesca         #+#    #+#             */
/*   Updated: 2025/10/04 15:45:06 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	print_usage(const char *prog)
{
	fprintf(stderr, "Usage: %s <scene.rt>\n", prog);
}
void free_array(char **arr)
{
    size_t i = 0;
    if (!arr) return;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}
/*Conta le righe del fd saltando quelle vuote, mi serve all inzio per creare una 
matrice dati dentro cui "copiare" solo i dati utili del FD*/
int	count_line(size_t num_line, const char *path)
{
	char	*line;
	int 	fd;

	if (!path)
	{
		printf("parse_file: argomenti non validi\n");
		return (-1);
	}
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("miniRT: open");
		return (-1);
	}
	num_line = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		if (ft_is_blank(line)) // <-- controlla se riga è vuota o spazi
		{
			free(line);
			continue ; // salta le righe vuote
		}
		num_line++;
		free(line);
	}
	if (close(fd) == -1)
	{
		perror("miniRT: close");
		return (-1);
	}
	return (num_line);
}

/* Rimuove New_line finale */
void	chop_newline(char *s)

{
	size_t	n;

	if (!s)
		return ;
	n = ft_strlen(s);
	if (n > 0 && s[n - 1] == '\n')
		s[n - 1] = '\0';
	return ;
}
