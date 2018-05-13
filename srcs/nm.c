/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/10 02:02:18 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/13 21:29:23 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm_display.h"

/*
** t_gatherer nm_gatherer
*/

static bool		nm_gatherer(const bool is_64)
{
	static const uint32_t		lc_seg[2] = {LC_SEGMENT, LC_SEGMENT_64};
	static const t_lc_manager	segment_manager[2] = {
		&nm_manage_segment_32,
		&nm_manage_segment_64
	};
	static const t_lc_manager	symtab_manager[2] = {
		&nm_manage_symtab_32,
		&nm_manage_symtab_64
	};

	nm_sections_character_table(0);
	if (!(iterate_lc(is_64, lc_seg[is_64], segment_manager[is_64])))
		return (errors(ERR_THROW, __func__));
	if (!(iterate_lc(is_64, LC_SYMTAB, symtab_manager[is_64])))
		return (errors(ERR_THROW, __func__));
	return (true);
}

/*
** Flags are toggled (on/off) for files in the order they are given:
**   ./ft_nm -p ft_ls -r -p -a ft_otool
** would apply -p for ft_ls, and only use -r and -a for ft_otool
** the flags are disabled on the second use (toggled on then off)
*/

int				main(int ac, char **av)
{
	if (ac < 2)
	{
		extract_macho(DEFAULT_TARGET, &nm_gatherer);
	}
	while (*++av)
	{
		if (**av == '-')
		{
			if (!nm_set_flag(*av))
				return (EXIT_FAILURE);
		}
		else
		{
			extract_macho(*av, &nm_gatherer);
		}
	}
	return (EXIT_SUCCESS);
}
