/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 10:15:41 by amansour          #+#    #+#             */
/*   Updated: 2019/09/04 12:30:12 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static bool	process_64(t_file *f, struct load_command *lc, uint32_t macro)
{
	int			i;
	uint32_t	nsyms;
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
		if (swap32(f->swap_bits, lc->cmd) == macro && macro == LC_SEGMENT_64)
			err = manage_sections_64(f, (void*)lc);
		else if (swap32(f->swap_bits, lc->cmd) == macro)
		{
			nsyms = swap32(f->swap_bits, ((struct symtab_command *)lc)->nsyms);
			err = output_64((void*)lc, f, nsyms);
		}
		lc = (struct load_command *)iscorrup(f, (void*)lc + \
				swap32(f->swap_bits, lc->cmdsize), sizeof(*lc));
	}
	return (err);
}

static bool	process_32(t_file *f, struct load_command *lc, uint32_t macro)
{
	int			i;
	uint32_t	nsyms;
	bool		err;

	f->nsects = 0;
	i = -1;
	err = false;
	while (++i < f->ncmds && !err)
	{
		if (!lc)
			return (errors(f->filename, CORRUPT_FILE));
		if (swap32(f->swap_bits, lc->cmd) == macro && macro == LC_SEGMENT)
			err = manage_sections_32(f, (void*)lc);
		else if (swap32(f->swap_bits, lc->cmd) == macro)
		{
			nsyms = swap32(f->swap_bits, ((struct symtab_command *)lc)->nsyms);
			err = output_32((void*)lc, f, nsyms);
		}
		lc = (struct load_command *)iscorrup(f, (void*)lc + \
				swap32(f->swap_bits, lc->cmdsize), sizeof(*lc));
	}
	return (err);
}

bool		handle_mach_o(t_file *f)
{
	struct mach_header_64	*hd64;
	struct mach_header		*hd;
	struct load_command		*lc;
	bool					err;

	if (f->arch == ARCH_32)
	{
		if (!(hd = (struct mach_header *)iscorrup(f, f->ptr, sizeof(*hd))))
			return (errors(f->filename, CORRUPT_FILE));
		f->ncmds = swap32(f->swap_bits, hd->ncmds);
		lc = (struct load_command *)iscorrup(f, \
				(f->ptr + sizeof(*hd)), sizeof(*lc));
		if (!(err = process_32(f, lc, LC_SEGMENT)))
			return (process_32(f, lc, LC_SYMTAB));
		return (err);
	}
	if (!(hd64 = (struct mach_header_64 *)iscorrup(f, f->ptr, sizeof(*hd64))))
		return (errors(f->filename, CORRUPT_FILE));
	f->ncmds = swap32(f->swap_bits, hd64->ncmds);
	lc = (struct load_command *)iscorrup(f, (f->ptr + \
				sizeof(*hd64)), sizeof(*lc));
	if (!(err = process_64(f, lc, LC_SEGMENT_64)))
		return (process_64(f, lc, LC_SYMTAB));
	return (err);
}
