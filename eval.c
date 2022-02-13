#include <stdlib.h>
#include <stdio.h>

#include "eval.h"
#include "types.h"
#include "util.h"
#include "list_area.h"
#include "pointers.h"
#include "reader.h"
#include "printing.h"
#include "stack.h"
#include "primitives.h"

lisp_ptr_t
vm_lookup(lisp_vm_t *vm, lisp_ptr_t atom)
{
    if(get_ptr_tag(atom) != TYPE_ATOM) {
        dbg("Error: pointer 0x%08X is not an atom\n", atom);
        return TP_NIL;
    }
    
    lisp_untptr_t addr = get_ptr_content(atom);
    return vm->table.table_ptr[addr].value;
}


lisp_ptr_t apply_primitive_proc(lisp_vm_t *vm, lisp_ptr_t fun, lisp_ptr_t argl);
lisp_ptr_t vm_make_bindings(lisp_vm_t *vm, lisp_ptr_t lambda_list, lisp_ptr_t argl);

lisp_ptr_t
vm_explicit_eval(lisp_vm_t *vm)
{
    // This is an explicit control evaluator inspired by SICP.
    // In fact, it was build by looking at the SICP classes and writing
    // the evaluator by transforming its instructions into proper modifications
    // of the VM's registers.
    
    register_area_t *reg = &vm->registers;

    // Helper atoms
    lisp_ptr_t
        tp_exp_err               = vm_get_atom(vm, "expression-error"),
        tp_done                  = vm_get_atom(vm, "done"),
        tp_eval_args             = vm_get_atom(vm, "eval-args"),
        tp_accumulate_arg        = vm_get_atom(vm, "accumulate-arg"),
        tp_accumulate_last_arg   = vm_get_atom(vm, "accumulate-last-arg"),
        tp_unknown_function_type = vm_get_atom(vm, "unknown-function-type");

    // We assume that EXP and ENV are in place.
    reg->cont = tp_done;

    /* UTILITY MACROS */
    
    // Macro for go-to's under CONT register
#define GOTO_CONTINUE_REGISTER                                        \
    if(reg->cont == tp_done) goto done;                               \
    if(reg->cont == tp_eval_args) goto eval_args;                     \
    if(reg->cont == tp_accumulate_arg) goto accumulate_arg;           \
    if(reg->cont == tp_accumulate_last_arg) goto accumulate_last_arg; \
    goto expression_error;

    // Common helper predicates
#define SELF_EVALUATINGP(tptr) (get_ptr_tag(tptr) == TYPE_NUMBER)
#define VARIABLEP(tptr) (get_ptr_tag(tptr) == TYPE_ATOM)
#define PRIMITIVE_FUNCTION_P(tptr) (get_ptr_tag(tptr) == TYPE_BUILTIN_FUNCTION)
#define PRIMITIVE_SPECIAL_P(tptr) (get_ptr_tag(tptr) == TYPE_BUILTIN_SPECIAL)
#define COMPOUND_FUNCTION_P(tptr) (get_ptr_tag(tptr) == TYPE_FUNCTION)
#define COMPOUND_SPECIAL_P(tptr) (get_ptr_tag(tptr) == TYPE_FUNCTION)
#define FUNCTIONP(tptr) \
    ((get_ptr_tag(tptr) == TYPE_BUILTIN_FUNCTION) \
     || (get_ptr_tag(tptr) == TYPE_FUNCTION))
#define SPECIALP(tptr) \
    ((get_ptr_tag(tptr) == TYPE_BUILTIN_SPECIAL) \
     || (get_ptr_tag(tptr) == TYPE_SPECIAL))
#define APPLICATIONP(tptr) \
    ((get_ptr_tag(tptr) == TYPE_CONS) \
     && (FUNCTIONP(vm_lookup(vm, vm_car(vm, tptr))) \
         || (SPECIALP(vm_lookup(vm, vm_car(vm, tptr))))))
#define LAST_OPERAND_P(tptr) (vm_cdr(vm, tptr) == TP_NIL)
    
    /* EVALUATION */
eval_dispatch:
    edbg("eval-dispatch\n");
    // Entry point for evaluation
    if(SELF_EVALUATINGP(reg->exp)) goto ev_self_eval;
    if(VARIABLEP(reg->exp))        goto ev_variable;
    if(APPLICATIONP(reg->exp))     goto ev_application;
    goto expression_error;

ev_self_eval:
    edbg("ev-self-eval\n");
    reg->val = reg->exp;
    GOTO_CONTINUE_REGISTER;
    
ev_application:
    edbg("ev-application\n");
    reg->unev = vm_cdr(vm, reg->exp);
    reg->exp  = vm_car(vm, reg->exp);

    // Setup registers for recursive evaluation
    vm_stack_push(vm, reg->cont);
    vm_stack_push(vm, reg->env);
    vm_stack_push(vm, reg->unev);
    reg->cont = tp_eval_args;
    goto eval_dispatch;
    
ev_variable:
    edbg("ev-variable\n");
    reg->val = vm_lookup(vm, reg->exp);
    GOTO_CONTINUE_REGISTER;
    
eval_args:
    edbg("eval-args\n");
    reg->unev = vm_stack_pop(vm);
    reg->env  = vm_stack_pop(vm);
    reg->fun  = reg->val;
    vm_stack_push(vm, reg->fun);
    reg->argl = TP_NIL;
    goto eval_arg_loop;

eval_arg_loop:
    edbg("eval-arg-loop\n");
    vm_stack_push(vm, reg->argl);
    reg->exp = vm_car(vm, reg->unev);
    if(LAST_OPERAND_P(reg->unev)) goto eval_last_arg;
    vm_stack_push(vm, reg->env);
    vm_stack_push(vm, reg->unev);
    reg->cont = tp_accumulate_arg;
    goto eval_dispatch;
    
accumulate_arg:
    edbg("accumulate-arg\n");
    reg->unev = vm_stack_pop(vm);
    reg->env  = vm_stack_pop(vm);
    reg->argl = vm_stack_pop(vm);
    reg->argl = vm_cons(vm, reg->val, reg->argl);
    reg->unev = vm_cdr(vm, reg->unev);
    goto eval_arg_loop;
    
eval_last_arg:
    edbg("eval-last-arg\n");
    reg->cont = tp_accumulate_last_arg;
    goto eval_dispatch;
    
accumulate_last_arg:
    edbg("accumulate-last-arg\n");
    reg->argl = vm_stack_pop(vm);
    reg->argl = vm_cons(vm, reg->val, reg->argl);
    reg->fun  = vm_stack_pop(vm);
    goto apply_dispatch;

    
    /* APPLICATION */
apply_dispatch:
    edbg("apply-dispatch\n");
    if(PRIMITIVE_FUNCTION_P(reg->fun)) goto primitive_apply;
    //if(PRIMITIVE_SPECIAL_P(reg->fun))  goto expression_error; // Unimplemented
    if(COMPOUND_FUNCTION_P(reg->fun))  goto compound_apply;
    //if(COMPOUND_SPECIAL_P(reg->fun))   goto expression_error; // Unimplemented
    goto unknown_function_type_error;
    
primitive_apply:
    edbg("primitive-apply\n");
    reg->val = apply_primitive_proc(vm, reg->fun, reg->argl);
    reg->cont = vm_stack_pop(vm);
    GOTO_CONTINUE_REGISTER;
    
compound_apply:
    edbg("compound-apply\n");
    // Compound functions are pointers to a cons #<FUNCTION (ARGS . BODY)>
    reg->exp  = vm_cdr(vm, reg->fun);
    reg->env  = vm_make_bindings(vm, vm_car(vm, reg->fun), reg->argl);
    reg->cont = vm_stack_pop(vm);
    goto eval_dispatch;
    
done:
    return reg->val;
expression_error:
    return tp_exp_err;
unknown_function_type_error:
    return tp_unknown_function_type;
}

lisp_ptr_t
vm_eval(lisp_vm_t *vm, lisp_ptr_t expr)
{
    vm->registers.exp = expr;
    vm->registers.env = TP_NIL;
    return vm_explicit_eval(vm);
}

void
vm_load_file(lisp_vm_t *vm, const char *filename)
{
    printf("Load file %s...\n", filename);
    char *file = slurp(filename);
    if(file) {
        lisp_ptr_t expr = read_expression(vm, file);
        free(file);
        
        while(expr != TP_NIL) {
            lisp_untptr_t content = get_ptr_content(expr);
            lisp_ptr_t result =
                vm_eval(vm, get_car(&vm->area, content));
            print_object(&vm->table, &vm->area, result);
            putchar(10);
            expr = get_cdr(&vm->area, content);
        }
        putchar(10);
    }
}


lisp_ptr_t
apply_primitive_proc(lisp_vm_t *vm, lisp_ptr_t fun, lisp_ptr_t argl)
{
    lisp_ptr_t tp_plus = vm_get_atom(vm, "+");
    if(get_ptr_content(fun) == get_ptr_content(tp_plus)) {
        lisp_ptr_t result =
            make_pointer(
                TYPE_NUMBER,
                get_ptr_content(vm_car(vm, argl)) +
                get_ptr_content(vm_cadr(vm, argl)));
        return result;
    }
    return TP_NIL;
}

lisp_ptr_t
vm_make_bindings(lisp_vm_t *vm, lisp_ptr_t lambda_list, lisp_ptr_t argl)
{
    return TP_NIL;
}
