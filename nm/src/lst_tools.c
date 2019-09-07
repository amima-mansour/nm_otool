/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 14:27:00 by amansour          #+#    #+#             */
/*   Updated: 2019/09/07 14:27:35 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		free_list(t_list *lst)
{
	t_list		*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp->content);
		free(tmp);
		tmp = NULL;
	}
}

void		add_list(t_list **l, t_list *lst)
{
	t_list	*tmp;
	t_list	*prev;

	prev = NULL;
	tmp = *l;
	while (tmp)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (prev)
		prev->next = lst;
	else
		*l = lst;
}
