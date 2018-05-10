/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/10 02:08:44 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/10 02:22:30 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static inline bool	*singleton(void)
{
	static bool	little_endian = 0;

	return (&little_endian);
}

void				endian_little_mode(bool is_little_endian)
{
	bool			*endian;

	endian = singleton();
	*endian = is_little_endian;
}

uint32_t			endian_4(uint32_t n)
{
	if (*singleton())
		return ((n >> 24) | ((n & 0xff0000) >> 8) \
				| ((n & 0xff00) << 8) | (n << 24));
	return (n);
}

uint64_t			endian_8(uint64_t n)
{
	if (*singleton())
	return ((n >> 56) | (n << 56) \
			| (n & 0x00ff000000000000) >> 40 \
			| (n & 0x0000ff0000000000) >> 24 \
			| (n & 0x000000ff00000000) >> 8 \
			| (n & 0x00000000ff000000) << 8 \
			| (n & 0x0000000000ff0000) << 24 \
			| (n & 0x000000000000ff00) << 40);
	return (n);
}
