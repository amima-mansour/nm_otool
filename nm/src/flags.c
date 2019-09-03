/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 10:02:33 by amansour          #+#    #+#             */
/*   Updated: 2019/09/03 10:03:22 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		save_flags(char *str)
{
	char		*option;
	int			i;

	i = 0;
	while (str[i] && str[i] == '-')
		i++;
	while (str[i])
	{
		if ((option = ft_strchr(OPTIONS, str[i])) == NULL)
			return (1);
		else if (str[i] == 'p')
			g_flags |= P_FLAG;
		else if (str[i] == 'n')
			g_flags |= N_FLAG;
		else if (str[i] == 'r')
			g_flags |= R_FLAG;
		else if (str[i] == 'u')
			g_flags |= U_FLAG;
		else
			g_flags |= BIGU_FLAG;
		i++;
	}
	return (0);
}

int				get_flags(int *argc, char ***argv)
{
	int		err;
	char	**av;

	err = 0;
	while (--(*argc))
	{
		av = *argv;
		++(*argv);
		if (***argv != '-')
			break ;
		if ((err = save_flags(**argv)) == 1)
			return (err);
	}
	*argc += 1;
	*argv = av;
	return (err);
}
