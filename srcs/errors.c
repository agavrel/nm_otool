/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/10 02:08:55 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/10 02:22:31 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

bool				errors(const int err, const char *str)
{
	static const char	*msg[ERR_NUMBER] =
	{
		"Error: ",
		"Bad usage: ",
		"Bad file: ",
		"  -> in _"
	};

	ft_putstr_fd(msg[err], 2);
	ft_putendl_fd(str, 2);
	return (BOOL_FALSE);
}
