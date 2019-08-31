#include "ft_nm.h"
static t_sect	*find_section(t_list *lst, uint8_t n_sect)
{
	while (lst && lst->content)
	{
		if (((t_sect *)lst->content)->index == n_sect)
			return (lst->content);
		lst = lst->next;
	}
	return (NULL);
}

static void		match_symbol_section(t_list *sect_lst, t_sym *sym)
{
	t_sect *sect;

	if ((sect = find_section(sect_lst, sym->sect)))
	{
		if (!ft_strcmp(sect->name, SECT_TEXT))
			sym->type_s = 'T';
		else if (!ft_strcmp(sect->name, SECT_DATA))
			sym->type_s = 'D';
		else if (!ft_strcmp(sect->name, SECT_BSS))
			sym->type_s = 'B';
		else
			sym->type_s = 'S';
	}
}

void		get_symbol_letter(t_sym *sym, t_list *sects)
{
	sym->type_s = '?';
    if (N_STAB & sym->type)
		sym->type_s = '-';
	else if ((N_TYPE & sym->type) == N_UNDF && sym->value != 0 && (sym->type & N_EXT))
		sym->type_s = 'C';
	else if ((N_TYPE & sym->type) == N_UNDF && (sym->type & N_EXT))
        sym->type_s = 'U';
    else if ((N_TYPE & sym->type) == N_ABS)
		sym->type_s = 'A';
	else if ((N_TYPE & sym->type) == N_SECT)
		match_symbol_section(sects, sym);
	else if ((N_TYPE & sym->type) == N_INDR)
		sym->type_s = 'I';
	if (!(N_EXT & sym->type))
		sym->type_s = ft_tolower(sym->type_s);
}