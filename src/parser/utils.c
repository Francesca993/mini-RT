/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:25:56 by francesca         #+#    #+#             */
/*   Updated: 2025/10/31 11:35:33 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Internal helper: consumes a comma with optional spaces around it and returns
0 if it does not find the comma
*/
int	skip_comma(const char **input_ptr)
{
	const char	*cursor;

	if (input_ptr == NULL || *input_ptr == NULL)
		return (0);
	cursor = *input_ptr;
	if (*cursor != ',')
		return (0);
	cursor++;
	*input_ptr = cursor;
	return (1);
}

/*
Print error message
*/
int	print_err_msg(const char *msg)
{
	fprintf(stderr, "Error\n");
	fprintf(stderr, "%s\n", msg);
	return (1);
}

/*
Print specifc error message
*/
int	err_msg(char *type, int num, const char *msg)
{
	fprintf(stderr, "%s numero: %d, %s\n", type, num, msg);
	return (1);
}
