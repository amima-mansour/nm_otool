/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 12:12:03 by amansour          #+#    #+#             */
/*   Updated: 2019/09/08 09:05:58 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static t_sect	*find_section(t_list *lst, uint8_t n_sect)
{
	while (lst && lst->content)
	{
		if (((t_sect *)lst->content)->index == n_sect)
			return (lst->content);
		lst = lst->next;
	}
	return (NULL);
}

static void		match_symbol_section(t_list *sect_lst, t_sym *sym)
{
	t_sect *sect;

	if ((sect = find_section(sect_lst, sym->sect)))
	{
		if (!ft_strcmp(sect->name, SECT_TEXT))
			sym->type_s = 'T';
		else if (!ft_strcmp(sect->name, SECT_DATA))
			sym->type_s = 'D';
		else if (!ft_strcmp(sect->name, SECT_BSS))
			sym->type_s = 'B';
		else
			sym->type_s = 'S';
	}
	else
		sym->type_s = 'S';
}

static void		get_single_symbol(t_list *lst, t_list *sects)
{
	t_sym *sym;

	sym = ((t_sym*)(lst->content));
	sym->type_s = '?';
	if (N_STAB & sym->type)
		sym->type_s = '-';
	else if ((N_TYPE & sym->type) == N_UNDF && \
			sym->value != 0 && sym->type & N_EXT)
		sym->type_s = 'C';
	else if ((N_TYPE & sym->type) == N_UNDF && (sym->type & N_EXT))
		sym->type_s = 'U';
	else if ((N_TYPE & sym->type) == N_ABS)
		sym->type_s = 'A';
	else if ((N_TYPE & sym->type) == N_SECT)
		match_symbol_section(sects, sym);
	else if ((N_TYPE & sym->type) == N_INDR)
		sym->type_s = 'I';
	if (!(N_EXT & sym->type) && ft_isalpha(sym->type_s))
		sym->type_s = ft_tolower(sym->type_s);
}

void			get_symbol_letter(t_list *data, t_list *sects)
{
	uint32_t	i;
	t_list		*tmp;

	i = 0;
	tmp = data;
	while (tmp)
	{
		get_single_symbol(tmp, sects);
		tmp = tmp->next;
	}
	free_list(sects);
}
