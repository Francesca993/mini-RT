/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:27:58 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/27 17:32:51 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
funzione da usare per render "completo" e preciso. Disegna pixel per pixel.
*/
void	*render_scene(t_worker *worker)
{
	t_intersections	arr;                                  // array per salvare tutte le intersezioni trovate
	int				x;
	int				y;

	y = worker->y_start;                                  // inizia dalla prima riga assegnata a questo thread
	while (y < worker->y_end)                             // finché non ha completato la sua zona
	{
		x = 0;
		while (x < worker->width)                        // percorri tutta la larghezza dell'immagine
		{
			set_color(worker, x, y, 0);                  // resetta colore pixel 
			render_pixel(x, y, &arr, worker);            // calcola e disegna pixel
			x += 1;
		}
		y += 1;
	}
	return (NULL);
}

/*
Lancia raggio e calcola colore di un pixel.// esegue ray tracing + scrive su immagine
FUNZIONE CENTRALE DEL RAY TRACING 
Generazione del raggio
Intersezione con oggetti
Calcolo del colore
Scrittura del colore
*/

t_color	render_pixel(double x, double y, t_intersections *arr, t_worker *worker)
{
	int		shape_idx;
	t_scene	*scene;
	t_ray	ray;
	t_color	color;

	set_color(worker, x, y, 0);                       // reset del colore (non essenziale, ma ok)
	scene = worker->scene;                            // prendi la scena da cui ricavare camera, oggetti, ecc.
	ray_from_cam(&ray, &scene->cam, x + 0.5, y + 0.5); // genera un raggio che passa per il centro del pixel
	shape_idx = -1;
	arr->count = 0;                                   // resetta intersezioni
	while (++shape_idx < scene->n_shapes)         // controlla intersezione con ogni oggetto
		intersect(&scene->shapes[shape_idx], &ray, arr);
	color = shade_point(arr, scene, &ray);            // calcola il colore finale da assegnare al pixel
	set_color(worker, x, y, create_mlx_color(&color)); // scrive il colore nel buffer immagine
	return (color);
}


static void debug_print_vector(const char *label, t_vector v)
{
	if (label)
		printf("%s: (x=%.6f, y=%.6f, z=%.6f, w=%.6f)\n", label, v.x, v.y, v.z, v.w);
}

void populate_shapes_array(t_scene *scene)
{
    t_objnode *node = scene->obj_head;
    int i = 0;

    printf("Iterazione sugli oggetti nella scena:\n");
    while (node && i < MAX_SHAPES)
    {
        printf("Copiando oggetto %d, type=%d\n", i, node->type);

        if (node->type == SPHERE)
        {
            scene->shapes[i].type = node->type;
			scene->shapes[i].origin = node->fig.sphere.center;
			printf("SHAPES: ");
			debug_print_vector("center", scene->shapes[i].origin);
			default_shape_props(&scene->shapes[i].props); //aggiunto
            
           scene->shapes[i].props.radius = node->fig.sphere.radius;
		   printf("radius SHAPES: %.6f\n", scene->shapes[i].props.radius);
			scene->shapes[i].props.radius_squared = node->fig.sphere.radius * node->fig.sphere.radius;
		    scene->shapes[i].props.color = node->fig.sphere.color;
        }
		/*else if (node->type == PLANE)
        {
            scene->shapes[i].type = node->type;
			scene->shapes[i].origin = node->fig.plane.point;
			printf("SHAPES: ");
			debug_print_vector("PLANE POINT", scene->shapes[i].origin);
			default_shape_props(&scene->shapes[i].props); //aggiunto
            scene->shapes[i].orientation = node->fig.plane.normal;
			debug_print_vector("PLANE POINT", scene->shapes[i].orientation);
		    scene->shapes[i].props.color = node->fig.plane.color;
        }*/
	   else if (node->type == PLANE)
		{
			scene->shapes[i].type = node->type;
			scene->shapes[i].origin = node->fig.plane.point;
			printf("SHAPES: ");
			debug_print_vector("PLANE POINT", scene->shapes[i].origin);
			default_shape_props(&scene->shapes[i].props); //aggiunto
			scene->shapes[i].orientation = node->fig.plane.normal;
			scene->shapes[i].props.color = node->fig.plane.color;
		}

        else
        {
            printf("  [Tipo sconosciuto]\n");
        }

        node = node->next;
        i++;
    }
	scene->n_shapes = i;
    printf("Totale shapes copiati: %d\n", scene->n_shapes);
}



/*
Funzione chiamata per disegnare l’intera scena
quella chiamata da MiniRT per iniziare il rendering
*/
void	draw_scene(t_scene *scene)
{
	t_worker		workers[NUM_THREADS];

	init_workers(workers, scene);                             // inizializza i thread
	//run_workers(workers, scene, false, render_scene);
	run_workers(workers, render_scene);
	mlx_put_image_to_window(scene->disp->mlx, scene->disp->win,
		scene->disp->display_img, 0, 0);                      // mostra immagine su schermo COMMENTATO PER MAC
		printf("Immagine aggiornata nella finestra.\n");
}


/*void	*render_scene(t_worker *worker)
{
	t_intersections	arr;                                  // array per salvare tutte le intersezioni trovate
	int				x;
	int				y;

	y = worker->y_start;                                  // inizia dalla prima riga assegnata a questo thread
	while (y < worker->y_end)                             // finché non ha completato la sua zona
	{
		x = 0;
		while (x < worker->width)                        // percorri tutta la larghezza dell'immagine
		{
			set_color(worker, x, y, 0);                  // resetta colore pixel (opzionale)
			render_pixel(x, y, &arr, worker);            // calcola e disegna pixel
			printf("Thread %d: Rendering pixel (%d, %d)\n", worker->worker_id, x, y); // Debug
			x += 1;
		}
		y += 1;
	}
	return (NULL);
}*/

/*t_color	render_pixel(double x, double y, t_intersections *arr, t_worker *worker)
{
	int		shape_idx;
	t_scene	*scene;
	t_ray	ray;
	t_color	color;

	set_color(worker, x, y, 0);                       // reset del colore (non essenziale, ma ok)
	scene = worker->scene;                            // prendi la scena da cui ricavare camera, oggetti, ecc.
	ray_from_cam(&ray, &scene->cam, x + 0.5, y + 0.5); // genera un raggio che passa per il centro del pixel
	shape_idx = -1;
	arr->count = 0;                                   // resetta intersezioni
	while (++shape_idx < scene->n_shapes)         // controlla intersezione con ogni oggetto
		intersect(&scene->shapes[shape_idx], &ray, arr);
	color = shade_point(arr, scene, &ray);            // calcola il colore finale da assegnare al pixel
	set_color(worker, x, y, create_mlx_color(&color)); // scrive il colore nel buffer immagine
	
	// Debug per la direzione del raggio e il colore calcolato
	printf("Ray origin: (%f, %f, %f), Direction: (%f, %f, %f)\n", 
		ray.origin.x, ray.origin.y, ray.origin.z, 
		ray.dir.x, ray.dir.y, ray.dir.z);
	
	printf("Pixel (%f, %f) - Colore calcolato: R: %f, G: %f, B: %f\n", 
		x, y, color.r, color.g, color.b);
	
	return (color);
}*/
//FUNZIONA QUI SOTTO
/*void draw_scene(t_scene *scene)
{
    // Inizia il rendering (esempio di debug)
    printf("Rendering la scena...\n");

    // Controlla che la scena e la fotocamera siano correttamente inizializzate
    if (!scene || !scene->cam.present) {
        printf("Errore: scena o fotocamera non inizializzate correttamente.\n");
        return;
    }

    // Logga il processo di rendering dei pixel (ad esempio)
    for (int y = 0; y < scene->settings.render_h; ++y)
    {
        for (int x = 0; x < scene->settings.render_w; ++x)
        {
            // Calcola il colore per il pixel (esempio)
            unsigned int color = (x * 255 / scene->settings.render_w) << 16 | (y * 255 / scene->settings.render_h) << 8;
            
            // Scrivi il colore nel buffer dell'immagine
            scene->disp->disp_addr[(y * scene->settings.render_w + x) * 4] = (color & 0xFF);         // Blu
            scene->disp->disp_addr[(y * scene->settings.render_w + x) * 4 + 1] = ((color >> 8) & 0xFF); // Verde
            scene->disp->disp_addr[(y * scene->settings.render_w + x) * 4 + 2] = ((color >> 16) & 0xFF); // Rosso
            scene->disp->disp_addr[(y * scene->settings.render_w + x) * 4 + 3] = 0;                  // Alpha (trasparente)
        }
    }

    printf("Rendering completato.\n");

    // Aggiorna la finestra con l'immagine
    mlx_put_image_to_window(scene->disp->mlx, scene->disp->win, scene->disp->display_img, 0, 0);
}*/

