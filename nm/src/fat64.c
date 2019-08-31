#include "ft_nm.h"

static bool     same_arch(t_file *file, struct fat_arch_64 *arch, uint32_t nfat, bool *err)
{
    uint32_t i;
    uint64_t offset;
    uint64_t size;

    i = -1;
	while (++i < nfat)
	{
        if (!arch || swap32(file->swap_bits, arch->align) > 15)
        {
            *err = errors(file->filename, CORRUPT_FILE);
            return (true);
        }
		offset = swap64(file->swap_bits, arch->offset);
        size = swap64(file->swap_bits, arch->size);
		if (swap32(file->swap_bits, arch->cputype) == CPU_TYPE_X86_64)
		{
            if (!iscorrup(file, file->ptr + offset, size))
            {
                *err = errors(file->filename, CORRUPT_FILE);
                return (true);
            }
			*err = nm(file->ptr + offset, size, file->filename, NULL);
			return (true);
		}
        arch = (struct fat_arch_64 *)iscorrup(file, arch + 1, sizeof(*arch));
	}
    return (false);
}

static bool     all_arch(t_file *file, struct fat_arch_64 *arch, uint32_t nfat)
{
    uint32_t        i;
    uint64_t        offset;
    uint64_t        size;
    
    i = -1;
	while (++i < nfat)
	{
        if (!arch)
            return(errors(file->filename, CORRUPT_FILE));
        offset = swap64(file->swap_bits, arch->offset);
        size = swap64(file->swap_bits, arch->size);
        if (!iscorrup(file, file->ptr + offset, size))
            return(errors(file->filename, CORRUPT_FILE));
		ft_printf("\n%s (for architecture %s):\n", file->filename, get_arch_name(swap32(file->swap_bits, arch->cputype), swap32(file->swap_bits, arch->cpusubtype)));
		if (nm(file->ptr + offset, size, file->filename, NULL))
            return (true);
        arch = (struct fat_arch_64 *)iscorrup(file, arch + 1, sizeof(*arch));
	}
    return (false);
}

bool			handle_fat_64(t_file *file)
{
	uint32_t			    nfat;
	struct fat_header	    *header;
	struct fat_arch_64		*arch;
    bool                    err;

	header = (struct fat_header *)iscorrup(file, (void*)file->ptr, sizeof(*header));
    if (!header)
        return (errors(file->filename, CORRUPT_FILE));
	nfat = swap32(file->swap_bits, header->nfat_arch);
	arch = (struct fat_arch_64 *)iscorrup(file, (void*)header + sizeof(*header), sizeof(*arch));
    err = false;
    if (same_arch(file, arch, nfat, &err))
        return (err);
    err = all_arch(file, arch, nfat);
    return (err);
}
