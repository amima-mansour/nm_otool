/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amansour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 15:53:10 by amansour          #+#    #+#             */
/*   Updated: 2019/09/02 16:09:46 by amansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

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
# define OPEN_ERROR		"Open file"
# define FSTAT_ERROR	"Fstat Error"

typedef struct			s_sym
{
	char				*name;
	uint32_t			name_size;
	uint64_t			value;
	uint8_t				type;
	uint8_t				sect;
	uint8_t				type_s;
	uint16_t			desc;
}						t_sym;

typedef enum			e_arch
{
	ARCH_32,
	ARCH_64
}						t_arch;

typedef struct			s_sect_64
{
	uint64_t			addr;
	uint64_t			size;
	uint32_t			offset;
}						t_sect_64;

typedef struct			s_sect_32
{
	uint32_t			addr;
	uint32_t			size;
	uint32_t			offset;
}						t_sect_32;

typedef struct			s_file
{
	char				*filename;
	void				*ptr;
	bool				isfat;
	int32_t				ncmds;
	uint32_t			size;
	bool				swap_bits;
	t_arch				arch;
	uint64_t			nsects;
	cpu_type_t      	cpu; 
	t_sect_32			sect_32;
	t_sect_64			sect_64;
}						t_file;

int						main(int argc, char **av);
bool					otool(void *ptr, uint64_t size, char *name, char *arch);

bool					handle_mach_o(t_file *file);
bool					handle_archive(t_file *file);
bool					handle_fat_32(t_file *file);
bool					handle_fat_64(t_file *file);
bool					manage_sections_32(t_file *file, void *lc);
bool					manage_sections_64(t_file *file, void *lc);

bool					output(t_file *file, bool is64);

uint32_t				swap32(bool value, uint32_t n);
uint64_t				swap64(bool value, uint64_t n);
bool					is_swap(uint32_t magic);
void					*iscorrup(t_file *file, void *ptr, uint64_t size);
bool					errors(char *name, char *msg);

char					*get_arch_name(cpu_type_t cpu_type, \
						cpu_subtype_t cpu_subtype);
#endif