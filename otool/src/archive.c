/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 10:43:26 by amansour          #+#    #+#             */
/*   Updated: 2019/09/03 14:56:52 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static	uint32_t	extract_padding(char *ar_name)
{
	uint32_t	ar_str_size;

	if ((ft_strncmp(ar_name, AR_EFMT1, ft_strlen(AR_EFMT1))) == 0)
		ar_str_size = ft_atoi(ar_name + ft_strlen(AR_EFMT1));
	else
		ar_str_size = 0;
	return (ar_str_size);
}

static	bool		list_all(t_file *f, struct ranlib *ran, uint32_t r_size, \
		uint32_t t_size)
{
	struct ar_hdr	*hd;
	char			*n;
	uint32_t		size;

	hd = iscorrup(f, (void*)ran + r_size + sizeof(uint32_t) + \
			t_size, sizeof(*hd));
	if (!hd)
		return (errors(f->filename, CORRUPT_FILE));
	while (hd)
	{
		n = (char *)iscorrup(f, hd + 1, sizeof(*n));
		if (!n)
			return (errors(f->filename, CORRUPT_FILE));
		size = ft_atoi(hd->ar_size);
		if (iscorrup(f, (void *)n, size) == NULL)
			return (errors(f->filename, CORRUPT_FILE));
		if (otool((void *)n + extract_padding(hd->ar_name), size, \
					n, f->filename))
			return (true);
		hd = iscorrup(f, (void *)hd + sizeof(*hd) + size, sizeof(*hd));
	}
	return (true);
}

bool				handle_archive(t_file *f)
{
	struct ar_hdr	*hd;
	struct ranlib	*ran;
	uint32_t		r_size;
	uint32_t		t_size;

	if (!(hd = (struct ar_hdr*)iscorrup(f, f->ptr + SARMAG, sizeof(*hd))))
		return (false);
	r_size = *(uint32_t*)((void*)(hd + 1) + extract_padding(hd->ar_name));
	if (iscorrup(f, hd + 1, sizeof(r_size)) == NULL)
		return (errors(f->filename, CORRUPT_FILE));
	ran = (struct ranlib*)iscorrup(f, (void*)(hd + 1) + \
			extract_padding(hd->ar_name) + sizeof(uint32_t), sizeof(*ran));
	if (!ran || iscorrup(f, (void*)ran + r_size, sizeof(t_size)) == NULL)
	{
		if (!iscorrup(f, (void*)ran + r_size, sizeof(t_size)))
			return (errors(f->filename, CORRUPT_FILE));
	}
	t_size = *(uint32_t*)((void*)ran + r_size);
	list_all(f, ran, r_size, t_size);
	return (false);
}
