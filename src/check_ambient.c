/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 23:12:05 by francesca         #+#    #+#             */
/*   Updated: 2025/10/07 00:03:33 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT"

/* Parsing double base-10 con '.' (niente expo). Ritorna 1 ok, 0 errore. */
/*
Cosa fa in breve
Salta gli spazi iniziali.
Legge un segno opzionale + o -.
Legge la parte intera (sequenza di cifre).
Se c’è un punto . legge la parte frazionaria (altre cifre).
Se non ha letto nessuna cifra (né prima né dopo il .) → fallisce.
Calcola valore = segno * (intero + frazionale) dove
frazionale = frac / scale (es. per “.37” → frac=37, scale=100, quindi 0.37).
Aggiorna il puntatore d’ingresso per “consumare” i caratteri letti.
Ritorna 1 se ok, 0 se errore.
*/
int	parse_double(const char **line, double *out)
{
	const char	*n_line;
	int			sign;
	double		val; //Accumula la parte intera (prima del .). Se leggi “123”, alla fine val = 123.
	double		frac; //Accumulano la parte frazionaria come intero e la scala corrispondente.
	double		scale; // La scala o.100 / 0.10 / 0.1000
	int			ndigits; // Conta quante cifre sono state lette prima il punto.
	int			nfrac; // Conta quante cifre sono state lette dopo il punto.
	double		fractional_part; // variabile temporanea per calcolare frac/scale

    n_line = skip_spaces(*line); // Salta gli spazi iniziali
	sign = 1;
	val = 0.0;
	frac = 0.0;
	scale = 1.0;
	ndigits = 0;
	nfrac = 0;
    /* Se c’è +/-, aggiorna sign e avanza. */
	if (*n_line == '+' || *n_line == '-')
	{
		if (*n_line == '-')
			sign = -1;
		n_line++;
	}
    /* Parte intera */
	while (is_digit(*n_line))
	{
		val = val * 10.0 + (double)(*n_line - '0');
		n_line++;
		ndigits++;
	}
    /* Parte dopo il . */
	if (*n_line == '.')
	{
		n_line++;
		while (is_digit(*n_line))
		{
			frac = frac * 10.0 + (double)(*n_line - '0');
			scale *= 10.0;
			n_line++;
			nfrac++;
		}
	}
	/* Se non è stato letto nessun carattere numerico né nella parte intera né in quella frazionaria,
		la funzione fallisce e restituisce 0. */
	if (ndigits == 0 && nfrac == 0)
	{
		return (0);
	}
	/* Calcolo esplicito della parte frazionaria normalizzata. Se nfrac è zero uso 0.0,
		altrimenti uso frac / scale. */
	if (nfrac == 0)
	{
		fractional_part = 0.0;
	}
	else
	{
		fractional_part = frac / scale;
	}
	/* Assegno al puntatore di output il valore con segno */
	*out = sign * (val + fractional_part);
	/* Aggiorno il puntatore della stringa chiamante per avanzare alla nuova posizione */
	*line = n_line;
	/* Successo */
	return (1);
}

/* --- Ambiente --- */
/*
// Ambient lighting (A ratio [0..1], RGB [0..255] -> normalizzati)
typedef struct s_ambient
{
	double ratio;  // [0..1]
	t_color color; // normalizzato 0..1
	bool present;  // per validare che A compaia una sola volta
}	t_ambient;

*/
int	check_ambient(t_scene *scene, char *line)
{
	const char	*p;
	double		ratio;
	int			R;
	int			G;
	int			B;

	if (!scene || !line)
		return (fprintf(stderr, "Parametro ambient mancante\n"), 1);
	if (scene->n_ambient > 1)
	{
		fprintf(stderr, "Error\nAmbiente 'A' definito più di una volta\n");
		return (1);
	}
	scene->amb.present = true;
	/* ratio */
	if (!parse_double(&line, &ratio))
		return (fprintf(stderr, "Formato ratio di 'A' non valido"), 1);
	if (ratio < 0.0 || ratio > 1.0)
		return (fprintf(stderr, "Ratio di 'A' fuori range [0..1]"), 1);
	if (*line == '\0' || !ft_is_space_char(*line))
		return (fprintf(stderr, "Color di 'A' mancante (R,G,B)"), 1);
	line = skip_spaces(line);
	if (*line == '\0')
		return (fprintf(stderr, "Color di 'A' mancante (R,G,B)"), 1);
	/* R,G,B in [0..255] con virgole */
	if (!parse_rgb_255(&line, &R, &G, &B))
		return (fprintf(stderr,
				"Formato colore di 'A' non valido (atteso R,G,B)"), 1);
	/* Controllo che non ci sia niente neanche dopo*/
	line = skip_spaces(line);
	if (*line != '\0')
		return (fprintf(stderr, "Token extra dopo colore in 'A'"), 1);
	/* Carica nella scena (normalizzazione 0..1) */
	scene->amb.ratio = ratio;
	scene->amb.color.r = (double)R / 255.0;
	scene->amb.color.g = (double)G / 255.0;
	scene->amb.color.b = (double)B / 255.0;
	scene->amb.present = true;
	return (0);
}
