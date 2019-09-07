/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:10:25 by amansour          #+#    #+#             */
/*   Updated: 2019/09/07 11:30:05 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static t_file	init_file(char *filename, char *name, uint64_t size, void *ptr)
{
	t_file file;

	file.ptr = ptr;
	file.size = size;
	file.sects = NULL;
	file.syms = NULL;
	file.filename = filename;
	if (name)
		ft_printf("\n%s(%s):\n", name, filename);
	return (file);
}

bool			nm(void *ptr, uint64_t size, char *filename, char *archive)
{
	uint32_t	magic_nb;
	t_file		file;

	magic_nb = *(uint32_t*)((uintptr_t)(ptr));
	file = init_file(filename, archive, size, ptr);
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

static bool		treat_file(char *name)
{
	struct stat	buf;
	void		*ptr;
	bool		value;
	int			fd;

	if ((fd = open(name, O_RDONLY)) < 0)
		return (errors(name, OPEN_ERROR));
	if ((fstat(fd, &buf)) < 0)
		return (errors(name, FSTAT_ERROR));
	if (S_ISDIR(buf.st_mode))
		return (errors(name, DIR));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, \
					MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(name, NOT_VALID));
	value = nm(ptr, buf.st_size, name, NULL);
	if (munmap(ptr, buf.st_size) < 0)
		return (errors(name, MUN_ERROR));
	if (close(fd) == -1)
		return (errors(name, CLOSE_ERROR));
	return (value);
}

int				main(int ac, char **av)
{
	bool	err;

	if (ac < 2)
		err = treat_file(DEFAULT_FILE);
	if (ac > 2 && get_flags(&ac, &av))
		return (usage());
	g_multi_file = ac > 2;
	while (--ac)
	{
		av++;
		err = treat_file(*av);
	}
	if (!err)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
