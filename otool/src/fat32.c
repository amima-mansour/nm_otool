/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 10:56:33 by amansour          #+#    #+#             */
/*   Updated: 2019/09/02 15:59:15 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static bool		same_arch(t_file *f, struct fat_arch *a, uint32_t nfat, bool *e)
{
	uint32_t	i;
	uint32_t	offset;
	uint32_t	size;

	i = -1;
	while (++i < nfat)
	{
		if (!a || swap32(f->swap_bits, a->align) > 15)
		{
			*e = errors(f->filename, CORRUPT_FILE);
			return (true);
		}
		offset = swap32(f->swap_bits, a->offset);
		size = swap32(f->swap_bits, a->size);
		if (swap32(f->swap_bits, a->cputype) == CPU_TYPE_X86_64)
		{
			if (!iscorrup(f, f->ptr + offset, size))
				*e = errors(f->filename, CORRUPT_FILE);
			else
				*e = otool(f->ptr + offset, size, f->filename, NULL);
			return (true);
		}
		a = (struct fat_arch *)iscorrup(f, a + 1, sizeof(*a));
	}
	return (false);
}

static bool		all_arch(t_file *file, struct fat_arch *arch, uint32_t nfat)
{
	cpu_type_t		cputype;
	cpu_subtype_t	cpusubtype;
	uint32_t		offset;
	uint32_t		size;

	while (nfat--)
	{
		if (!arch)
			return (errors(file->filename, CORRUPT_FILE));
		offset = swap32(file->swap_bits, arch->offset);
		size = swap32(file->swap_bits, arch->size);
		if (!iscorrup(file, file->ptr + offset, size))
			return (errors(file->filename, CORRUPT_FILE));
		cputype = swap32(file->swap_bits, arch->cputype);
		cpusubtype = swap32(file->swap_bits, arch->cpusubtype);
		ft_printf("%s (architecture %s):\nContents of (__TEXT,__text) section\n", file->filename, \
				get_arch_name(cputype, cpusubtype));
		if (otool(file->ptr + offset, size, file->filename, NULL))
			return (true);
		arch = (struct fat_arch *)iscorrup(file, arch + 1, sizeof(*arch));
	}
	return (false);
}

bool			handle_fat_32(t_file *file)
{
	uint32_t			nfat;
	struct fat_header	*header;
	struct fat_arch		*arch;
	bool				err;

	header = (struct fat_header *)iscorrup(file, \
			(void*)file->ptr, sizeof(*header));
	if (!header)
		return (errors(file->filename, CORRUPT_FILE));
	nfat = swap32(file->swap_bits, header->nfat_arch);
	arch = (struct fat_arch *)iscorrup(file, (void*)header + \
			sizeof(*header), sizeof(*arch));
	err = false;
	if (same_arch(file, arch, nfat, &err))
		return (err);
	err = all_arch(file, arch, nfat);
	return (err);
}
