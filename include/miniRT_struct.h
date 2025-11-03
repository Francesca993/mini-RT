/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:29:54 by francesca         #+#    #+#             */
/*   Updated: 2025/11/03 17:08:04 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_STRUCT_H
# define MINIRT_STRUCT_H

# include <stdbool.h>
# include <stddef.h>

# define MAX_LIGHTS 100
# define MAX_SHAPES 100

# define NUM_THREADS 16

typedef struct s_numparse
{
	const char					*cursor;
	int							sign;
	double						val;
	double						frac;
	double						scale;
	int							ndigits;
	int							nfrac;
}								t_numparse;

/* --- COORDINATES --- */
typedef struct s_vector
{
	double						x;
	double						y;
	double						z;
	double						w;
}								t_vector;

/* --- COLORS --- */
typedef struct s_color
{
	double						r;
	double						g;
	double						b;
	double						a;
}								t_color;

typedef struct s_shape			t_shape;
typedef struct s_phong			t_phong;
struct							s_phong
{
	t_color						effective_color;
	t_color						diffuse;
	t_color						specular;
	t_color						ambient;
};

typedef struct s_ray			t_ray;
struct							s_ray
{
	t_vector					origin;
	t_vector					dir;
};

/*-- INTERSECTIONS --*/
typedef struct s_intersect		t_intersection;
struct							s_intersect
{
	double						time;
	t_shape						*shape;
	t_vector					point;
	t_vector					normal;
	t_vector					eye;
	t_vector					over_point;
	t_vector					reflect_vec;
	bool						inside;
};

typedef struct s_intersections	t_intersections;
struct							s_intersections
{
	t_intersection				arr[200];
	int							count;
};

/* --- AMBIENT --- */
typedef struct s_ambient
{
	double						intensity;
	t_color						color;
	bool						present;
}								t_ambient;

/*-- MATRICES -- */
typedef double					t_mat4[4][4];
typedef double					t_mat3[3][3];
typedef double					t_mat2[2][2];

/* --- CAMERA --- */
typedef struct s_camera
{
	t_vector					pos;
	t_vector					dir;
	int							fov_deg;
	double						fov;
	bool						present;
	double						pixel_size;
	double						half_width;
	double						half_height;
	t_mat4						transform;
	t_mat4						inv_trans;
	bool						initialized;
	double						theta;
	double						phi;
}								t_camera;

/* --- LIGHT --- */
typedef enum e_light_type		t_light_type;
enum							e_light_type
{
	POINT,
	// SPOT
};

/* --LIGHT SOURCE-- */
typedef struct s_light
{
	t_light_type				type;
	t_vector					position;
	t_vector					direction;
	t_vector					init_direction;
	t_mat4						added_rots;
	double						intensity;
	t_color						color;
	bool						present;
}								t_light;

typedef enum e_shape_type
{
	SPHERE,
	PLANE,
	CYLINDER
}								t_shape_type;

/* OBJECTS PROPERTY */
typedef struct s_shape_props
{
	double						radius;
	double						radius_squared;
	double						height;
	t_color						color;
	double						diffuse;
	double						specular;
	double						shininess;
	double						reflectiveness;
	double						distance_from_origin;
	t_vector					scale;
	bool						highlighted;
}								t_shape_props;

typedef struct s_shape
{
	t_shape_type				type;
	int							id;
	t_shape_props				props;
	t_vector					origin;
	t_vector					orientation;
	t_mat4						transf;
	t_mat4						inv_transf;
	t_mat4						norm_transform;
	t_mat4						norm_transf;
	t_mat4						added_rots;

}								t_shape;

/* --- SPHERE --- */
typedef struct s_sphere
{
	t_vector					center;
	double						radius;
	t_color						color;
}								t_sphere;

/* --- PLANE --- */
typedef struct s_plane
{
	t_vector					point;
	t_vector					normal;
	t_color						color;
}								t_plane;

/* --- CYLINDER --- */
typedef struct s_cylinder
{
	t_vector					base;
	t_vector					axis;
	double						radius;
	double						height;
	t_color						color;
}								t_cylinder;

/* -- GENERIC CONTAINER -- */

typedef union u_figures
{
	t_sphere					sphere;
	t_plane						plane;
	t_cylinder					cylinder;
}								t_figures;

typedef struct s_objnode
{
	t_shape_type				type;
	t_figures					fig;
	t_shape_props				props;
	t_vector					origin;
	struct s_objnode			*next;
}								t_objnode;

typedef struct s_settings
{
	int							disp_w;
	int							disp_h;
	int							render_w;
	int							render_h;
	int							edit_w;
	int							edit_h;
	bool						edit_mode;
	int							reflection_depth;
}								t_settings;

/* WINDOWS MLX */
typedef struct s_display		t_display;
struct							s_display
{
	void						*mlx;
	void						*win;
	void						*display_img;
	char						*disp_addr;
	int							bpp;
	int							line_length;
	int							endian;
};

typedef struct s_look_at
{
	t_vector					final_pos;
	t_vector					final_dir;
	t_vector					current_dir;
	t_vector					initial_orientation;
	t_vector					pos_diff;
	t_vector					dir_diff;
	double						step_amount;
	int							step_num;
	bool						trigger;
}								t_look_at;

typedef struct s_keys			t_keys;
struct							s_keys
{
	bool						w;
	bool						a;
	bool						s;
	bool						d;
	bool						up;
	bool						down;
	bool						left;
	bool						right;
	bool						plus;
	bool						minus;
	bool						tab;
	bool						r;
	bool						o;
};

/* SCENE */
typedef struct s_scene
{
	t_ambient					ambient;
	t_camera					cam;
	int							n_ambient;
	int							n_camera;
	int							n_lights;
	int							n_shapes;
	int							n_spheres;
	int							n_planes;
	int							n_cylinders;
	size_t						object_count;
	t_objnode					*obj_head;
	t_objnode					*obj_end;
	t_light						*lights;
	t_shape						shapes[MAX_SHAPES];
	t_settings					settings;
	t_display					*disp;
	t_look_at					look_at;
	int							shape_idx;
	t_keys						keys_held;
}								t_scene;

/* THREAD AND RENDERING */

typedef struct s_worker			t_worker;
struct							s_worker
{
	int							worker_id;
	int							y_start;
	int							y_end;
	int							y_scale_start;
	int							y_scale_end;
	int							max_workers;
	int							height;
	int							width;
	char						*addr;
	t_scene						*scene;
};

#endif