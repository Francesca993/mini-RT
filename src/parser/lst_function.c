/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_function.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:59:53 by francesca         #+#    #+#             */
/*   Updated: 2025/10/30 20:15:29 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Resets only the list-related fields
*/
void	objlist_init(t_scene *scene)
{
	scene->obj_head = NULL;
	scene->obj_end = NULL;
	scene->n_ambient = 0;
	scene->n_camera = 0;
	scene->n_lights = 0;
	scene->n_shapes = 0;
	scene->n_spheres = 0;
	scene->n_planes = 0;
	scene->n_cylinders = 0;
	scene->object_count = 0;
}

/*
Frees the entire linked list of objects. After freeing, it clears the list
pointers and resets all related counters.
 */
void	objlist_destroy(t_scene *scene)
{
	t_objnode	*current;
	t_objnode	*next;

	if (!scene)
		return ;
	current = scene->obj_head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	scene->obj_head = NULL;
	scene->obj_end = NULL;
	scene->n_ambient = 0;
	scene->n_camera = 0;
	scene->n_lights = 0;
	scene->n_shapes = 0;
	scene->n_spheres = 0;
	scene->n_planes = 0;
	scene->n_cylinders = 0;
	scene->object_count = 0;
}

/* Add a new node */
t_objnode	*objnode_new(t_shape_type type, t_figures fig)
{
	t_objnode	*n;

	n = (t_objnode *)malloc(sizeof(t_objnode));
	if (!n)
		return (NULL);
	n->type = type;
	n->fig = fig;
	n->next = NULL;
	return (n);
}

/*
Appends a new object node to the scene's singly linked list in O(1),
preserving the order in which objects appear in the input file and
updating counters accordingly.
Parameters:
 * scene         : target scene
 * object_type   : shape type for the new node
 * object_payload: figure payload (union) for the new node
Returns:
 *  0 on success
 *  1 on error (prints an error message)
*/
int	object_list_append(t_scene *scene, t_shape_type object_type,
		t_figures object_payload)
{
	t_objnode	*new_node;

	if (scene == NULL)
		return (print_err_msg("scene NULL"));
	new_node = objnode_new(object_type, object_payload);
	if (new_node == NULL)
		return (print_err_msg("malloc failed node object"));
	if (scene->obj_head == NULL)
	{
		scene->obj_head = new_node;
		scene->obj_end = new_node;
	}
	else
	{
		scene->obj_end->next = new_node;
		scene->obj_end = new_node;
	}
	scene->object_count += 1;
	return (0);
}
