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

atom_table_t
make_atom_table()
{
    size_t byte_size = ATOM_TABLE_SIZE * sizeof(atom_table_entry_t);
    dbg("starting atom table with %lu cells (%lu bytes).\n",
        ATOM_TABLE_SIZE, byte_size);
    atom_table_entry_t *table_ptr = malloc(byte_size);
    memset(table_ptr, 0, byte_size);

    atom_table_t table;
    table.table_ptr = table_ptr;
    table.last = 0;

    return table;
}

void
clear_atom_table(atom_table_t *table)
{
    lisp_untptr_t current = 0;
    while(current < table->last) {
        free(table->table_ptr[current].name);
        current++;
    }
    free(table->table_ptr);
}

void
print_atom_table(atom_table_t *table)
{
    lisp_untptr_t current = 0;
    while(current < table->last) {
        if(current % 100 == 0) {
            printf("\n%5s | %15s | %14s | %5s | %5s\n",
                   "ADDR", "SYMBOL NAME",
                   "VALUE", "PLIST", "BINDL");
            int i = 0;
            while(i < 60) {
                putchar('-');
                i++;
            }
            putchar(10);
        }
        
        atom_table_entry_t *entry = &table->table_ptr[current];
        printf("%05X | %15s | %5s | %06lu | %05X | %05X\n",
               current,
               entry->name,
               type_repr_str(get_ptr_tag(entry->value)),
               get_ptr_content(entry->value),
               entry->plist,
               entry->bindlist);
        current++;
    }
}

