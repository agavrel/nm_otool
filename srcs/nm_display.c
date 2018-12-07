/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_display.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/12 17:28:45 by angavrel          #+#    #+#             */
/*   Updated: 2018/12/07 23:12:14 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm_display.h"

/*
** ft_nm has 8 available flags
** "-a" -- Display  all symbol table entries
** "-g" -- Display only global (external) symbols
** "-p" -- Don't sort; display in symbol-table order
** "-u" -- Display only undefined symbols
** "-U" -- Don't display undefined symbols
** "-j" -- Just display the symbol names (no value or type)
** "-r" -- Sort in reverse order
** "-n" -- Sort numerically rather than alphabetically
*/

static uint8_t			flags = 0;

bool					nm_set_flag(const char *av)
{
	uint8_t				i;
	const char flags_tab[8] = {'a', 'g', 'p', 'u', 'U', 'j', 'r', 'n'};

	i = 0;
	if (av[0] == '-')
	{
		while (i < 8)
		{
			if (av[1] == flags_tab[i])
				flags ^= 1 << i;
			return (true);
		}
	}
	return (errors(ERR_USAGE, av));
}

bool					nm_symbol_allocate(t_sym_sort *sorted_symbols, \
							const uint64_t nsyms)
{
	ft_bzero(sorted_symbols, sizeof(*sorted_symbols));
	if (nsyms)
	{
		sorted_symbols->symbols = ft_memalloc(nsyms * sizeof(t_symbol));
		if (!sorted_symbols->symbols)
			return (errors(ERR_SYS, "malloc failed"));
		sorted_symbols->symbols_sort = ft_memalloc(nsyms * sizeof(void *));
		if (!sorted_symbols->symbols_sort)
			return (errors(ERR_SYS, "malloc failed"));
	}
	return (true);
}
/*
** ft_nm has 8 available flags
** "-a" -- Display  all symbol table entries
** "-g" -- Display only global (external) symbols
** "-p" -- Don't sort; display in symbol-table order
** "-u" -- Display only undefined symbols
** "-U" -- Don't display undefined symbols
** "-j" -- Just display the symbol names (no value or type)
** "-r" -- Sort in reverse order
** "-n" -- Sort numerically rather than alphabetically
**
** 8 bools stored in the 8 bits of:
**   uint8_t flags;
** Usage: Boolean check
**   if (NM_FLAG_P(flags));
** Usage: Toggle
**   flags ^= NM_FLAG_P(NM_TOGGLE);
*/

#define NM_FLAG_A(x)	(x & 0b1)
#define NM_FLAG_G(x)		(x & 0b10)
#define NM_FLAG_P(x)		(x & 0b100)
#define NM_FLAG_U(x)		(x & 0b1000)
#define NM_FLAG_UU(x)		(x & 0b10000)
#define NM_FLAG_J(x)		(x & 0b100000)
#define NM_FLAG_R(x)		(x & 0b1000000)
#define NM_FLAG_N(x)		(x & 0b10000000)

#define NM_TOGGLE			0b11111111
void					nm_store_value(t_sym_sort *sorted_symbols, \
							const t_symbol *new_symbol)
{
	if ((NM_FLAG_A(flags) || new_symbol->type != '-') && \
		!(NM_FLAG_G(flags) && !ft_isupper(new_symbol->type)) && \
		!(NM_FLAG_U(flags) && \
		(new_symbol->type != 'u' && new_symbol->type != 'U')) && \
		!(NM_FLAG_UU(flags) && \
		(new_symbol->type == 'u' || new_symbol->type == 'U')))
	{
		sorted_symbols->symbols[sorted_symbols->nsyms_sort] = *new_symbol;
		sorted_symbols->symbols_sort[sorted_symbols->nsyms_sort] = \
			&sorted_symbols->symbols[sorted_symbols->nsyms_sort];
		sorted_symbols->nsyms_sort++;
	}
}

void					nm_sort_print_free(t_sym_sort *sorted_symbols, \
							const int padding)
{
	t_symbol			*curr;
	size_t				i;

	if (!NM_FLAG_P(flags))
		nm_selection_sort(sorted_symbols, \
			(!!NM_FLAG_R(flags) + (!!NM_FLAG_N(flags)) * 2));
	i = 0;
	while (i < sorted_symbols->nsyms_sort)
	{
		curr = sorted_symbols->symbols_sort[i];
		if (NM_FLAG_J(flags) || NM_FLAG_U(flags))
			ft_printf("%s\n", curr->string);
		else if (curr->offset || !(curr->type == 'u' || curr->type == 'U'))
			ft_printf("%0*lx %c %.*s\n", padding, curr->offset, curr->type, \
				curr->str_max_size, curr->string);
		else
			ft_printf("  %*c %.*s\n", padding, curr->type, \
				curr->str_max_size, curr->string);
		i++;
	}
	free(sorted_symbols->symbols);
	free(sorted_symbols->symbols_sort);
}
