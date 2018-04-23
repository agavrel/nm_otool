/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 22:29:58 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/24 00:01:06 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static bool		manage_fat(const int magic)
{
	return (errors(ERR_FILE, "you're fat"));//TODO actually manage smth
}

static bool		manage_fat_64(const int magic)
{
	return (errors(ERR_FILE, "you're fat"));//TODO actually manage smth
}

static bool		manage_archive(const int magic)
{
	return (errors(ERR_FILE, "you're arch"));//TODO actually manage smth
}

bool			extract_macho(const char *filename, bool (*gatherer)(void))
{
	int			*magic;

	if (!read_file(filename))
		return (errors(ERR_THROW, "in _read_file"));

	magic = safe(0, sizeof(uint32_t));

	if (*magic == MH_MAGIC || *magic == MH_CIGAM || \
		*magic == MH_MAGIC_64 || *magic == MH_CIGAM_64)
		return (gatherer());
	if (*magic == FAT_MAGIC || *magic == FAT_CIGAM)
		return (manage_fat(*magic));
	if (*magic == FAT_MAGIC_64 || *magic == FAT_CIGAM_64)
		return (manage_fat_64(*magic));
	if (*magic == ARCHIVE_MAGIC)
		return (manage_archive(*magic));

	return (errors(ERR_FILE, "unknown file format"));
}
