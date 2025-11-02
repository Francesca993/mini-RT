/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:48:43 by francesca         #+#    #+#             */
/*   Updated: 2025/11/02 14:33:27 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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

/*
Reads the line with get_next_line (gnl), skips empty ones, initializes the
scene, strips the trailing '\n' to '\0', and starts the routine that,
while reading line by line, also performs real-time requirement checks.
 */
int	parse_file(const char *path, t_scene *scene)
{
	char	*line;
	int		fd;

	if (!path || !scene)
		return (print_err_msg("parse_file: invalid arguments."));
	if (!scene_reset(scene))
		return (print_err_msg("scene allocation."));
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (print_err_msg("miniRT: open"));
	while ((line = get_next_line(fd)) != NULL)
	{
		if (ft_is_blank(line))
		{
			free(line);
			continue ;
		}
		chop_newline(line);
		if (lex_scan_check_and_count(scene, line) != 0)
		{
			free(line);
			close(fd);
			return (1);
		}
		free(line);
	}
	if (close(fd) == -1)
		return (print_err_msg("Error: miniRT: close"));
	if (check_presence(scene) == 1)
		return (1);
	return (0);
}
