/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ranlib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 22:41:33 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/06 00:30:21 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** size of ranlib header is 80 bytes
** little endian applies to size_t and uint32_t
*/

typedef struct	s_ranlib
{
	size_t		magic;
	char		name[16];
	uint32_t	timestamp[3];
	char		userid[6];
	char		groupid[6];
	uint32_t	mode[2];
	uint32_t	size[2];
	uint32_t	end_header;
	char		long_name[20];
	uint32_t	symbol_tab_size;
}				t_ranlib;
