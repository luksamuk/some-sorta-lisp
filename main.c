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

void
main()
{
    print("Lisp interpreter v0.0\n"
          "Written by Lucas S. Vieira\n");
    
    lisp_vm_t *vm = make_lisp_vm();
    
    if(vm == nil) {
        sysfatal("Unable to initialize Virtual Machine!");
    }
    
    init_lisp_vm(vm);

    char buffer[256];
    print("Use Ctrl+D to abort.\n");
    int echo_p = 0;
    while(1) {
        print("> ");
        char *result = fgets(buffer, sizeof buffer, stdin);
        long len = strlen(buffer);
        if(len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
        if(!result) break;
        if(!strncmp("#dump", buffer, 5)) {
            print("\nATOM TABLE\n----------\n");
            print_atom_table(&vm->table);
            print("\nLIST AREA\n---------\n");
            print_list_area(&vm->area);
            print("\n");
        } else if(!strncmp("#statistics", buffer, 11)) {
            debrief_vm(vm);
        } else if(!strncmp("#echo", buffer, 5)) {
            echo_p = !echo_p;
            print("echo %s\n", echo_p ? "on" : "off");
        } else if(!strncmp("#load ", buffer, 6)) {
            vm_load_file(vm, result + 6);
        } else {
            lisp_ptr_t expr = read_expression(vm, buffer);
            if(echo_p) {
                print("# ");
                print_object(&vm->table, &vm->area, expr);
                print("\n");
            }
            
            expr = vm_car(vm, expr);
            lisp_ptr_t expr_eval = vm_eval(vm, expr);
            print_object(&vm->table, &vm->area, expr_eval);
            print("\n");
        }
		buffer[0] = '\0';
    }
    print("\n");
    
#ifdef NDEBUG
    debrief_vm(vm);
#endif

    clear_lisp_vm(vm);
}

