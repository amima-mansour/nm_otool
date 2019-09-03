/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alpha_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 09:59:32 by amansour          #+#    #+#             */
/*   Updated: 2019/09/03 10:00:08 by amansour         ###   ########.fr       */
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

static uint32_t	select_alpha_max(t_sym data[], uint32_t start, uint32_t finish)
{
	uint32_t index;
	uint32_t i;

	index = start;
	i = start;
	while (++i <= finish)
	{
		if (ft_strcmp(data[i].name, data[index].name) > 0)
			index = i;
		if (ft_strcmp(data[i].name, data[index].name) == 0)
		{
			if (data[i].value > data[index].value)
				index = i;
		}
	}
	return (index);
}

void			alpha_sort(t_sym *data[], uint32_t len, bool rev)
{
	uint32_t	i;
	t_sym		tmp;
	uint32_t	index;

	i = -1;
	while (++i < (len - 1))
	{
		index = (rev) ? select_alpha_max(*data, i, len - 1) : \
				select_alpha_min(*data, i, len - 1);
		tmp = (*data)[i];
		(*data)[i] = (*data)[index];
		(*data)[index] = tmp;
	}
}
