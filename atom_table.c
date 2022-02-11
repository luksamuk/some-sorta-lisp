#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "def.h"
#include "util.h"
#include "types.h"
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
            printf("\n%6s | %15s | %14s | %5s | %5s\n",
                   "ADDR", "SYMBOL NAME",
                   "VALUE", "PLIST", "BINDL");
            int i = 0;
            while(i < 57) {
                putchar('-');
                i++;
            }
            putchar(10);
        }
        
        atom_table_entry_t *entry = &table->table_ptr[current];
        printf("%06X | %15s | %5s | %06X | %05X | %05X\n",
               current,
               entry->name,
               type_repr_str(get_ptr_tag(entry->value)),
               get_ptr_content(entry->value),
               entry->plist,
               entry->bindlist);
        current++;
    }
}

