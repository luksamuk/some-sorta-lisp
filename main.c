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
#include "eval.h"

int
main()
{
    printf("Lisp interpreter v0.0\n"
           "Written by Lucas S. Vieira\n");
    
    lisp_vm_t *vm = make_lisp_vm();
    init_lisp_vm(vm);

    {
        // Slurp test file
        char *testfile = slurp("test.l");
        if(testfile) {
            printf("Loading test.l...\n");
            read_expression(vm, testfile);
        }
        free(testfile);
    }

    char buffer[256];
    printf(
        "Type an S-Expression then press enter to echo.\n"
        "Use Ctrl+D to abort.\n");
    while(1) {
        putchar('>');
        putchar(' ');
        char *result = fgets(buffer, sizeof buffer, stdin);
        if(!result) break;
        lisp_ptr_t expr = read_expression(vm, buffer);
        lisp_ptr_t expr_eval = vm_proto_eval(vm, expr);
        print_object(&vm->table, &vm->area, expr_eval);
        putchar(10);
    }
    putchar(10);

    debrief_vm(vm);
    
#ifdef NDEBUG
    putchar(10);
    printf("ATOM TABLE\n----------\n");
    print_atom_table(&vm->table);

    putchar(10);
    printf("LIST AREA\n---------\n");
    print_list_area(&vm->area);
#endif

    clear_lisp_vm(vm);
    return 0;
}

