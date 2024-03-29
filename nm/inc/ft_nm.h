/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:05:38 by amansour          #+#    #+#             */
/*   Updated: 2019/09/07 15:07:32 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <stdlib.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <ar.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdbool.h>
# include "../../libft/inc/libft.h"

# define DEFAULT_FILE	"a.out"
# define BAD_STRING		"bad string index"
# define CORRUPT_FILE	"truncated or malformed object"
# define NOT_VALID		"The file was not recognized as a valid object file"
# define MAP_ERROR		"Map error"
# define MUN_ERROR		"Munmap error"
# define CLOSE_ERROR	"Close error"
# define DIR			"Is a directory"
# define OPEN_ERROR		"No such file or directory"
# define FSTAT_ERROR	"Fstat Error"
# define OPTIONS		"prunU"
# define P_FLAG			1
# define N_FLAG			2
# define R_FLAG			4
# define U_FLAG			8
# define BIGU_FLAG		16

typedef struct			s_sym
{
	char				*name;
	uint32_t			strname;
	uint64_t			value;
	uint8_t				type;
	uint8_t				sect;
	uint8_t				type_s;
	uint32_t			index;
}						t_sym;

typedef enum			e_arch
{
	ARCH_32,
	ARCH_64
}						t_arch;

typedef struct			s_sect
{
	char				*name;
	uint64_t			index;
	uint64_t			addr;
	uint64_t			size;
	uint32_t			offset;
}						t_sect;

typedef struct			s_file
{
	char				*filename;
	void				*ptr;
	int32_t				ncmds;
	uint32_t			size;
	bool				swap_bits;
	t_arch				arch;
	uint64_t			nsects;
	t_list				*sects;
	t_list				*syms;
	uint32_t			stroff;
}						t_file;

bool					g_multi_file;
int						g_flags;

int						main(int argc, char **av);
bool					nm(void *ptr, uint64_t size, char *name, char *arch);

int						get_flags(int *argc, char ***argv);
int						usage(void);
void					sort(t_list  **data);
void					alpha_sort(t_list  **data, bool rev);
void					num_sort(t_list  **data, bool rev);
void					remove_lst(t_list **syms, t_list *lst);

bool					handle_mach_o(t_file *file);
bool					handle_archive(t_file *file);
bool					handle_fat_32(t_file *file);
bool					handle_fat_64(t_file *file);
bool					manage_sections_32(t_file *file, void *lc);
bool					manage_sections_64(t_file *file, void *lc);

bool					output_32(struct symtab_command *sym, t_file *f, \
						uint32_t nsyms);
bool					output_64(struct symtab_command *sym, t_file *f, \
						uint32_t nsyms);

void					print_nm(t_file *f, bool is64);

uint32_t				swap32(bool value, uint32_t n);
uint64_t				swap64(bool value, uint64_t n);
bool					is_swap(uint32_t magic);
void					*iscorrup(t_file *file, void *ptr, uint64_t size);
bool					errors(char *name, char *msg);

char					*get_arch_name(cpu_type_t cpu_type, \
						cpu_subtype_t cpu_subtype);
void					get_symbol_letter(t_list *sym, t_list *sects);
void					free_list(t_list *lst);
void					add_list(t_list **l, t_list *lst);
#endif
