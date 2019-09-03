/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 12:07:41 by amansour          #+#    #+#             */
/*   Updated: 2019/09/02 13:35:30 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

bool	output_32(struct symtab_command *sym, t_file *f, uint32_t nsyms)
{
	char			*str;
	struct nlist	*table;
	uint32_t		i;
	t_sym			data[nsyms];

	if (!(table = (struct nlist *)iscorrup(f, (void *)f->ptr + \
					swap32(f->swap_bits, sym->symoff), sizeof(*table) * nsyms)))
		return (errors(f->filename, CORRUPT_FILE));
	if (!(str = (char*)iscorrup(f, (void*)f->ptr + \
					swap32(f->swap_bits, sym->stroff), sizeof(*str))))
		return (errors(f->filename, CORRUPT_FILE));
	i = -1;
	while (++i < nsyms)
	{
		data[i].name = (char*)iscorrup(f, str + swap32(f->swap_bits, \
					table[i].n_un.n_strx), sizeof(*(data[i].name)));
		data[i].name = (!(data[i].name)) ? BAD_STRING : data[i].name;
		data[i].type = table[i].n_type;
		data[i].value = swap32(f->swap_bits, table[i].n_value);
		data[i].sect = table[i].n_sect;
		data[i].desc = table[i].n_desc;
	}
	(g_multi_file) ? ft_printf("\n%s:\n", f->filename) : 0;
	print_nm(data, nsyms, f->sects, false);
	return (false);
}

bool	output_64(struct symtab_command *sym, t_file *f, uint32_t nsyms)
{
	char			*str;
	struct nlist_64	*table;
	uint32_t		i;
	t_sym			data[nsyms];

	i = -1;
	if (!(table = (struct nlist_64 *)iscorrup(f, (void *)f->ptr + \
					swap32(f->swap_bits, sym->symoff), sizeof(*table) * nsyms)))
		return (errors(f->filename, CORRUPT_FILE));
	if (!(str = (char*)iscorrup(f, (void*)f->ptr + \
					swap32(f->swap_bits, sym->stroff), sizeof(*str))))
		return (errors(f->filename, CORRUPT_FILE));
	while (++i < nsyms)
	{
		data[i].name = (char*)iscorrup(f, str + swap32(f->swap_bits, \
					table[i].n_un.n_strx), sizeof(*data[i].name));
		data[i].name = (!(data[i].name)) ? BAD_STRING : data[i].name;
		data[i].type = table[i].n_type;
		data[i].value = swap64(f->swap_bits, table[i].n_value);
		data[i].sect = table[i].n_sect;
		data[i].desc = table[i].n_desc;
	}
	(g_multi_file) ? ft_printf("\n%s:\n", f->filename) : 0;
	print_nm(data, nsyms, f->sects, true);
	return (false);
}