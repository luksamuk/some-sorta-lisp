#include "def.h"
#include "util.h"
#include "types.h"
#include "atom.h"
#include "pointers.h"
#include "atom_table.h"

void
init_atom_table(atom_table_t *table_ptr)
{
    memset(table_ptr, 0, sizeof(atom_table_t));
}

void
clear_atom_table(atom_table_t *table)
{
    lisp_untptr_t current = 0;
    while(current < table->last) {
        free(table->table_ptr[current].name);
        current++;
    }
    table->last = 0;
}

void
print_atom_table(atom_table_t *table)
{
    lisp_untptr_t current = 0;
    while(current < table->last) {
        if(current % 100 == 0) {
            print("\n%6s | %21s | %14s | %5s | %5s\n",
                  "ADDR", "SYMBOL NAME",
                  "VALUE", "PLIST", "BINDL");
            int i = 0;
            while(i < 63) {
                print("-");
                i++;
            }
            print("\n");
        }
        
        atom_table_entry_t *entry = &table->table_ptr[current];
        print("%06X | %21s | %5s | %06X | %05X | %05X\n",
              current,
              entry->name,
              type_repr_str(get_ptr_tag(entry->value)),
              get_ptr_content(entry->value),
              entry->plist,
              entry->bindlist);
        current++;
    }
}

lisp_untptr_t
find_atom(atom_table_t *table, const char *name)
{
    // Linear search...
    // I promes I'll improve this, but we'll need
    // some sort of map or compact radix tree to do
    // that! I'm still sorting it out.
    lisp_untptr_t current = 0;
    char *correct_name = make_atom_name(name);
    while(current < table->last) {
        if(strcmp(get_atom_name(table, current), correct_name) == 0) {
            free(correct_name);
            return current;
        }
        current++;
    }
    free(correct_name);
    return make_atom(table, name);
}
