/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:24:06 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/07 14:28:03 by francesca        ###   ########.fr       */
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
void	chop_newline(char *s); // Rimuove /n finale
void	scene_free(t_scene *scene); // Libera la Union, gli objects della t_scene
int     parse_double(const char **line, double *out);
int     parse_rgb(const char **input_ptr, int *out_red, int *out_green, int *out_blue);
const char *skip_spaces(const char *p);

/* ================= Debug ================= */
void debug_print_scene(const t_scene *scene_ptr, const char *title);

/* ================= Free ================= */
void    free_array(char **arr);

/* ================= ft_skip_spaces ================= */
int ft_is_blank(const char *s);
int ft_is_space_char(char c);
const char *skip_spaces(const char *p);

/* ================= Check File ================= */
int		has_rt_extension(const char *path); // controlla che abbia l'estensione .rt
int     check_startingscene(t_scene *scene); // controlla che cia sia una sola A, L, C e almeno una sp, cy, pl
int     is_valid_identifier(const char *id);  // Ritorna 1 se id è uno dei token ammessi nel mandatory
int     parse_ambient_line(t_scene *scene, char *rest_of_line);

/* ================= Parse File ================= */
int     parse_file(const char *path, t_scene *scene);

/* ================= Lexer ================= */
int lex_scan_check_and_count(t_scene *scene, char *line); ///* Prima scansione della matrice e aggiornamento contatori + duplicati A/C/L */
#endif
