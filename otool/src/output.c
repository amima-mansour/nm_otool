/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 12:07:41 by amansour          #+#    #+#             */
/*   Updated: 2019/09/02 13:35:30 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void custom_print(t_file *file, uint64_t index, bool is64, int64_t size)
{
	int 		j;
	void 		*offset;
	uint32_t	text_off;

	j = -1;
	text_off = (is64) ? file->sect_64.offset : file->sect_32.offset; 
	while (++j < 16)
	{
		offset = (void *)file->ptr + text_off + j + index;
		if (offset >= (void*)file->ptr + text_off + size)
			return ;
		ft_printf("%-3.2hhx", (uint8_t)swap32(file->swap_bits, *(uint32_t*)offset));
	}
}

static void compact_print(t_file *file, uint64_t index, bool is64, int64_t size)
{
	int			j;
	void 		*offset;
	uint32_t	text_off;

	j = 0;
	text_off = (is64) ? file->sect_64.offset : file->sect_32.offset; 
	while (j < 16)
	{
		offset = (void *)file->ptr + text_off + index + j;
		if (offset >= (void*)file->ptr + text_off + size)
			return ;
		ft_printf("%-9.8x", (uint32_t)swap32(file->swap_bits, *(uint32_t *)offset));
        j += 4;
	}
}

bool	output(t_file *f, bool is64)
{
	void		*offset;
	uint64_t	i;
	uint64_t	size;

	size = (is64) ? f->sect_64.size : f->sect_32.size;
	offset = (is64) ? (void *)f->ptr + f->sect_64.offset
	: (void *)f->ptr + f->sect_32.offset;
	if (iscorrup(f, offset, size) == NULL)
		return (errors(f->filename, CORRUPT_FILE));
	i = 0;
	while (i < size)
	{
		if (is64)
			ft_printf("%.16llx\t", f->sect_64.addr + i);
		else
			ft_printf("%.8llx\t", f->sect_32.addr + i);
		if (f->cpu == CPU_TYPE_X86 || f->cpu == CPU_TYPE_X86_64 
			|| f->cpu == CPU_TYPE_I386)
			custom_print(f, i, is64, size);
		else
			compact_print(f, i, is64, size);
		ft_putchar('\n');
		i += 16;
	}
	return (false);
}
