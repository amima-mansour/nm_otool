/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 12:07:41 by amansour          #+#    #+#             */
/*   Updated: 2019/09/04 11:42:08 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

bool	output_32(struct symtab_command *sym, t_file *f, uint32_t nsyms)
{
	char			*str;
	struct nlist	*table;
	uint32_t		i;
	t_sym			d[nsyms];

	if (!(table = (struct nlist *)iscorrup(f, (void *)f->ptr + \
					swap32(f->swap_bits, sym->symoff), sizeof(*table) * nsyms)))
		return (errors(f->filename, CORRUPT_FILE));
	f->stroff = swap32(f->swap_bits, sym->stroff);
	if (!(str = (char*)iscorrup(f, (void*)f->ptr + f->stroff, sizeof(*str))))
		return (errors(f->filename, CORRUPT_FILE));
	i = -1;
	while (++i < nsyms)
	{
		d[i].strname = swap32(f->swap_bits, table[i].n_un.n_strx);
		d[i].name = (char*)iscorrup(f, str + d[i].strname, sizeof(*d[i].name));
		d[i].name = (!(d[i].name)) ? BAD_STRING : d[i].name;
		d[i].type = table[i].n_type;
		d[i].value = swap32(f->swap_bits, table[i].n_value);
		d[i].sect = table[i].n_sect;
		d[i].desc = table[i].n_desc;
	}
	(g_multi_file) ? ft_printf("\n%s:\n", f->filename) : 0;
	print_nm(d, nsyms, f, false);
	return (false);
}

bool	output_64(struct symtab_command *sym, t_file *f, uint32_t nsyms)
{
	char			*str;
	struct nlist_64	*table;
	uint32_t		i;
	t_sym			d[nsyms];

	if (!(table = (struct nlist_64 *)iscorrup(f, (void *)f->ptr + \
					swap32(f->swap_bits, sym->symoff), sizeof(*table) * nsyms)))
		return (errors(f->filename, CORRUPT_FILE));
	f->stroff = swap32(f->swap_bits, sym->stroff);
	if (!(str = (char*)iscorrup(f, (void*)f->ptr + f->stroff, sizeof(*str))))
		return (errors(f->filename, CORRUPT_FILE));
	i = -1;
	while (++i < nsyms)
	{
		d[i].strname = swap32(f->swap_bits, table[i].n_un.n_strx);
		d[i].name = (char*)iscorrup(f, str + d[i].strname, sizeof(*d[i].name));
		d[i].name = (!(d[i].name)) ? BAD_STRING : d[i].name;
		d[i].type = table[i].n_type;
		d[i].value = swap64(f->swap_bits, table[i].n_value);
		d[i].sect = table[i].n_sect;
		d[i].desc = table[i].n_desc;
	}
	(g_multi_file) ? ft_printf("\n%s:\n", f->filename) : 0;
	print_nm(d, nsyms, f, true);
	return (false);
}
