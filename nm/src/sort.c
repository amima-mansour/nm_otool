/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 10:01:02 by amansour          #+#    #+#             */
/*   Updated: 2019/09/03 10:01:22 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		sort(t_sym *data[], uint32_t len)
{
	bool rev;

	rev = false;
	if (g_flags & P_FLAG)
		return ;
	if (g_flags & R_FLAG)
		rev = 1;
	if (g_flags & N_FLAG)
		num_sort(data, len, rev);
	else
		alpha_sort(data, len, rev);
}
