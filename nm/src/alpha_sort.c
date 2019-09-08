/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alpha_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 09:59:32 by amansour          #+#    #+#             */
/*   Updated: 2019/09/07 14:29:35 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static t_list	*select_alpha_min(t_list *data)
{
	t_list		*el;
	int			i;

	el = data;
	i = 0;
	data = data->next;
	while (data)
	{
		//ft_printf("renteri = %.5s\n", ((t_sym*)(data->content))->name);
		if (ft_strcmp(((t_sym *)(data->content))->name, \
		((t_sym *)(el->content))->name) < 0)
			el = data;
		//ft_printf("sortie = %.5s\n", ((t_sym *)(data->content))->name);
		if (ft_strcmp(((t_sym *)(data->content))->name, \
		((t_sym *)(el->content))->name) == 0)
		{
			if (((t_sym *)(data->content))->value < \
			((t_sym *)(el->content))->value)
				el = data;
		}
		//ft_printf("sortie = %s\n I = %d\n", ((t_sym*)(el->content))->name, ++i);
		data = data->next;
	}
	return (el);
}

static t_list	*select_alpha_max(t_list *data)
{
	t_list		*el;

	el = data;
	data = data->next;
	while (data)
	{
		if (ft_strcmp(((t_sym *)(data->content))->name, \
		((t_sym *)(el->content))->name) > 0)
			el = data;
		if (ft_strcmp(((t_sym *)(data->content))->name, \
		((t_sym *)(el->content))->name) == 0)
		{
			if (((t_sym *)(data->content))->value > \
			((t_sym *)(el->content))->value)
				el = data;
		}
		data = data->next;
	}
	return (el);
}

void			remove_lst(t_list **syms, t_list *lst)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = *syms;
	prev = NULL;
	while (tmp && (((t_sym *)(tmp->content))->index != \
	((t_sym *)(lst->content))->index))
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp)
	{
		if (prev)
			prev->next = tmp->next;
		else
			*syms = tmp->next;
	}
}

void			alpha_sort(t_list **syms, bool rev)
{
	t_list		*lst;
	t_list		*sort;

	sort = NULL;
	while ((*syms)->next)
	{
		lst = (rev) ? select_alpha_max(*syms) : \
				select_alpha_min(*syms);
		remove_lst(syms, lst);
		lst->next = NULL;
		add_list(&sort, lst);
	}
	add_list(&sort, *syms);
	*syms = sort;
}
