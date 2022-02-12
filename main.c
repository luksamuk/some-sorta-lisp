#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "def.h"
#include "types.h"
#include "atom_table.h"
#include "atom.h"
#include "pointers.h"
#include "list_area.h"
#include "printing.h"
#include "vm.h"
#include "reader.h"

int
main()
{
    printf("Lisp interpreter v0.0\n"
           "Written by Lucas S. Vieira\n\n");
    
    lisp_vm_t *vm = make_lisp_vm();
    init_lisp_vm(vm);

    char buffer[256];
    printf(
        "\nType an S-Expression then press enter to echo.\n"
        "Use Ctrl+D to abort.\n");
    while(fgets(buffer, sizeof buffer, stdin)) {
        putchar('>');
        putchar(' ');
        print_object(&vm->table, &vm->area, read_expression(vm, buffer));
        putchar(10);
    }
    putchar(10);

    printf("ATOM TABLE\n----------\n");
    print_atom_table(&vm->table);

    putchar(10);
    printf("LIST AREA\n---------\n");
    print_list_area(&vm->area);

    debrief_vm(vm);

    clear_lisp_vm(vm);
    return 0;
}

