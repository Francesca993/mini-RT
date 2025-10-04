/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:24:06 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/04 12:12:18 by francesca        ###   ########.fr       */
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

/* ================= Check File ================= */
int		has_rt_extension(const char *path);

/* ================= Parse File ================= */
int     parse_file(const char *path, t_scene *out);

#endif