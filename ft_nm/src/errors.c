#include "ft_nm.h"

void				*iscorrup(t_file *file, void *ptr, uint64_t size)
{
	if ((uint64_t)(ptr + size - file->ptr) > file->size)
		return (NULL);
	return (ptr);
}


bool			errors(char *filename, char *msg)
{
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	return (true);
}

int				nm_usage(void)
{
    ft_printf("Usage: %s [file ...]\n", "ft_nm");
	return(EXIT_FAILURE);
}
