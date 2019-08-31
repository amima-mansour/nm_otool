/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:10:25 by amansour          #+#    #+#             */
/*   Updated: 2019/08/20 11:22:12 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	display_name(char *filename, char *name)
{
	//if (!name)
	//	ft_printf("\n%s:\n", filename);
	//else
    if (name)
		ft_printf("\n%s(%s):\n", name, filename);
}

bool	nm(void *ptr, uint64_t size, char *filename, char *archive)
{
	uint32_t magic_nb;
	t_file	file;
	bool	err;

	display_name(filename, archive);
	file.ptr = ptr;
	file.size = size;
	file.filename = filename;
	magic_nb = *(uint32_t*)((uintptr_t)(ptr));
	file.swap_bits = false;
	if (magic_nb == MH_CIGAM_64 || magic_nb == MH_CIGAM || magic_nb == FAT_CIGAM_64
		|| magic_nb == FAT_CIGAM)
		file.swap_bits = true; 
	if (magic_nb == MH_MAGIC_64 || magic_nb == MH_CIGAM_64
		|| magic_nb == MH_MAGIC || magic_nb == MH_CIGAM)
	{
		if (magic_nb == MH_MAGIC_64 || magic_nb == MH_CIGAM_64)
			file.arch = ARCH_64;
		else
			file.arch = ARCH_32;
		err = handle_mach_o(&file);
	}
	else if (magic_nb == FAT_MAGIC_64 || magic_nb == FAT_CIGAM_64)
	{
		file.arch = ARCH_64;
		err = handle_fat_64(&file);
	}
	else if (magic_nb == FAT_MAGIC || magic_nb == FAT_CIGAM)
	{
		file.arch = ARCH_32;
		err = handle_fat_32(&file);
	}
	else if (ft_strncmp((char *)ptr, ARMAG, SARMAG) == 0)
		err = handle_archive(&file);
	else
		return (errors(filename, NOT_RECOGNIZED));
	return (err);
}

static bool	treat_file(int fd, char *name)
{
	struct stat	buf;
	void		*ptr;
	bool		value;

	if ((fstat(fd, &buf)) < 0)
		return (errors(name, FSTAT_ERROR));
	if (S_ISDIR(buf.st_mode))
		return (errors(name, DIR));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(name, MAP_ERROR));
	value = nm(ptr, buf.st_size, name, NULL); 
	if (munmap(ptr, buf.st_size) < 0)
		return (errors(name, MUN_ERROR));
	if (close(fd) == -1)
		return (errors(name, CLOSE_ERROR));
	return (value);
}

int			main(int ac, char **av)
{
	int		fd;

	if (ac < 2)
		return (nm_usage());
	while (ac > 1)
	{
		if ((fd = open(av[ac - 1], O_RDONLY)) < 0)
		{
			errors(av[ac - 1], OPEN_ERROR);
			return (EXIT_FAILURE);
		}
		if (treat_file(fd, av[ac - 1]))
			return (EXIT_FAILURE);
		ac -= 1;
	}
	return (EXIT_SUCCESS);
}
