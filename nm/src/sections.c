/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 10:16:07 by amansour          #+#    #+#             */
/*   Updated: 2019/09/04 11:24:58 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

bool	manage_sections_32(t_file *file, void *lc)
{
	struct section	*sect;
	t_sect			mysect;
	t_list			*lst;
	uint32_t		nsects;
	uint32_t		i;

	nsects = swap32(file->swap_bits, ((struct segment_command*)lc)->nsects);
	sect = (struct section *)iscorrup(file, (void*)lc + \
			sizeof(struct segment_command), sizeof(*sect));
	i = -1;
	while (++i < nsects)
	{
		if (!sect || sect->size > ((struct segment_command *)lc)->vmsize)
			return (errors(file->filename, CORRUPT_FILE));
		file->nsects++;
		mysect.name = ((struct section *)sect)->sectname;
		mysect.index = file->nsects;
		if (!(lst = ft_lstnew(&mysect, sizeof(t_sect))))
			return (errors(file->filename, MAP_ERROR));
		ft_lstadd(&(file->sects), lst);
		sect = sect + 1;
	}
	return (false);
}

bool	manage_sections_64(t_file *file, void *lc)
{
	struct section_64		*sect;
	t_sect					mysect;
	t_list					*lst;
	uint32_t				nsects;
	uint32_t				i;

	nsects = swap32(file->swap_bits, ((struct segment_command_64 *)lc)->nsects);
	sect = (struct section_64 *)iscorrup(file, (void*)lc + \
			sizeof(struct segment_command_64), sizeof(*sect));
	i = -1;
	while (++i < nsects)
	{
		if (!sect || sect->size > ((struct segment_command_64*)lc)->vmsize)
			return (errors(file->filename, CORRUPT_FILE));
		file->nsects++;
		mysect.name = ((struct section_64 *)sect)->sectname;
		mysect.index = file->nsects;
		if (!(lst = ft_lstnew(&mysect, sizeof(t_sect))))
			return (errors(file->filename, MAP_ERROR));
		ft_lstadd(&(file->sects), lst);
		sect = sect + 1;
	}
	return (false);
}
