/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ranlib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 22:41:33 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/03 23:52:48 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANLIB_H
# define RANLIB_H

typedef struct	s_rablib
{
	size_t		magic;
	char		name[16];
	uint32_t	timestamp[3];
	char		userid:6;
	char		groupid:6;
	size_t		mode;
	size_t		size;
	uint32_t	endheader;
	uint32_t	longname[5];
}				t_ranlib;

#endif
