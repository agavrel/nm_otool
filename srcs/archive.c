/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/08 22:06:13 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/12 17:22:10 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "archive.h"

/*
** for unsorted archive
*/

static bool		loop_archive_unsorted(t_archive *header, \
									t_archive_symtab *symtab_array)
{
	t_archive_symtab				*symtab;
	uint32_t						offset;

	offset = 0;
	while (offset < header->symbol_tab_size)
	{
		if (!(symtab = safe(sizeof(t_archive) - sizeof(uint32_t) \
						+ offset, sizeof(*symtab))))
			return (errors(ERR_FILE, "bad symtab offset"));
		symtab_array[offset / sizeof(*symtab)] = *symtab;
		offset += sizeof(t_archive_symtab);
	}
	return (true);
}

/*
** for sorted archive, which is the default case
*/

static bool		loop_archive_sorted(t_archive *header, \
									t_archive_symtab *symtab_array)
{
	uint32_t						offset;
	uint32_t						i;
	t_archive_symtab				*symtab;

	offset = 0;
	while (offset < header->symbol_tab_size)
	{
		if (!(symtab = safe(sizeof(t_archive) - sizeof(uint32_t) \
						+ offset, sizeof(*symtab))))
			return (errors(ERR_FILE, "bad symtab offset"));
		i = 0;
		while (symtab->sym_offset > symtab_array[i].sym_offset)
			++i;
		ft_memmove(&symtab_array[i + 1], &symtab_array[i], \
					offset - i * sizeof(t_archive_symtab));
		symtab_array[i] = *symtab;
		offset += sizeof(t_archive_symtab);
	}
	return (true);
}

static bool		parse_object_header(t_gatherer func_ptr, uint32_t offset, \
											const char *filename)
{
	t_object_header					*obj_header;
	uint32_t						*magic;

	if (!(obj_header = safe(offset, sizeof(*obj_header))))
		return (errors(ERR_FILE, "bad object header offset"));
	offset += sizeof(*obj_header);
	ft_printf("%s(%.20s):\n", filename, obj_header->long_name);
	if (!(magic = safe(offset, sizeof(uint32_t))))
		return (errors(ERR_FILE, "bad magic ptr offset"));
	endian_little_mode(*magic == MH_CIGAM_64 || *magic == MH_CIGAM);
	set_start_offset(offset);
	func_ptr(*magic == MH_MAGIC_64 || *magic == MH_CIGAM_64);
	set_start_offset(0);
	if (!(magic = safe(0, sizeof(uint32_t))))
		return (errors(ERR_FILE, "bad magic ptr offset"));
	endian_little_mode(*magic == ARCHIVE_CIGAM);
	return (true);
}

/*
** archive manager: calls a t_gatherer for each object found
*/

bool			manage_archive(t_gatherer func_ptr, const char *file)
{
	t_archive						*header;
	t_archive_symtab				*symtab_arr;
	uint32_t						i;
	static const t_loop_archive		loop_archive[2] =
	{
		&loop_archive_unsorted,
		&loop_archive_sorted
	};

	if (!(header = safe(0, sizeof(*header))))
		return (errors(ERR_FILE, "missing archive header"));
	if (!(symtab_arr = malloc(header->symbol_tab_size)))
		return (errors(ERR_SYS, "malloc failed"));
	ft_memset(symtab_arr, -1, header->symbol_tab_size);
	if (!loop_archive[!ft_strcmp(header->long_name, \
		"__.SYMDEF SORTED")](header, symtab_arr))
		return (errors(ERR_THROW, __func__));
	i = 0;
	while (i < header->symbol_tab_size / sizeof(t_archive_symtab))
		if (!(parse_object_header(func_ptr, symtab_arr[i++].obj_offset, file)))
			return (errors(ERR_THROW, __func__));
	free(symtab_arr);
	return (true);
}
