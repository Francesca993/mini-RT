
static int gnl_lexer(t_scene *scene, int fd)
{
    char	*line;

    line = get_next_line(fd);
	while (line != NULL)
	{
		if (ft_is_blank(line))
		{
			free(line);
			line = get_next_line(fd);
			continue;
		}
		chop_newline(line);
		if (lex_scan_check_and_count(scene, line) != 0)
		{
			free(line);
			// close(fd);
			return (1);
		}
		free(line);
		line = get_next_line(fd);
	}
    return(0);
}


int	parse_file(const char *path, t_scene *scene)
{

	int		fd;

	if (!path || !scene)
		return (print_err_msg("parse_file: invalid arguments."));
	if (!scene_reset(scene))
		return (print_err_msg("scene allocation."));
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (print_err_msg("miniRT: open"));
    if (gnl_lexer(scene, fd) != 0)
    {
        close(fd);
        return (1);
    }
	if (close(fd) == -1)
		return (print_err_msg("Error: miniRT: close"));
	if (check_presence(scene) == 1)
		return (1);
	return (0);
}

