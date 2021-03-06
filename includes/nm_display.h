/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_display.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/12 17:30:54 by angavrel          #+#    #+#             */
/*   Updated: 2018/12/07 23:08:29 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_DISPLAY_H
# define NM_DISPLAY_H

/*
** struct s_symbol:
**   offset: offset of the symbol
**   string: safe pointer to the string or NULL value if invalid
**   str_max_size: max size of the string
**   type: character for the type
*/

typedef struct	s_symbol
{
	uint64_t	offset;
	char		*string;
	uint32_t	str_max_size;
	char		type;
}				t_symbol;

/*
** struct	s_sym_sort:
**   symbols: symbols array (malloc'd)
**   symbols_sort: array of pointers to symbol array (sorted)
**   nsyms_sort: symbols in symbols_sort (flags sometimes hide some symbols)
*/

typedef struct	s_sym_sort
{
	t_symbol	*symbols;
	t_symbol	**symbols_sort;
	size_t		nsyms_sort;
}				t_sym_sort;

/*
** in nm_display.c:
**   nm_set_flag: remembers flags
**   nm_symbol_allocate: allocates memory for symbol info
**   nm_store_value: stores values if flags allow it
**   nm_sort_print_free: sorts according to flags, prints and frees
*/

bool		nm_set_flag(const char *av);
bool		nm_symbol_allocate(t_sym_sort *sorted_symbols, const uint64_t nsyms);
void		nm_store_value(t_sym_sort *sorted_symbols, const t_symbol *new_symbol);
void		nm_sort_print_free(t_sym_sort *sorted_symbols, const int padding);

/*
** in nm_sort.c
**   nm_selection_sort: sorts sorted_symbols->symbols_sort with sort_type
**   (0) alphabetically: no flags
**   (1) reverse_alphabetically: -r
**   (2) numerically: -n
**   (3) reverse_numerically: -n -r
*/


void		nm_selection_sort(t_sym_sort *sorted_symbols, uint8_t sort_type);

/*
** in nm_sections_character_table.c:
**   nm_sections_character_table: manages n_sect type character table
**   nm_extract_values: stores values before sorting
*/

char		nm_sections_character_table(const size_t offset);
void		nm_extract_values(const struct nlist *nlist, const uint64_t n_value,
				const struct symtab_command *sym, t_sym_sort *sorted_symbols);

#endif
