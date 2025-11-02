/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:24:06 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/31 14:22:12 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../Libft/libft.h"
# include "../mlx/mlx.h"
# include "miniRT_struct.h"
# include "mathematical.h"
# include <limits.h>
# include <math.h>
# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <string.h>

# define KEY_ESC 65307
# define KEY_LEFT 65361 // rotations
# define KEY_RIGHT 65363
# define KEY_DOWN 65364
# define KEY_UP 65362
# define KEY_SPACE 32  // edit mode
# define KEY_W 119     // front
# define KEY_A 97      // left
# define KEY_S 115     // back
# define KEY_D 100     // right
# define KEY_PLUS 61   // increase scale
# define KEY_MINUS 45  // decrease
# define KEY_TAB 65289 // change selected object
# define KEY_R 114     // rotate Z
# define KEY_O 111     // for look at

/* ================= Hook ================= */
int			close_hook(void *param);
int			key_hook(int keycode, void *param);

/* ================= Utils ================= */
void		print_usage(const char *prog);
void		chop_newline(char *s);
void		scene_free(t_scene *scene);
int			parse_double(const char **line, double *out);
int			parse_rgb(const char **input_ptr, t_color *out_color);
int			parse_vec3(const char **input_ptr, t_vector *out_vec,
				double normalized);
int			skip_comma(const char **input_ptr);
int			parse_int(const char **line_pointer, int *out_value);

/* ================= Debug ================= */
void		debug_print_scene(const t_scene *scene_ptr, const char *title);
void		print_shapes(t_scene *scene);

/* ================= Free ================= */
void		free_array(char **arr);
int			print_err_msg(const char *msg);
int			err_msg(char *type, int num, const char *msg);
int			scene_reset(t_scene *scene);

/* ================= ft_skip_spaces ================= */
int			ft_is_blank(const char *s);
int			ft_is_space_char(char c);
const char	*skip_spaces(const char *p);

/* ================= Check File ================= */
int			has_rt_extension(const char *path);
int			check_startingscene(t_scene *scene);
int			is_valid_identifier(const char *id);
int			check_vec3direction(t_vector *direction_value);
int			check_presence(t_scene *scene);

/* ================= Parse File ================= */
int			parse_file(const char *path, t_scene *scene);
int			parse_ambient_line(t_scene *scene, char *rest_of_line);
int			parse_camera_line(t_scene *scene, char *rest_of_line);
int			parse_light_line(t_scene *scene, char *rest_of_line);
int			parse_sphere(t_scene *scene, char *rest_of_line);
int			parse_plane(t_scene *scene, char *rest_of_line);
int			parse_cylinder(t_scene *scene, char *rest_of_line);

/* ================= Lexer ================= */
int			lex_scan_check_and_count(t_scene *scene, char *line);

/* ================= Linked List Utils ================= */
void		objlist_init(t_scene *scene);
void		objlist_destroy(t_scene *scene);
t_objnode	*objnode_new(t_shape_type type, t_figures fig);
int			object_list_append(t_scene *scene, t_shape_type object_type,
				t_figures object_payload);

/* ================= Init utils ================= */
void		init_settings(t_settings *settings);
void		init_display(t_display *disp, t_settings *settings);

#endif