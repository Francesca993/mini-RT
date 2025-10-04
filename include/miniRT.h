/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:24:06 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/04 15:12:37 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "miniRT_struct.h"
# include <math.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include "../Libft/libft.h"

/* ================= Config ================= */
# ifndef WIDTH
#  define WIDTH 1200
# endif
# ifndef HEIGHT
#  define HEIGHT 800
# endif

# define EPSILON 1e-6
# define PI 3.14159265358979323846

/* ================= Keycodes ================= */
# ifdef __APPLE__
#  define KEY_ESC 53
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_DOWN 125
#  define KEY_UP 126
# else
#  define KEY_ESC 65307
#  define KEY_LEFT 65361
#  define KEY_RIGHT 65363
#  define KEY_DOWN 65364
#  define KEY_UP 65362
# endif

/* ================= Hook ================= */
int		close_hook(void *param);
int		key_hook(int keycode, void *param);

/* ================= Utils ================= */
void	print_usage(const char *prog);
void    free_array(char **arr);
int	    count_line(size_t num_line, char *path);

/* ================= Check File ================= */
int		has_rt_extension(const char *path); // controlla che abbia l'estensione .rt
int     check_startingscene(t_scene *scene); // controlla che cia sia una sola A, L, C e almeno una sp, cy, pl

/* ================= Parse File ================= */
int     parse_file(const char *path, t_scene *scene);
char	**alloc_data(data, num_line, path); // Copia il file fd dentro una matrice, saltando righe vuote e spazi

#endif