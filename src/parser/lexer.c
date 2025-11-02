/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:31:44 by francesca         #+#    #+#             */
/*   Updated: 2025/10/31 12:23:59 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
Parses the first token (identifier) from `line`, skipping leading spaces,
copies it into `id` (up to 9 chars + '\0'), validates it, then positions
`*rest_out` at the start of the remainder of the line (after the id and
following spaces). Does NOT modify the original `line` buffer.
Returns: 0 on success; 1 on error (and prints an error message).
*/
static int	check_first_token(char *line, char **rest_out, char *id)
{
	int	i;

	i = 0;
	if (rest_out)
		*rest_out = NULL;
	while (*line && ft_is_space_char(*line))
		line++;
	while (*line && !ft_is_space_char(*line) && i < 9)
		id[i++] = *line++;
	id[i] = '\0';
	if (!is_valid_identifier(id))
		return (print_err_msg("identificator doesn't valid"));
	while (*line && ft_is_space_char(*line))
		line++;
	if (*line)
		*rest_out = line;
	return (0);
}

/*
Dispatches parsing for uppercase identifiers:
 * "A": ambient
 * "C": camera
 * "L": light
Error messages are handled inside the specific parse_* functions.
Returns: 0 on success; 1 on parse error or unknown identifier.
 */
static inline int	handle_capital(t_scene *scene, char *line, const char *id)
{
	if (ft_strcmp(id, "A") == 0)
		return (parse_ambient_line(scene, line) != 0);
	else if (ft_strcmp(id, "C") == 0)
		return (parse_camera_line(scene, line) != 0);
	else if (ft_strcmp(id, "L") == 0)
		return (parse_light_line(scene, line) != 0);
	else
		return (print_err_msg("identificator doesn't valid"));
}

/*
 Dispatches parsing for object identifiers:
 * "sp": sphere
 * "pl": plane
 * "cy": cylinder
Returns: 0 on success; 1 on parse error or unknown identifier.
 */
static inline int	handle_obj(t_scene *scene, char *line, const char *id)
{
	if (ft_strcmp(id, "sp") == 0)
		return (parse_sphere(scene, line) != 0);
	else if (ft_strcmp(id, "pl") == 0)
		return (parse_plane(scene, line) != 0);
	else if (ft_strcmp(id, "cy") == 0)
		return (parse_cylinder(scene, line) != 0);
	else
		return (print_err_msg("identificator doesn't valid"));
}

/*
Scans a single .rt line:
- extracts the identifier
- validates it
- dispatches to the appropriate parser
- enforces subject rules for uppercase tokens ("A", "C", "L"):
those elements must be declared at most once (the specific
parse_* functions should track presence/counters and report duplicates)
Returns: 0 on success; 1 on error.
 */
int	lex_scan_check_and_count(t_scene *scene, char *line)
{
	char	id[10];
	char	*rest_out;

	if (!line || !scene)
		return (1);
	rest_out = NULL;
	if (check_first_token(line, &rest_out, id))
		return (1);
	if (id[0] == 'A' || id[0] == 'C' || id[0] == 'L')
		return (handle_capital(scene, rest_out, id));
	else if (id[0] == 's' || id[0] == 'p' || id[0] == 'c')
		return (handle_obj(scene, rest_out, id));
	else
		return (1);
}
