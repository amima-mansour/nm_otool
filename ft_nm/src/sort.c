/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 11:22:28 by amansour          #+#    #+#             */
/*   Updated: 2019/08/30 14:54:01 by amansour         ###   ########.fr       */
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

static uint32_t	select_alpha_min(t_sym data[], uint32_t start, uint32_t finish)
{
	uint32_t index;
	uint32_t i;

	index = start;
	i = start;
	while (++i <= finish)
	{
		if (ft_strcmp(data[i].name, data[index].name) < 0)
			index = i;
		if (ft_strcmp(data[i].name, data[index].name) == 0)
		{
			if (data[i].value < data[index].value)
				index = i;
		}
	}
	return (index);
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
}

static void		get_symbol_letter(t_sym *sym, t_list *sects)
{
	sym->type_s = '?';
    if (N_STAB & sym->type)
		sym->type_s = '-';
	else if ((N_TYPE & sym->type) == N_UNDF && sym->value != 0 && (sym->type & N_EXT))
		sym->type_s = 'C';
	else if ((N_TYPE & sym->type) == N_UNDF && (sym->type & N_EXT))
        sym->type_s = 'U';
    else if ((N_TYPE & sym->type) == N_ABS)
		sym->type_s = 'A';
	else if ((N_TYPE & sym->type) == N_SECT)
		match_symbol_section(sects, sym);
	else if ((N_TYPE & sym->type) == N_INDR)
		sym->type_s = 'I';
	if (!(N_EXT & sym->type))
		sym->type_s = ft_tolower(sym->type_s);
}

void		alpha_sort(t_sym data[], uint32_t len, t_list *sects, bool is64)
{
	uint32_t	i;
	t_sym		tmp;
	uint32_t	index_min;

	i = -1;
	while (++i < (len - 1))
	{
		index_min = select_alpha_min(data, i, len - 1);
		tmp = data[i];
		data[i] = data[index_min];
		data[index_min] = tmp;
	}
	i = -1;
	while (++i < len)
		get_symbol_letter(&(data[i]), sects);
	i = -1;
	while (++i < len)
	{
		if (data[i].type_s == '-')
			continue ;
		if (is64)
        {
            if (data[i].type_s != 'u' && data[i].type_s != 'U')
                ft_printf("%.16llx ", data[i].value);
            else
                ft_printf("%17c", ' ');
        }
        else
        {
            if (data[i].type_s != 'u' && data[i].type_s != 'U')
                ft_printf("%.8llx ", data[i].value);
            else
                ft_printf("%9c", ' ');
        }
        ft_printf("%c %s\n", data[i].type_s, data[i].name);
	}
}

