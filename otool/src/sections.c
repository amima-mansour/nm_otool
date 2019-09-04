/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 10:16:07 by amansour          #+#    #+#             */
/*   Updated: 2019/09/04 15:28:25 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

bool	manage_sections_32(t_file *file, void *lc)
{
	struct section	*sect;
	uint32_t		nsects;
	uint32_t		i;

	nsects = swap32(file->swap_bits, ((struct segment_command*)lc)->nsects);
	sect = (struct section *)iscorrup(file, (void*)lc + \
			sizeof(struct segment_command), sizeof(*sect));
	i = -1;
	while (++i < nsects)
	{
		if (!sect)
			return (errors(file->filename, CORRUPT_FILE));
		if (ft_strcmp(sect->sectname, "__text") == 0)
		{
			file->sect_32.offset = swap32(file->swap_bits, sect->offset);
			file->sect_32.size = swap32(file->swap_bits, sect->size);
			file->sect_32.addr = swap32(file->swap_bits, sect->addr);
		}
		sect = sect + 1;
	}
	return (false);
}

bool	manage_sections_64(t_file *file, void *lc)
{
	struct section_64		*sect;
	uint32_t				nsects;
	uint32_t				i;

	nsects = swap32(file->swap_bits, ((struct segment_command_64 *)lc)->nsects);
	sect = (struct section_64 *)iscorrup(file, (void*)lc + \
			sizeof(struct segment_command_64), sizeof(*sect));
	i = -1;
	while (++i < nsects)
	{
		if (!sect)
			return (errors(file->filename, CORRUPT_FILE));
		if (ft_strcmp(sect->sectname, "__text") == 0)
		{
			file->sect_64.offset = swap32(file->swap_bits, sect->offset);
			file->sect_64.size = swap64(file->swap_bits, sect->size);
			file->sect_64.addr = swap64(file->swap_bits, sect->addr);
			break ;
		}
		sect = sect + 1;
	}
	return (false);
}
