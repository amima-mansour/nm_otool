/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 10:00:51 by amansour          #+#    #+#             */
/*   Updated: 2019/09/03 10:00:52 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static uint32_t	select_num_min(t_sym data[], uint32_t start, uint32_t finish)
{
	uint32_t index;
	uint32_t i;

	index = start;
	i = start;
	while (++i <= finish)
	{
		if (data[i].value < data[index].value)
			index = i;
	}
	return (index);
}

static uint32_t	select_num_max(t_sym data[], uint32_t start, uint32_t finish)
{
	uint32_t index;
	uint32_t i;

	index = start;
	i = start;
	while (++i <= finish)
	{
		if (data[i].value > data[index].value)
			index = i;
	}
	return (index);
}

void			num_sort(t_sym *data[], uint32_t len, bool rev)
{
	uint32_t	i;
	t_sym		tmp;
	uint32_t	index;

	i = -1;
	while (++i < (len - 1))
	{
		index = (rev) ? select_num_max(*data, i, len - 1) : \
				select_num_min(*data, i, len - 1);
		tmp = (*data)[i];
		(*data)[i] = (*data)[index];
		(*data)[index] = tmp;
	}
}
