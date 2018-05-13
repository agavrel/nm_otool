/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/10 02:09:18 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/13 21:25:48 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** t_gatherer otool_gatherer
*/

static bool		otool_gatherer(const bool is_64)
{
	static const uint32_t		lc[2] = {LC_SEGMENT, LC_SEGMENT_64};
	static const t_lc_manager	manager[2] = {
		&manage_segment,
		&manage_segment_64
	};

	return (iterate_lc(is_64, lc[is_64], manager[is_64]));
}

int				main(int ac, char **av)
{
	if (ac < 2)
		extract_macho(DEFAULT_TARGET, &otool_gatherer);
	else
	{
		while (*++av)
			extract_macho(*av, &otool_gatherer);
	}
	return (0);
}
