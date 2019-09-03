/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 11:22:28 by amansour          #+#    #+#             */
/*   Updated: 2019/09/02 14:29:26 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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

static void		alpha_sort(t_sym *data[], uint32_t len)
{
	uint32_t	i;
	t_sym		tmp;
	uint32_t	index_min;

	i = -1;
	while (++i < (len - 1))
	{
		index_min = select_alpha_min(*data, i, len - 1);
		tmp = (*data)[i];
		(*data)[i] = (*data)[index_min];
		(*data)[index_min] = tmp;
	}
}

static	void	ft_putnstr(t_file *f, t_sym data)
{
	int i;

	i = 0;
	while (data.name[i] && (i + 1 + f->stroff + data.name_size) < f->size)
		++i;
	//ft_printf("I = %d\n", i);
	//ft_printf("strname = %u\n", data.name_size);
	//ft_printf("stroff = %u\n", f->stroff);
	write(1, data.name, i);
	ft_putchar('\n');
}

void			print_nm(t_sym data[], uint32_t len, t_file *f, bool is64)
{
	uint32_t	i;

	alpha_sort(&data, len);
	get_symbol_letter(data, f->sects, len);
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
		ft_printf("%c ", data[i].type_s);
		ft_putnstr(f, data[i]);
	}
}
