/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:24:06 by fmontini          #+#    #+#             */
/*   Updated: 2025/09/29 11:49:01 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "mlx.h"
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <math.h>
# include <stdio.h> 
# include "/nfs/homes/fmontini/Desktop/miniRT/Libft/libft.h"


/* ================= Config ================= */
# ifndef WIDTH
#  define WIDTH 800
# endif
# ifndef HEIGHT
#  define HEIGHT 600
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

/* ================= Struct base =============== */
typedef struct s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

typedef struct s_color
{
	double r; /* 0..1 */
	double			g;
	double			b;
}					t_color;

typedef struct s_img
{
	void			*ptr;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	int				w;
	int				h;
}					t_img;

typedef struct s_camera
{
	t_vec3			pos;
	t_vec3			dir;
	double			fov_deg;
}					t_camera;

typedef struct s_light
{
	t_vec3			pos;
	double			intensity;
	t_color			color;
	struct s_light	*next;
}					t_light;

typedef enum e_objtype
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER
}					t_objtype;

typedef struct s_sphere
{
	t_vec3			c;
	double			radius;
}					t_sphere;
typedef struct s_plane
{
	t_vec3			p;
	t_vec3			n;
}					t_plane;
typedef struct s_cyl
{
	t_vec3			c;
	t_vec3			axis;
	double			radius;
	double			height;
}					t_cyl;

typedef struct s_object
{
	t_objtype		type;
	t_color			color;
	union
	{
		t_sphere	sp;
		t_plane		pl;
		t_cyl		cy;
	} u;
	struct s_object	*next;
}					t_object;

typedef struct s_scene
{
	double			ambient_ratio;
	t_color			ambient_color;
	t_camera		camera;
	t_light			*lights;
	t_object		*objects;
}					t_scene;

typedef struct s_app
{
	void			*mlx;
	void			*win;
	t_img			img;
	t_scene			scene;
	char			*scene_path;
}					t_app;

/* ================= Hook ================= */
int	close_hook(void *param);
int	key_hook(int keycode, void *param);

#endif /* RT_H */