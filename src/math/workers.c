/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   workers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:27:22 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/24 19:02:22 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <pthread.h>
# include "miniRT.h"

/*
Inizializza la struttura t_worker per ciascun thread, dividendogli il lavoro da fare
Questa funzione divide l’immagine tra i thread e prepara tutti i dati 
che ciascun thread userà per disegnare la sua parte.
*/
void	init_workers(t_worker *workers, t_scene *scene)
{
	unsigned int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		workers[i].height = scene->settings.render_h;
		workers[i].width = scene->settings.render_w;
		workers[i].addr = scene->disp->disp_addr;
		
		workers[i].max_workers = NUM_THREADS;
		workers[i].worker_id = i;
		workers[i].scene = scene;
		workers[i].y_start = (workers[i].height / (double)NUM_THREADS) * i;
		workers[i].y_end = (workers[i].height / (double)NUM_THREADS) * (i + 1);
		/*workers[i].y_scale_start = (scene->settings.disp_h \
			/ (double)NUM_THREADS) * i;
		workers[i].y_scale_end = (scene->settings.disp_h \
			/ (double)NUM_THREADS) * (i + 1);*/
		i++;
	}
}
/*
Fa partire i thread di rendering e attende che finiscano prima di proseguire
Così non procedo al mlx_put_image_to_window prima che tutto sia disegnato
*/

//void	run_workers(t_worker *workers, t_scene *scene, void *func)
void	run_workers(t_worker *workers, void *func)
{
	pthread_t	threads[NUM_THREADS];
	int			i;

	i = -1;
	while (++i < NUM_THREADS)
		pthread_create(&threads[i], NULL, func, &workers[i]);
	i = -1;
	while (++i < NUM_THREADS)
		pthread_join(threads[i], NULL);
}
