/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/12 17:42:16 by angavrel          #+#    #+#             */
/*   Updated: 2018/05/12 19:22:01 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool		ft_isupper(int c)
{
	return (c >= 0x41 && c <= 0x5a);
}