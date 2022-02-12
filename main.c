#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    vm_load_file(vm, "init.l");

    char buffer[256];
    printf("Use Ctrl+D to abort.\n");
    int echo_p = 0;
    while(1) {
        putchar('>');
        putchar(' ');
        char *result = fgets(buffer, sizeof buffer, stdin);
        if(!result) break;
        if(!strcmp("#dump\n", result)) {
            putchar(10);
            printf("ATOM TABLE\n----------\n");
            print_atom_table(&vm->table);
            putchar(10);
            printf("LIST AREA\n---------\n");
            print_list_area(&vm->area);
            putchar(10);
        } else if(!strcmp("#echo\n", result)) {
            echo_p = !echo_p;
            printf("echo %s\n", echo_p ? "on" : "off");
        } else {
            lisp_ptr_t expr = read_expression(vm, buffer);
            if(echo_p) {
                printf("# ");
                print_object(&vm->table, &vm->area, expr);
                putchar(10);
            }
            expr = get_car(&vm->area, get_ptr_content(expr));
            lisp_ptr_t expr_eval = vm_proto_eval(vm, expr);
            print_object(&vm->table, &vm->area, expr_eval);
            putchar(10);
        }
    }
    putchar(10);
    
#ifdef NDEBUG
    debrief_vm(vm);
#endif

    clear_lisp_vm(vm);
    return 0;
}

