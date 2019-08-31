/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 10:15:41 by amansour          #+#    #+#             */
/*   Updated: 2019/08/30 15:38:20 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_nm.h"

static bool process(t_file *file, struct load_command *lc, uint32_t MACRO)
{
	int			i;
	uint32_t	nsyms;
	bool		err;

	file->nsects = 0;
	i = -1;
	err = false;
	while (++i < file->ncmds && !err)
	{
		if (!lc)
			return(errors(file->filename, CORRUPT_FILE));
		if (file->arch == ARCH_64 && swap32(file->swap_bits, lc->cmdsize) % 8 != 0)
			return (errors(file->filename, CORRUPT_FILE));
		if (swap32(file->swap_bits, lc->cmd) == MACRO && (MACRO == LC_SEGMENT || MACRO == LC_SEGMENT_64))
		{
			if (MACRO == LC_SEGMENT)
				err = manage_sections_32(file, (void*)lc);
			else
				err = manage_sections_64(file, (void*)lc);
		}
		else if (swap32(file->swap_bits, lc->cmd) == MACRO)
		{
			nsyms = swap32(file->swap_bits, ((struct symtab_command *)lc)->nsyms);
			if (file->arch == ARCH_32)
				err = print_output_32((void*)lc, file, nsyms);
			else
				err = print_output_64((void*)lc, file, nsyms);
		}
		lc = (struct load_command *)iscorrup(file, (void*)lc + swap32(file->swap_bits, lc->cmdsize), sizeof(*lc));
	}
	return (err);
}

bool	handle_mach_o(t_file *file)
{
	struct mach_header_64	*hd64;
	struct mach_header		*hd;
	struct load_command		*lc;
	bool					err;

	if (file->arch == ARCH_32)
	{
		//pb with header
		hd = (struct mach_header *)iscorrup(file, file->ptr, sizeof(*hd));
		if (!hd)
			return(errors(file->filename, CORRUPT_FILE));
		file->ncmds = swap32(file->swap_bits, hd->ncmds);
		lc = (struct load_command *)iscorrup(file, (file->ptr+ sizeof(*hd)), sizeof(*lc));
		err = process(file, lc, LC_SEGMENT);
	}
	else
	{
		//pb with header
		hd64 = (struct mach_header_64 *)iscorrup(file, file->ptr, sizeof(*hd64));
		if (!hd64)
			return(errors(file->filename, CORRUPT_FILE));
		file->ncmds = swap32(file->swap_bits, hd64->ncmds);
		lc = (struct load_command *)iscorrup(file, (file->ptr + sizeof(*hd64)), sizeof(*lc));
		err = process(file, lc, LC_SEGMENT_64);
	}
	if (!err)
		err = process(file, lc, LC_SYMTAB);
	return (err);
}
