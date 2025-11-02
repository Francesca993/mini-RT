/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathematical.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:14:33 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/30 18:45:13 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHEMATICAL_H
# define MATHEMATICAL_H

# define RAD_TO_DEG 57.2957795131
# define DEG_TO_RAD 0.01745329252
# define EPSILON 1e-6

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

// FUNZIONI SUI VETTORI
// 1.vector_arithmetic.c
void					scale_vec(t_vector *res, const t_vector *v,
							double scale);
void					sub_vec(t_vector *res, const t_vector *v1,
							const t_vector *v2);
void					negate_vec(t_vector *res, const t_vector *v);
void					add_vec(t_vector *res, const t_vector *v1,
							const t_vector *v2);
void					reflect_vector(t_vector *res, t_vector *in_vector,
							t_vector *normal);

// 2.vector_operations.c
double					vec_distance(const t_vector *v1, const t_vector *v2);
double					vec_magnitude(const t_vector *vec);
double					dot_product(const t_vector *v1, const t_vector *v2);
void					normalize_vec(t_vector *vec);
void					cross_product(t_vector *res, const t_vector *v1,
							const t_vector *v2);

// MATRICI
typedef double			t_mat4[4][4];
typedef double			t_mat3[3][3];
typedef double			t_mat2[2][2];

// FUNZIONI SULLE MATRICI
// 3.transformation_matrices.c
void					translate_matrix(t_mat4 *mat, double x, double y,
							double z);
void					scaling_matrix(t_mat4 *mat, double x, double y,
							double z);
void					rotation_matrix_x(t_mat4 *mat, double r);
void					rotation_matrix_y(t_mat4 *mat, double r);
void					axis_angle(t_mat4 *rot_mat, const t_vector *ax,
							double angle);

// MOLTIPLICAZIONI
// 4.matrix_operations.c
void					identity_matrix(t_mat4 *mat);
void					transpose_matrix(t_mat4 *mat);
void					mat_vec_multiply(t_vector *res, const t_mat4 *mat,
							const t_vector *vec);
void					mat_multiply(t_mat4 *res, const t_mat4 *m1,
							const t_mat4 *m2);

// FUNZIONI AVANZATE MATRICI
// 5.matrix_inverse.c
// 6.matrix_inverse2.c
void					submat4(t_mat3 *res, const t_mat4 *mat, int row,
							int col);
double					det3(const t_mat3 *mat3);
void					mat_inverse(t_mat4 *res, const t_mat4 *mat);

// FUNZIONI MATH CAMERA
void					view_transform(t_mat4 *res, const t_vector *from,
							const t_vector *up, const t_vector *forward);
void					camera_init(t_camera *camera, t_scene *scene);
void					calculate_camera_transform(t_scene *scene);

// FUNZIONI MATH COLOR
void					add_colors(t_color *res, const t_color *c1,
							const t_color *c2);
void					mult_color(t_color *res, const t_color *color,
							double val);
void					sub_colors(t_color *res, const t_color *c1,
							const t_color *c2);
void					blend_colors(t_color *res, const t_color *c1,
							const t_color *c2);

t_color					get_shape_color(t_intersection *itx);

typedef struct s_scene	t_scene;
bool					get_specular_and_diffuse(t_scene *scene, int light_idx,
							t_intersection *itx, t_phong *phong);

// FUNZIONI MATH UTILS

double					max3(double n1, double n2, double n3);
double					min3(double n1, double n2, double n3);
double					min(double a, double b);
double					max(double a, double b);

// INTERSEZIONI
// intersection.c
bool					intersect(t_shape *shape, const t_ray *ray,
							t_intersections *xs);
bool					intersect_sphere_fast(const t_ray *ray,
							t_intersections *xs, t_shape *sphere);
bool					intersect_plane_fast(const t_ray *ray, t_shape *plane,
							t_intersections *xs);
bool					intersect_cylinder(const t_ray *ray, t_shape *shape,
							t_intersections *xs);

// intersection_utils.c
void					prepare_computations(t_intersection *intersection,
							t_ray *ray);
t_intersection			*hit(t_intersections *xs);
void					ray_position(t_vector *pos, const t_ray *ray,
							double time);
void					transform_ray(t_ray *transformed_ray, const t_ray *ray,
							const t_shape *shape);
void					ray_from_cam(t_ray *ray, const t_camera *cam, double x,
							double y);

// normal.c
t_vector				plane_normal(const t_shape *shape);
t_vector				sphere_normal(t_vector *normal, const t_shape *shape,
							const t_vector *itx_point);
t_vector				normal_at(const t_shape *shape,
							const t_vector *itx_point);

// look_at.c
void					look_at(t_scene *scene, t_shape *shape);

// FUNZIONI MATH LUCI
bool					is_shadowed(t_scene *scene, int light_idx,
							t_vector *itx_point);
void					calculate_specular(t_vector *reflect_v,
							t_intersection *itx, t_phong *phong,
							t_light *light);
t_color					phong(t_intersection *itx, t_scene *scene,
							int light_idx);
t_color					shade_point(t_intersections *arr, t_scene *scene,
							t_ray *ray);

// FUNZIONI RENDER
t_color					render_pixel(double x, double y, t_intersections *arr,
							t_worker *worker);
void					init_workers(t_worker *workers, t_scene *scene);
void					run_workers(t_worker *workers, void *func);
void					draw_scene(t_scene *scene);
void					default_shape_props(t_shape_props *props);
void					populate_shapes_array(t_scene *scene);

void					fill_sphere(t_shape *shape, t_sphere *sphere);
void					fill_plane(t_shape *shape, t_plane *plane);
void					fill_cylinder(t_shape *shape, t_cylinder *cylinder);

// FUNZIONI COLOR UTILS
unsigned int			create_mlx_color(t_color *color);
void					set_color(t_worker *worker, int x, int y, int color);

// FUNZIONI TRANSFORM
void					calculate_orientation(t_mat4 *rot_transform,
							t_shape *shape);
void					multiply_transforms(t_shape *shape, t_mat4 *scale,
							t_mat4 *rot, t_mat4 *translate);
void					calculate_shape_transforms(t_shape *shape);
void					calculate_transforms(t_scene *scene);

// FUNZIONI HOOK LOOP KEYS

void					rotate_x(t_scene *scene, t_mat4 *rot_mat, double deg);
void					rotate_y(t_scene *scene, t_mat4 *rot_mat, double deg);
void					rotate_z(t_scene *scene, t_mat4 *rot_mat, double deg);
int						key_press(int key, t_scene *scene);
int						key_release(int key, t_scene *scene);
int						render_loop(t_scene *scene);
void					toggle_edit_mode(int key, t_scene *scene);
void					select_shape(int key, t_scene *scene);
void					toggle_keys_held(int key, t_scene *scene, bool on_off);
bool					is_toggle_key(int key, t_scene *scene);
void					toggle_shape(t_scene *scene);
void					move_object_fwd(t_scene *scene, t_shape *shape);
void					move_object_h(t_scene *scene, t_shape *shape);
void					transform_object(t_scene *scene);
bool					is_loop_hook_key(t_scene *scene);
void					move_object_h(t_scene *scene, t_shape *shape);
void					move_object_v(t_scene *scene, t_shape *shape);
t_vector				*sphere_to_xyz(t_vector *vec, double phi, double theta,
							double r);
void					change_height(t_scene *scene, t_shape *shape);
void					scale_object(t_scene *scene, t_shape *shape);

#endif