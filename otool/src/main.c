/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:10:25 by amansour          #+#    #+#             */
/*   Updated: 2019/09/05 09:33:57 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		ft_putstr_name(t_file f, char *name, uint32_t magic)
{
	if (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64)
		return ;
	if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		return ;
	if (ft_strncmp((char *)f.ptr, ARMAG, SARMAG) == 0)
		ft_printf("Archive : %s\n", f.filename);
	else if (!name)
		ft_printf("%s:\nContents of (__TEXT,__text) section\n", f.filename);
	else if (ft_strcmp(name, FAT) == 0)
		return ;
	else
	{
		ft_printf("%s(%s):\n", name, f.filename);
		ft_printf("Contents of (__TEXT,__text) section\n");
	}
}

static t_file	init_file(char *filename, uint64_t size, void *ptr)
{
	t_file file;

	file.ptr = ptr;
	file.size = size;
	file.filename = filename;
	return (file);
}

bool			otool(void *ptr, uint64_t size, char *filename, char *name)
{
	uint32_t	magic_nb;
	t_file		file;

	magic_nb = *(uint32_t*)((uintptr_t)(ptr));
	file = init_file(filename, size, ptr);
	ft_putstr_name(file, name, magic_nb);
	file.swap_bits = is_swap(magic_nb);
	if (magic_nb == MH_MAGIC_64 || magic_nb == MH_CIGAM_64)
	{
		file.arch = ARCH_64;
		return (handle_mach_o(&file));
	}
	if (magic_nb == MH_MAGIC || magic_nb == MH_CIGAM)
	{
		file.arch = ARCH_32;
		return (handle_mach_o(&file));
	}
	if (magic_nb == FAT_MAGIC_64 || magic_nb == FAT_CIGAM_64)
		return (handle_fat_64(&file));
	if (magic_nb == FAT_MAGIC || magic_nb == FAT_CIGAM)
		return (handle_fat_32(&file));
	if (ft_strncmp((char *)ptr, ARMAG, SARMAG) == 0)
		return (handle_archive(&file));
	return (errors(filename, NOT_VALID));
}

static bool		treat_file(int fd, char *name)
{
	struct stat	buf;
	void		*ptr;
	bool		value;

	if ((fstat(fd, &buf)) < 0)
		return (errors(name, FSTAT_ERROR));
	if (S_ISDIR(buf.st_mode))
		return (errors(name, DIR));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, \
					MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(name, NOT_VALID));
	value = otool(ptr, buf.st_size, name, NULL);
	if (munmap(ptr, buf.st_size) < 0)
		return (errors(name, MUN_ERROR));
	if (close(fd) == -1)
		return (errors(name, CLOSE_ERROR));
	return (value);
}

int				main(int ac, char **av)
{
	int		fd;
	int		i;

	i = 0;
	if (ac < 2)
	{
		write(2, "Usage: ./ft_otool binary_file [...] \n", 38);
		return (EXIT_FAILURE);
	}
	while (++i < ac)
	{
		if ((fd = open(av[i], O_RDONLY)) < 0)
		{
			errors(av[i], OPEN_ERROR);
			return (EXIT_FAILURE);
		}
		if (treat_file(fd, av[i]))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
