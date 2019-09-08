/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 11:22:28 by amansour          #+#    #+#             */
/*   Updated: 2019/09/07 15:14:54 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static	void	ft_putnstr(t_file *f, t_sym *data)
{
	int i;

	i = 0;
	if ((g_flags & U_FLAG) && (data->type_s != 'U' && data->type != 'u'))
		return ;
	if ((g_flags & BIGU_FLAG) && (data->type_s == 'U' || data->type_s == 'u'))
		return ;
	if (!(g_flags & U_FLAG))
		ft_printf("%c ", data->type_s);
	if (ft_strcmp(data->name, BAD_STRING) == 0)
	{
		ft_printf("%s\n", data->name);
		return ;
	}
	while (data->name[i] && (i + 1 + f->stroff + data->strname) < f->size)
		++i;
	write(1, data->name, i);
	ft_putchar('\n');
}

static void		print_addr(t_list *data, bool is64)
{
	if (is64 && !(g_flags & U_FLAG))
	{
		if (((t_sym*)(data->content))->type_s != 'u' && \
		((t_sym*)(data->content))->type_s != 'U')
			ft_printf("%.16llx ", ((t_sym*)(data->content))->value);
		else if (!(g_flags & BIGU_FLAG))
			ft_printf("%17c", ' ');
	}
	else if (!(g_flags & U_FLAG))
	{
		if (((t_sym*)(data->content))->type_s != 'u' && \
		((t_sym*)(data->content))->type_s != 'U')
			ft_printf("%.8llx ", ((t_sym*)(data->content))->value);
		else if (!(g_flags & BIGU_FLAG))
			ft_printf("%9c", ' ');
	}
}

void			print_list(t_list *lst)
{
	t_list *l;
	int		i;

	l = lst;
	i = 0;
	while (l)
	{
		ft_printf("%u\n", ((t_sym *)((l)->content))->index);
		l = l->next;
		i += 1;
	}
	ft_printf("len = %u\n", i);
}

void			print_nm(t_file *f, bool is64)
{
	t_list		*data;

	(g_multi_file) ? ft_printf("\n%s:\n", f->filename) : 0;
	//print_list(f->syms);
	sort(&(f->syms));
	get_symbol_letter(f->syms, f->sects);
	data = f->syms;
	while (data)
	{
		//ft_printf("INDEX\n");
		//print_list(data);
		if (((t_sym*)(data->content))->type_s == '-')
		{
			data = data->next;
			continue;
		}
		print_addr(data, is64);
		ft_putnstr(f, data->content);
		data = data->next;
	}
	free_list(f->syms);
}
