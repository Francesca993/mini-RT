/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:29:04 by francesca         #+#    #+#             */
/*   Updated: 2025/10/02 09:39:06 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/miniRT.h"
#include <string.h>

//Controlla che il file sia .rt
//Check if the file has the extenctrion .rt
int has_rt_extension(const char *path)
{
	size_t len;

	if (path == NULL)
		return (0);
	len = ft_strlen(path);
	if (len < 3)
		return (0);
	if (ft_strcmp(path + len - 3, ".rt") == 0)
		return (1);
	return (0);
}
