/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 11:22:28 by amansour          #+#    #+#             */
/*   Updated: 2019/09/04 12:40:23 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static	void	ft_putnstr(t_file *f, t_sym data)
{
	int i;

	i = 0;
	if ((g_flags & U_FLAG) && (data.type_s != 'U' && data.type != 'u'))
		return ;
	if ((g_flags & BIGU_FLAG) && (data.type_s == 'U' || data.type_s == 'u'))
		return ;
	if (!(g_flags & U_FLAG))
		ft_printf("%c ", data.type_s);
	while (data.name[i] && (i + 1 + f->stroff + data.strname) < f->size)
		++i;
	write(1, data.name, i);
	if (data.type_s == 'I')
	{
		ft_printf(" (indirect for ");
		write(1, data.name, i);
		ft_printf(")");
	}
	ft_putchar('\n');
}

void			print_nm(t_sym data[], uint32_t len, t_file *f, bool is64)
{
	uint32_t	i;

	sort(&data, len);
	get_symbol_letter(data, f->sects, len);
	i = -1;
	while (++i < len)
	{
		if (data[i].type_s == '-')
			continue ;
		if (is64 && !(g_flags & U_FLAG))
		{
			if (data[i].type_s != 'u' && data[i].type_s != 'U')
				ft_printf("%.16llx ", data[i].value);
			else if (!(g_flags & BIGU_FLAG))
				ft_printf("%17c", ' ');
		}
		else if (!(g_flags & U_FLAG))
		{
			if (data[i].type_s != 'u' && data[i].type_s != 'U')
				ft_printf("%.8llx ", data[i].value);
			else if (!(g_flags & BIGU_FLAG))
				ft_printf("%9c", ' ');
		}
		ft_putnstr(f, data[i]);
	}
}
