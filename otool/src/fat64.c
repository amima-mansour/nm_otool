/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 11:05:51 by amansour          #+#    #+#             */
/*   Updated: 2019/09/03 15:13:19 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static bool	same_arch(t_file *f, struct fat_arch_64 *a, uint32_t nfat, bool *e)
{
	uint32_t	i;
	uint64_t	offset;
	uint64_t	size;

	i = -1;
	while (++i < nfat)
	{
		if (!a || swap32(f->swap_bits, a->align) > 15)
		{
			*e = errors(f->filename, CORRUPT_FILE);
			return (true);
		}
		offset = swap64(f->swap_bits, a->offset);
		size = swap64(f->swap_bits, a->size);
		if (swap32(f->swap_bits, a->cputype) == CPU_TYPE_X86_64)
		{
			if (!iscorrup(f, f->ptr + offset, size))
				*e = errors(f->filename, CORRUPT_FILE);
			else
				*e = otool(f->ptr + offset, size, f->filename, NULL);
			return (true);
		}
		a = (struct fat_arch_64 *)iscorrup(f, a + 1, sizeof(*a));
	}
	return (false);
}

static bool	all_arch(t_file *f, struct fat_arch_64 *a, uint32_t nfat)
{
	uint32_t		i;
	uint64_t		offset;
	uint64_t		size;
	char			*arch_name;

	i = -1;
	while (++i < nfat)
	{
		if (!a)
			return (errors(f->filename, CORRUPT_FILE));
		offset = swap64(f->swap_bits, a->offset);
		size = swap64(f->swap_bits, a->size);
		if (!iscorrup(f, f->ptr + offset, size))
			return (errors(f->filename, CORRUPT_FILE));
		arch_name = get_arch_name(swap32(f->swap_bits, a->cputype),\
				swap32(f->swap_bits, a->cpusubtype));
		ft_printf("%s (architecture %s):\n", f->filename, arch_name);
		ft_printf("Contents of (__TEXT,__text) section\n");
		if (otool(f->ptr + offset, size, f->filename, FAT))
			return (true);
		a = (struct fat_arch_64 *)iscorrup(f, a + 1, sizeof(*a));
	}
	return (false);
}

bool		handle_fat_64(t_file *file)
{
	uint32_t				nfat;
	struct fat_header		*header;
	struct fat_arch_64		*arch;
	bool					err;

	header = (struct fat_header *)iscorrup(file, (void*)file->ptr, \
			sizeof(*header));
	if (!header)
		return (errors(file->filename, CORRUPT_FILE));
	nfat = swap32(file->swap_bits, header->nfat_arch);
	arch = (struct fat_arch_64 *)iscorrup(file, (void*)header + \
			sizeof(*header), sizeof(*arch));
	err = false;
	if (same_arch(file, arch, nfat, &err))
		return (err);
	err = all_arch(file, arch, nfat);
	return (err);
}
