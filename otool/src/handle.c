/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 10:15:41 by amansour          #+#    #+#             */
/*   Updated: 2019/09/04 11:21:42 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static bool	process_64(t_file *f, struct load_command *lc)
{
	int			i;
	bool		err;

	f->nsects = 0;
	i = -1;
	err = false;
	while (++i < f->ncmds && !err)
	{
		if (!lc)
			return (errors(f->filename, CORRUPT_FILE));
		if (swap32(f->swap_bits, lc->cmdsize) % 8 != 0)
			return (errors(f->filename, CORRUPT_FILE));
		if (swap32(f->swap_bits, lc->cmd) == LC_SEGMENT_64)
			err = manage_sections_64(f, (void*)lc);
		if (err)
			return (err);
		lc = (struct load_command *)iscorrup(f, (void*)lc + \
				swap32(f->swap_bits, lc->cmdsize), sizeof(*lc));
	}
	output(f, true);
	return (err);
}

static bool	process_32(t_file *f, struct load_command *lc)
{
	int			i;
	bool		err;

	f->nsects = 0;
	i = -1;
	err = false;
	while (++i < f->ncmds && !err)
	{
		if (!lc)
			return (errors(f->filename, CORRUPT_FILE));
		if (swap32(f->swap_bits, lc->cmd) == LC_SEGMENT)
			err = manage_sections_32(f, (void*)lc);
		if (err)
			return (err);
		lc = (struct load_command *)iscorrup(f, (void*)lc + \
				swap32(f->swap_bits, lc->cmdsize), sizeof(*lc));
	}
	output(f, false);
	return (err);
}

bool		handle_mach_o(t_file *f)
{
	struct mach_header_64	*hd64;
	struct mach_header		*hd;
	struct load_command		*lc;

	if (f->arch == ARCH_32)
	{
		if (!(hd = (struct mach_header *)iscorrup(f, f->ptr, sizeof(*hd))))
			return (errors(f->filename, CORRUPT_FILE));
		f->cpu = hd->cputype;
		f->ncmds = swap32(f->swap_bits, hd->ncmds);
		lc = (struct load_command *)iscorrup(f, \
				(f->ptr + sizeof(*hd)), sizeof(*lc));
		return (process_32(f, lc));
	}
	if (!(hd64 = (struct mach_header_64 *)iscorrup(f, f->ptr, sizeof(*hd64))))
		return (errors(f->filename, CORRUPT_FILE));
	f->cpu = hd64->cputype;
	f->ncmds = swap32(f->swap_bits, hd64->ncmds);
	lc = (struct load_command *)iscorrup(f, (f->ptr + \
				sizeof(*hd64)), sizeof(*lc));
	return (process_64(f, lc));
}
