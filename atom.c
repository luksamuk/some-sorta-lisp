#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "atom.h"
#include "pointers.h"

char *
make_atom_name(const char *given_name)
{
    size_t i;
    size_t num_chars = strlen(given_name);
    char *name = malloc((num_chars + 1) * sizeof(char));
    strcpy(name, given_name);
    for(i = 0; i < num_chars; i++) {
        name[i] = toupper(name[i]);
    }
    name[num_chars] = '\0';
    return name;
}

lisp_untptr_t
make_atom(atom_table_t *table, const char *atom_name)
{
    lisp_untptr_t atom_addr = table->last;
    table->last++;
    atom_table_entry_t *entry = &table->table_ptr[atom_addr];
    entry->name = make_atom_name(atom_name);
    entry->value = entry->plist = entry->bindlist = 0;
    return atom_addr;
}

lisp_untptr_t
bind_atom(
    atom_table_t *table,
    lisp_untptr_t atom, lisp_ptr_t value)
{
    if(atom >= table->last)
        return 0;
    atom_table_entry_t *entry = &table->table_ptr[atom];
    entry->value = value;
    return entry->value;
}

const char *
get_atom_name(atom_table_t *table, lisp_untptr_t atom)
{
    return table->table_ptr[atom].name;
}
