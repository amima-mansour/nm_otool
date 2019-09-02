/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 10:55:46 by amansour          #+#    #+#             */
/*   Updated: 2019/09/02 15:37:28 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void			*iscorrup(t_file *file, void *ptr, uint64_t size)
{
	if ((uint64_t)(ptr + size - file->ptr) > file->size)
		return (NULL);
	return (ptr);
}

bool			errors(char *filename, char *msg)
{
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	return (true);
}
