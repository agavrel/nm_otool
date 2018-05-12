/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/08 22:06:13 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/13 19:13:47 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "archive.h"

//TODO endian management
/*
** for unsorted archive - sizeof(uint32_t) is the restricted variable of struct
*/

static bool     loop_archive_unsorted(t_archive_header *header, \
                    t_archive_symtab *symtab_array)
{
	const uint32_t	base_offset = sizeof(t_archive_header)
		- sizeof(uint32_t);

    if (!(symtab_array = safe(base_offset, header->symbol_tab_size
			* sizeof(t_archive_symtab))))
        return (errors(ERR_FILE, "bad archive symtab offset"));
    return (true);
}

/*
** for sorted archive, which is the default case
*/

static bool     loop_archive_sorted(t_archive_header *header, \
                    t_archive_symtab *symtab_array)
{
	uint32_t			i;
	t_archive_symtab	tmp;
	void				*src;
	const uint32_t		base_offset = sizeof(t_archive_header)
		- sizeof(uint32_t);

	if (!(src = safe(base_offset, header->symbol_tab_size)))
		return (errors(ERR_FILE, "bad archive symtab offset"));
    ft_memcpy(symtab_array, src, header->symbol_tab_size);
	i = 0;
	while (i * 8 < header->symbol_tab_size - 8)
	{
		if (symtab_array[i].sym_offset > symtab_array[i + 1].sym_offset)
		{
			tmp = symtab_array[i];
			symtab_array[i] = symtab_array[i + 1];
			symtab_array[i + 1] = tmp;
			i = 0;
		}
		else
			++i;
	}
    return (true);
}

static bool     parse_object_header(t_gatherer func_ptr, uint32_t offset, \
                    const char *filename, bool is_little_endian)
{
    t_object_header     *obj_header;
    uint32_t            *magic;

    if (!(obj_header = safe(offset, sizeof(*obj_header))))
        return (errors(ERR_FILE, "bad object header offset"));
    offset += sizeof(*obj_header);
    ft_printf("%s(%.20s):\n", filename, obj_header->long_name);
    if (!(magic = safe(offset, sizeof(*magic))))
        return (errors(ERR_FILE, "bad object magic offset"));
    endian_little_mode(*magic == MH_CIGAM_64 || *magic == MH_CIGAM);
    set_start_offset(offset);
    func_ptr(*magic == MH_MAGIC_64 || *magic == MH_CIGAM_64);
    set_start_offset(0);
    endian_little_mode(is_little_endian);
    return (true);
}

/*
** archive manager: calls a t_gatherer for each object found
*/

bool            manage_archive(t_gatherer func_ptr, const char *filename)
{
    const t_loop_archive        loop_archive[2] =
    {
        &loop_archive_unsorted,
        &loop_archive_sorted
    };
    t_archive_header            *header;
    t_archive_symtab            *symtab_arr;
    uint32_t                    i;
	bool						no_error;

	ft_printf("Archive : %s\n", filename);
    if (!(header = safe(0, sizeof(*header))))
        return (errors(ERR_FILE, "missing archive header"));
    if (!(symtab_arr = malloc(header->symbol_tab_size)))
        return (errors(ERR_SYS, "malloc failed"));
    no_error = loop_archive[!ft_strncmp(header->long_name, \
        "__.SYMDEF SORTED", sizeof(header->long_name))](header, symtab_arr);
    i = 0;
    while (no_error && i < header->symbol_tab_size / sizeof(t_archive_symtab))
        no_error = parse_object_header(func_ptr, symtab_arr[i++].obj_offset, \
            filename, (header->magic == ARCHIVE_CIGAM));
    free(symtab_arr);
    return (no_error || errors(ERR_THROW, __func__));
}
