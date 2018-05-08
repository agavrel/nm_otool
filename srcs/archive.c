/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/08 22:06:13 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/08 22:25:06 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "nm_otool.h"
# include "archive.h"

static uint32_t	ft_endian_4(uint32_t n)
{
	return ((n >> 24) | ((n & 0xff0000) >> 8) \
	| ((n & 0xff00) << 8) | (n << 24));
}

/*
** convert hexed ascii from header->size to its decimal value
** 3231303820202020 ->
*/

static long		get_size(uint32_t n[2])
{
	char	size[16];

	ft_memcpy(size, n, 16);
	return (ft_atol(size));
}

/*
** for unsorted archive
*/

static bool		loop_archive_unsort(t_gatherer func_ptr, const char *filename, \
							t_archive *header, t_archive_symtab	*symtab_array)
{
	t_archive_symtab				*symtab;
	uint32_t						i;
	uint32_t						offset;

	offset = 0;
	while (offset < header->symbol_tab_size)
	{
		symtab = safe(sizeof(t_archive) - sizeof(uint32_t) \
						+ offset, sizeof(*symtab));
		symtab_array[offset / sizeof(*symtab)] = *symtab;
		offset += sizeof(t_archive_symtab);
	}
}

/*
** for sorted archive, which is the default case
*/

static bool		loop_archive_sort(t_gatherer func_ptr, const char *filename, \
							t_archive *header, t_archive_symtab	*symtab_array)
{
	uint32_t						offset;
	uint32_t						i;
	t_archive_symtab				*symtab;

	offset = 0;
	while (offset < header->symbol_tab_size)
	{
		symtab = safe(sizeof(t_archive) - sizeof(uint32_t) \
						+ offset, sizeof(*symtab));
		i = 0;
		while (symtab->sym_offset > symtab_array[i].sym_offset)
			++i;
		ft_memmove(&symtab_array[i + 1], &symtab_array[i], \
					offset - i * sizeof(t_archive_symtab));
		symtab_array[i] = *symtab;
		offset += sizeof(t_archive_symtab);
	}
}

/*
** archive manager: calls a t_gatherer for each object found
*/

bool			manage_archive(t_gatherer func_ptr, const char *filename)
{
	// TODO set endian for every object in the archive
	t_archive						*header;
	t_archive_symtab				*symtab_array;
	uint32_t						i;
	t_object_header					*obj;

	static const t_loop_archive		loop_archive[2] =
	{
		&loop_archive_unsort,
		&loop_archive_sort

	};
	endian_little_mode(BOOL_TRUE);
	if (!(header = safe(0, sizeof(*header))))
		return (errors(ERR_FILE, "missing archive header"));
	if (ARCHIVE_MAGIC != header->magic)
		return (errors(ERR_THROW, "in _manage_archive"));
	if (!(symtab_array = malloc(header->symbol_tab_size)))
		return (errors(ERR_SYS, "malloc failed"));
	ft_memset(symtab_array, -1, header->symbol_tab_size);
	loop_archive[!ft_strcmp(header->long_name, "__.SYMDEF SORTED")] \
		(func_ptr, filename, header, symtab_array);
	i = 0;
	while (i < header->symbol_tab_size / sizeof(t_archive_symtab))
	{
		obj = safe(symtab_array[i].obj_offset, sizeof(*obj));
		ft_printf("%s(%.20s):\n", filename, obj->long_name);
		ft_printf("%lx %lx\n", symtab_array[i].sym_offset, symtab_array[i].obj_offset);//
		uint32_t myOffset = symtab_array[i].obj_offset + sizeof(*obj);
		set_start_offset(myOffset);
		ft_printf("%lx\n", myOffset);
		endian_little_mode(BOOL_FALSE); //recup mach 64 header FEEFACE -> see fat
		func_ptr(BOOL_TRUE); // -> recup magic 64 or not ?
		set_start_offset(0);

		getchar();//
		++i;
	}
	return (BOOL_TRUE);
}
