/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 22:41:33 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/06 19:41:48 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** size of archive header is 80 bytes
** little endian applies to size_t and uint32_t
*/

typedef struct	s_archive
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
	uint32_t	begin;
}				t_archive;

typedef struct	s_object_header
{
	char		name[16];
	uint32_t	timestamp[3];
	char		userid[6];
	char		groupid[6];
	uint32_t	mode[2];
	uint32_t	size[2];
	uint32_t	end_header;
	char		long_name[20];
}				t_object_header;

typedef struct	s_archive_symtab
{
	uint32_t	sym_offset;
	uint32_t	obj_offset;
}				t_archive_symtab;
