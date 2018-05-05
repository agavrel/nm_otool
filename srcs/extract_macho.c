/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_macho.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/29 21:37:12 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/06 01:04:59 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "ranlib.h"

static bool		known_magic_retriever_64(uint32_t nfat_arch, size_t offset, \
					bool *is_little_endian, uint64_t *target_offset)
{
	struct fat_arch_64		*arch;
	uint32_t				*magic;

	// loop through architectures looking for known magic
	while (nfat_arch--)
	{
		//retrieve safe pointers
		if (!(arch = safe(offset, sizeof(*arch))))
			return (errors(ERR_FILE, "bad fat arch offset"));
		if (!(magic = safe(endian_8(arch->offset), sizeof(*magic))))
			return (errors(ERR_FILE, "bad fat arch magic offset"));

		// check for known magic
		if ((*is_little_endian = !(*magic == MH_MAGIC_64)) || *magic == MH_CIGAM_64)
			*target_offset = endian_8(arch->offset);
		else if (!target_offset && \
			((*is_little_endian = !(*magic == MH_MAGIC)) || *magic == MH_CIGAM))
			*target_offset = endian_8(arch->offset);
		offset += endian_8(arch->size);
	}
	return (BOOL_TRUE);
}

static bool		known_magic_retriever_32(uint32_t nfat_arch, size_t offset, \
					bool *is_little_endian, uint64_t *target_offset)
{
	struct fat_arch			*arch;
	uint32_t				*magic;

	// loop through architectures looking for known magic
	while (nfat_arch--)
	{
		//retrieve safe pointers
		if (!(arch = safe(offset, sizeof(*arch))))
			return (errors(ERR_FILE, "bad fat arch offset"));
		if (!(magic = safe(endian_4(arch->offset), sizeof(*magic))))
			return (errors(ERR_FILE, "bad fat arch magic offset"));

		// check for known magic
		if ((*is_little_endian = !(*magic == MH_MAGIC_64)) || *magic == MH_CIGAM_64)
			*target_offset = endian_4(arch->offset);
		else if (!target_offset && \
			((*is_little_endian = !(*magic == MH_MAGIC)) || *magic == MH_CIGAM))
			*target_offset = endian_4(arch->offset);
		offset += endian_4(arch->size);
	}
	return (BOOL_TRUE);
}

/*
** fat management, call a t_gatherer for the favorite known arch (_64 then _32)
*/

static bool		manage_fat(t_gatherer func_ptr, const bool is_64)
{
	static const t_fat_magic_retriever	find_the_magic[2] =
	{
		&known_magic_retriever_32,
		&known_magic_retriever_64
	};
	struct fat_header					*header;
	bool								is_little_endian;
	uint64_t							target_offset;

	//read header
	target_offset = 0;
	if (!(header = safe(0, sizeof(*header))))
		return (errors(ERR_FILE, "missing fat header"));

	//find the magic
	if (!find_the_magic[is_64](endian_4(header->nfat_arch), \
		sizeof(*header), &is_little_endian, &target_offset))
		return (errors(ERR_THROW, "in _manage_fat"));

	//do the mach-o parsing magic
	if (!target_offset)
		return (errors(ERR_FILE, "no known architectures found"));
	set_start_offset(target_offset);
	endian_little_mode(is_little_endian);
	return (func_ptr(BOOL_FALSE));
}

static uint32_t	ft_endian_4(uint32_t n)
{
	return ((n >> 24) | ((n & 0xff0000) >> 8) \
	| ((n & 0xff00) << 8) | (n << 24));
}

/*
** convert hexed ascii from header->size to its decimal value
** 3231303820202020 ->
*/

static uint32_t	get_size(uint32_t n[2])
{
	char	size[16];

	ft_memcpy(size, n, 16);
	return (ft_atol(size));
}

/*
** archive manager: calls a t_gatherer for each object found
*/

static bool		manage_ranlib(t_gatherer func_ptr)
{
	// TODO print smth like "\nlibft/libft.a(ft_printf_buf.o):\n"
	// TODO set endian for every object in the archive
	t_ranlib						*header;
	uint32_t						symbol_tab_size;
	size_t							size;

	endian_little_mode(BOOL_TRUE);
	// read header
	if (!(header = safe(0, sizeof(*header))))
		return (errors(ERR_FILE, "missing ranlib header"));
	if (ARCHIVE_MAGIC != header->magic)
		return (errors(ERR_THROW, "in _manage_ranlib"));

	/*

	ft_printf("header->magic %p\n", (void *) &header->magic - (void *)header);
	ft_printf("header->name %p\n", (void *) &header->name - (void *)header);
	ft_printf("header->timestamp %p\n", (void *) &header->timestamp - (void *)header);
	ft_printf("header->userid %p\n", (void *) &header->userid - (void *)header);
	ft_printf("header->groupid %p\n", (void *) &header->groupid - (void *)header);
	ft_printf("header->mode %p\n", (void *) &header->mode - (void *)header);
	ft_printf("header->size %p\n", (void *) &header->size - (void *)header);
	ft_printf("header->end_header %p\n", (void *) &header->end_header - (void *)header);
	ft_printf("header->long_name %p\n", (void *) &header->long_name - (void *)header);
	ft_printf("header->symtab_size %p\n", (void *) &header->symtab_size - (void *)header);
	ft_printf("---------------------------\n");*/
	size = get_size(header->size);
	ft_printf("size %u\n", size);
	symbol_tab_size = header->symbol_tab_size;
	ft_printf("header->symtabsize %u\n", symbol_tab_size);


	// iterate over objects while...
	while (size--)
	{
		if (!(header = safe(0, sizeof(*header))))
			return (errors(ERR_THROW, "missing ranlib header"));
	}
		// if known magic (read magic cf above) else continue
			// set_start_offset(magic offset); // set object start_offset
			// endian_little_mode(object is_little_endian); // set object endian
			// call func_ptr(is_64)
			// set_start_offset(0); // reset start_offset
			// endian_little_mode(base archive endian); // reset to archive's endian
			// if !func_ptr(is_64)
				// break;

	return (errors(ERR_FILE, "you're arch"));//TODO actually manage smth
}

/*
** extract_macho and call a t_gatherer
*/

bool			extract_macho(const char *filename, t_gatherer func_ptr)
{
	size_t		*magic;
	bool		return_value;

	//map file
	if (!read_file(filename))
		return (errors(ERR_THROW, "in _read_file"));
	if (!(magic = safe(0, sizeof(uint32_t))))
		return (errors(ERR_FILE, "missing magic"));

	//detect endian
	endian_little_mode(*magic == FAT_CIGAM || *magic == FAT_CIGAM_64 || \
		*magic == MH_CIGAM || *magic == MH_CIGAM_64);

	//check magic
	if (*magic == ARCHIVE_MAGIC)
		return_value = manage_ranlib(func_ptr);
	else if (*magic == MH_MAGIC || *magic == MH_CIGAM)
		return_value = func_ptr(BOOL_FALSE);
	else if (*magic == MH_MAGIC_64 || *magic == MH_CIGAM_64)
		return_value = func_ptr(BOOL_TRUE);
	else if (*magic == FAT_MAGIC || *magic == FAT_CIGAM)
		return_value = manage_fat(func_ptr, BOOL_FALSE);
	else if (*magic == FAT_MAGIC_64 || *magic == FAT_CIGAM_64)
		return_value = manage_fat(func_ptr, BOOL_TRUE);
	else
		return_value = errors(ERR_FILE, "unknown file format");

	//unmap file
	if (!free_file())
		return (errors(ERR_THROW, "in _free_file"));
	return (return_value);
}
