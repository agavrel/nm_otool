/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:48 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/23 23:48:43 by agrumbac         ###   ########.fr       */
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
		"  -> "
	};

	ft_putstr_fd(msg[err], 2);
	ft_putendl_fd(str, 2);
	return (0);
}
