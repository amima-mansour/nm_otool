/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 11:44:56 by amansour          #+#    #+#             */
/*   Updated: 2019/09/02 12:18:46 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

uint32_t			swap32(bool value, uint32_t n)
{
	if (value == true)
		return ((n >> 24) & 0xff)
		| ((n >> 8) & 0xff00)
		| ((n << 8) & 0xff0000)
		| ((n << 24) & 0xff000000);
	return (n);
}

uint64_t			swap64(bool value, uint64_t n)
{
	if (value == true)
		return ((n & 0xff00000000000000) >> 56 \
			| (n & 0x00ff000000000000) >> 40 \
			| (n & 0x0000ff0000000000) >> 24 \
			| (n & 0x000000ff00000000) >> 8 \
			| (n & 0x00000000ff000000) << 8 \
			| (n & 0x0000000000ff0000) << 24 \
			| (n & 0x000000000000ff00) << 40 \
			| (n & 0x00000000000000ff) << 56);
		return (n);
}

bool				is_swap(uint32_t magic)
{
	if (magic == MH_CIGAM_64 || magic == MH_CIGAM || \
			magic == FAT_CIGAM || magic == FAT_CIGAM_64)
		return (true);
	return (false);
}
