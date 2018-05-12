/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_power_of_x.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/27 07:08:53 by angavrel          #+#    #+#             */
/*   Updated: 2017/02/12 12:07:23 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_is_power_of_x(unsigned n, unsigned x)
{
	if (n == 0)
		return (0);
	while (!(n % x))
		n /= x;
	return ((n == 1) ? 1 : 0);
}
