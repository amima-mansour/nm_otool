#include "ft_nm.h"

static bool     same_arch(t_file *file, struct fat_arch_64 *arch, uint32_t nfat)
{
    uint32_t i;
    uint64_t offset;
    uint64_t size;

    i = -1;
	while (++i < nfat)
	{
        if (!arch || swap32(file->swap_bits, arch->align) > 15)
        {
            errors(file->filename, CORRUPT_FILE);
            return (true);
        }
		offset = swap64(file->swap_bits, arch->offset);
        size = swap64(file->swap_bits, arch->size);
		if (swap32(file->swap_bits, arch->cputype) == CPU_TYPE_X86_64)
		{
            if (!iscorrup(file, file->ptr + offset, size))
            {
                errors(file->filename, CORRUPT_FILE);
                return (true);
            }
			nm(file->ptr + offset, size, file->filename, NULL);
			return (true);
		}
        arch = (struct fat_arch_64 *)iscorrup(file, arch + 1, sizeof(*arch));
	}
    return (false);
}

static void     all_arch(t_file *file, struct fat_arch_64 *arch, uint32_t nfat)
{
    uint32_t        i;
    uint64_t        offset;
    uint64_t        size;
    
    i = -1;
	while (++i < nfat)
	{
        if (!arch)
        {
            errors(file->filename, CORRUPT_FILE);
            return ;
        }
        offset = swap64(file->swap_bits, arch->offset);
        size = swap64(file->swap_bits, arch->size);
        if (!iscorrup(file, file->ptr + offset, size))
        {
            errors(file->filename, CORRUPT_FILE);
            return ;
        }
		ft_printf("\n%s (for architecture %s):\n", file->filename, get_archi_name(swap32(file->swap_bits, arch->cputype), swap32(file->swap_bits, arch->cpusubtype)));
		nm(file->ptr + offset, size, file->filename, NULL);
        arch = (struct fat_arch_64 *)iscorrup(file, arch + 1, sizeof(*arch));
	}
}
/**
 * il manque les erreurs pour un fat
 */
bool			handle_fat_64(t_file *file)
{
	uint32_t			    nfat;
	struct fat_header	    *header;
	struct fat_arch_64		*arch;

	header = (struct fat_header *)iscorrup(file, (void*)file->ptr, sizeof(*header));
    if (!header)
        return (errors(file->filename, CORRUPT_FILE));
	nfat = swap32(file->swap_bits, header->nfat_arch);
	arch = (struct fat_arch_64 *)iscorrup(file, (void*)header + sizeof(*header), sizeof(*arch));
    if (same_arch(file, arch, nfat))
        return (true);
    all_arch(file, arch, nfat);
    return (true);
}
