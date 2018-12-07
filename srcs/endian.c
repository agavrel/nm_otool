/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/10 02:08:44 by angavrel          #+#    #+#             */
/*   Updated: 2018/12/07 22:42:33 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static bool			little_endian = false;

void				endian_little_mode(bool is_little_endian)
{
	little_endian = is_little_endian;
}

uint16_t			endian_2(uint16_t n)
{
	if (little_endian)
		return (n >> 8 | n << 8);
	return (n);
}

uint32_t			endian_4(uint32_t n)
{
	if (little_endian)
		__asm__("mov  %eax, %edi;"
				"bswap %eax;");
	return (n);
}

uint64_t			endian_8(uint64_t n)
{
	if (little_endian)
		__asm__("mov  %rax, %rdi;"
				"bswap %rax;");
	return (n);
}
