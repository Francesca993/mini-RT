/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_double.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 14:07:53 by francesca         #+#    #+#             */
/*   Updated: 2025/11/02 14:08:19 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* --- Firma della funzione da testare --- */
int parse_double(const char **line, double *out);

typedef struct s_numparse
{
    const char *cursor;
    int        sign;
    double     val;
    double     frac;
    double     scale;
    int        ndigits;
    int        nfrac;
} t_numparse;

static inline void init_parameters(t_numparse *n)
{
    n->sign = 1;
    n->val = 0.0;
    n->frac = 0.0;
    n->scale = 1.0;
    n->ndigits = 0;
    n->nfrac = 0;
}

static inline void parse_numdoub(t_numparse *n)
{
    if (*n->cursor == '+' || *n->cursor == '-') 
    {
        if (*n->cursor == '-') 
            n->sign = -1;
        n->cursor++;
    }
    while (ft_isdigit((unsigned char)*n->cursor)) 
    {
        n->val = n->val * 10.0 + (double)(*n->cursor - '0');
        n->cursor++;
        n->ndigits++;
    }
    if (*n->cursor == '.') 
    {
        n->cursor++;
        while (ft_isdigit((unsigned char)*n->cursor)) 
        {
            n->frac  = n->frac * 10.0 + (double)(*n->cursor - '0');
            n->scale = n->scale * 10.0;   // <— NON dimenticare questo
            n->cursor++;
            n->nfrac++;
        }
    }
}


int parse_double(const char **line, double *out)
{
    t_numparse n;
    double fractional = 0.0;          // inizializza

    n.cursor = skip_spaces(*line);
    init_parameters(&n);
    parse_numdoub(&n);

    if (n.ndigits == 0 && n.nfrac == 0)
        return 0;                     // errore: nessuna cifra

    /* Calcolo della frazione: mantieni l'else QUI */
    if (n.nfrac > 0)
        fractional = n.frac / n.scale;
    else
        fractional = 0.0;             // <— QUI: questo è l'else che dicevo

    *out  = n.sign * (n.val + fractional);
    *line = n.cursor;
    return 1;                         // successo
}
