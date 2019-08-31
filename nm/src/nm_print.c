/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 11:53:46 by amansour          #+#    #+#             */
/*   Updated: 2019/08/30 13:52:27 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_nm.h"

bool	    print_output_32(struct symtab_command *sym, t_file *file, uint32_t nsyms)
{
	char			*strtable;
	struct nlist	*symtable;
	uint32_t		i;
	t_sym			data[nsyms];

	i = -1;
	symtable = (struct nlist *)iscorrup(file, (void *)file->ptr + swap32(file->swap_bits, sym->symoff), sizeof(*symtable) * nsyms);
	if (!symtable)
		return(errors(file->filename, CORRUPT_FILE));
	strtable = (char*)iscorrup(file, (void*)file->ptr + swap32(file->swap_bits, sym->stroff), sizeof(*strtable));
	if (!strtable)
		return(errors(file->filename, CORRUPT_FILE));
	while (++i < nsyms)
	{
        //name_offset = swap32(file->swap_bits, sym->stroff) + swap32(file->swap_bits, symtable[i].n_un.n_strx);
		data[i].name_size = swap32(file->swap_bits, sym->strsize) - swap32(file->swap_bits, symtable[i].n_un.n_strx);
		data[i].name = (char*)iscorrup(file, strtable + swap32(file->swap_bits, symtable[i].n_un.n_strx), sizeof(*(data[i].name)));
		if (!(data[i].name))
			data[i].name = BAD_STRING;
        data[i].type = symtable[i].n_type;
		data[i].value = swap32(file->swap_bits, symtable[i].n_value);
		data[i].sect = symtable[i].n_sect;
		data[i].desc = symtable[i].n_desc;
	}
	print_nm(data, nsyms, file->sects, false);
	return (false);
}

bool		print_output_64(struct symtab_command *sym, t_file *file, uint32_t nsyms)
{
	char			*strtable;
	struct nlist_64	*symtable;
	uint32_t		i;
	t_sym			data[nsyms];

	i = -1;
	symtable = (struct nlist_64 *)iscorrup(file, (void *)file->ptr + swap32(file->swap_bits, sym->symoff), sizeof(*symtable) * nsyms);
	if (!symtable)
		return(errors(file->filename, CORRUPT_FILE));
	strtable = (char*)iscorrup(file, (void*)file->ptr + swap32(file->swap_bits, sym->stroff), sizeof(*strtable));
	if (!strtable)
		return(errors(file->filename, CORRUPT_FILE));
	while (++i < nsyms)
	{
		data[i].name_size = swap32(file->swap_bits, sym->strsize) - swap32(file->swap_bits, symtable[i].n_un.n_strx);
		data[i].name = (char*)iscorrup(file, strtable + swap32(file->swap_bits, symtable[i].n_un.n_strx), sizeof(*data[i].name));
		if (!(data[i].name))
			data[i].name = BAD_STRING;
		data[i].type = symtable[i].n_type;
		data[i].value = swap64(file->swap_bits, symtable[i].n_value);
		data[i].sect = symtable[i].n_sect;
		data[i].desc = symtable[i].n_desc;
	}
	print_nm(data, nsyms, file->sects, true);
	return (false);
}
