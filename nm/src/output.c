/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 12:07:41 by amansour          #+#    #+#             */
/*   Updated: 2019/09/08 08:40:17 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		add_sym(t_list **symlst, t_sym data, char *filename)
{
	t_list			*lst;

	if (!(lst = ft_lstnew(&data, sizeof(t_sym))))
	{
		errors(filename, MAP_ERROR);
		exit(0);
	}
	add_list(symlst, lst);
}

bool			output_32(struct symtab_command *sym, t_file *f, uint32_t nsyms)
{
	char			*str;
	struct nlist	*table;
	uint32_t		i;
	t_sym			d;

	if (!(table = (struct nlist *)iscorrup(f, (void *)f->ptr + \
					swap32(f->swap_bits, sym->symoff), sizeof(*table) * nsyms)))
		return (errors(f->filename, CORRUPT_FILE));
	f->stroff = swap32(f->swap_bits, sym->stroff);
	if (!(str = (char*)iscorrup(f, (void*)f->ptr + f->stroff, sizeof(*str))))
		return (errors(f->filename, CORRUPT_FILE));
	i = -1;
	while (++i < nsyms)
	{
		d.strname = swap32(f->swap_bits, table[i].n_un.n_strx);
		d.name = (char*)iscorrup(f, str + d.strname, sizeof(*d.name));
		d.name = (!(d.name)) ? BAD_STRING : d.name;
		d.type = table[i].n_type;
		d.value = swap32(f->swap_bits, table[i].n_value);
		d.sect = table[i].n_sect;
		d.index = i;
		add_sym(&(f->syms), d, f->filename);
	}
	print_nm(f, false);
	return (false);
}

bool			output_64(struct symtab_command *sym, t_file *f, uint32_t nsyms)
{
	char			*str;
	struct nlist_64	*table;
	uint32_t		i;
	t_sym			d;

	if (!(table = (struct nlist_64 *)iscorrup(f, (void *)f->ptr + \
					swap32(f->swap_bits, sym->symoff), sizeof(*table) * nsyms)))
		return (errors(f->filename, CORRUPT_FILE));
	f->stroff = swap32(f->swap_bits, sym->stroff);
	if (!(str = (char*)iscorrup(f, (void*)f->ptr + f->stroff, sizeof(*str))))
		return (errors(f->filename, CORRUPT_FILE));
	i = -1;
	while (++i < nsyms)
	{
		d.strname = swap32(f->swap_bits, table[i].n_un.n_strx);
		d.name = (char*)iscorrup(f, str + d.strname, sizeof(*d.name));
		d.name = (!(d.name)) ? BAD_STRING : d.name;
		d.type = table[i].n_type;
		d.value = swap64(f->swap_bits, table[i].n_value);
		d.sect = table[i].n_sect;
		d.index = i;
		add_sym(&(f->syms), d, f->filename);
	}
	print_nm(f, true);
	return (false);
}
