/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 10:00:51 by amansour          #+#    #+#             */
/*   Updated: 2019/09/08 09:20:18 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static t_list	*select_num_min(t_list *data)
{
	t_list		*el;

	el = data;
	data = data->next;
	while (data)
	{
		if (((t_sym *)(data->content))->value < ((t_sym *)(el->content))->value)
			el = data;
		data = data->next;
	}
	return (el);
}

static t_list	*select_num_max(t_list *data)
{
	t_list		*el;

	el = data;
	data = data->next;
	while (data)
	{
		if (((t_sym *)(data->content))->value >= ((t_sym *)(el->content))->value)
			el = data;
		data = data->next;
	}
	return (el);
}

void			num_sort(t_list **syms, bool rev)
{
	t_list		*lst;
	t_list		*sort;

	sort = NULL;
	while ((*syms)->next)
	{
		lst = (rev) ? select_num_max(*syms) : \
				select_num_min(*syms);
		remove_lst(syms, lst);
		lst->next = NULL;
		add_list(&sort, lst);
	}
	add_list(&sort, *syms);
	*syms = sort;
}
