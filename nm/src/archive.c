#include "ft_nm.h"

static uint32_t	extract_padding(char *ar_name)
{
	uint32_t	ar_str_size;

	if ((ft_strncmp(ar_name, AR_EFMT1, ft_strlen(AR_EFMT1))) == 0)
		ar_str_size = ft_atoi(ar_name + ft_strlen(AR_EFMT1));
	else
		ar_str_size = 0;
	return (ar_str_size);
}

static bool list_all(t_file *file, struct ranlib *ran, uint32_t ran_size, uint32_t table_size)
{
	struct ar_hdr		*obj_header;
	char				*obj_name;
	uint32_t			obj_size;

	obj_header = iscorrup(file, (void*)ran + ran_size + sizeof(uint32_t) + table_size, sizeof(*obj_header));
	if (!obj_header)
		return (errors(file->filename, CORRUPT_FILE));
	while (obj_header)
	{
		obj_name = (char *)iscorrup(file, obj_header + 1, sizeof(*obj_name));
		if (!obj_name)
			return (errors(file->filename, CORRUPT_FILE));
		obj_size = ft_atoi(obj_header->ar_size);
		if (iscorrup(file, (void *)obj_name, obj_size) == NULL)
			return (errors(file->filename, CORRUPT_FILE));
		if (nm((void *)obj_name + extract_padding(obj_header->ar_name), obj_size, obj_name, file->filename))
			return (true);
		obj_header = iscorrup(file, (void *)obj_header + sizeof(*obj_header) + obj_size, sizeof(*obj_header));
	}
	return (true);
}

bool	handle_archive(t_file *file)
{
	struct ar_hdr	*hd;
	struct ranlib	*ran;
	uint32_t        ran_size;
	uint32_t        table_size;

	if (!(hd = (struct ar_hdr*)iscorrup(file, file->ptr + SARMAG, sizeof(*hd))))
		return (errors(file->filename, CORRUPT_FILE));
	ran_size = *(uint32_t*)((void*)(hd + 1) + extract_padding(hd->ar_name));
	if (iscorrup(file, hd + 1, sizeof(ran_size)) == NULL)
		return (errors(file->filename, CORRUPT_FILE));
	ran = (struct ranlib*)iscorrup(file, (void*)(hd + 1) + extract_padding(hd->ar_name) + sizeof(uint32_t), sizeof(*ran));
	if (ran == NULL || iscorrup(file, (void*)ran + ran_size, sizeof(table_size)) == NULL)
	{
		if (!iscorrup(file, (void*)ran + ran_size, sizeof(table_size)))
			return (errors(file->filename, CORRUPT_FILE));
	}
	table_size = *(uint32_t*)((void*)ran + ran_size);
	list_all(file, ran, ran_size, table_size);
	return (false);
}
