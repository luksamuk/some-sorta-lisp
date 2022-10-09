#include "eval.h"
#include "types.h"
#include "util.h"
#include "atom.h"
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


lisp_ptr_t apply_primitive_fn(lisp_vm_t *vm, lisp_ptr_t fun, lisp_ptr_t argl);
lisp_ptr_t vm_make_bindings(lisp_vm_t *vm, lisp_ptr_t lambda_list, lisp_ptr_t argl);
lisp_ptr_t vm_bind(lisp_vm_t *vm, lisp_ptr_t atom, lisp_ptr_t value);

enum EV_LABEL_JMP {
    LBL_EXPRESSION_ERROR,
    LBL_DONE,
    LBL_EVAL_ARGS,
    LBL_ACCUMULATE_ARG,
    LBL_ACCUMULATE_LAST_ARG,
    LBL_EVAL_ASSIGN,
};

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
	tp_quote                 = vm_get_atom(vm, "quote"),
        tp_lambda                = vm_get_atom(vm, "lambda"),
        tp_setq                  = vm_get_atom(vm, "setq");

    // We assume that EXP and ENV are in place.
    reg->cont = LBL_DONE;

    /* UTILITY MACROS */
    
    // Macro for go-to's under CONT register
#define GOTO_CONTINUE_REGISTER                                          \
    if(reg->cont == LBL_DONE)                goto done;                 \
    if(reg->cont == LBL_EVAL_ARGS)           goto eval_args;            \
    if(reg->cont == LBL_ACCUMULATE_ARG)      goto accumulate_arg;       \
    if(reg->cont == LBL_ACCUMULATE_LAST_ARG) goto accumulate_last_arg;  \
    if(reg->cont == LBL_EVAL_ASSIGN)         goto eval_assign;          \
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

#define IS_SPECIAL_FORM(tptr, tpspcl) \
    ((get_ptr_tag(tptr) == TYPE_CONS) && (vm_car(vm, tptr) == tpspcl))

#define QUOTE_P(tptr)  IS_SPECIAL_FORM(tptr, tp_quote)
#define LAMBDA_P(tptr) IS_SPECIAL_FORM(tptr, tp_lambda)
#define SETQ_P(tptr)   IS_SPECIAL_FORM(tptr, tp_setq)

    
    /* EVALUATION */
eval_dispatch:
    edbg("eval-dispatch\n");
    // Entry point for evaluation
    
    if(SELF_EVALUATINGP(reg->exp)) goto ev_self_eval;
    if(VARIABLEP(reg->exp))        goto ev_variable;

    // Special forms
    if(QUOTE_P(reg->exp))          goto ev_quote;
    if(LAMBDA_P(reg->exp))         goto ev_lambda;
    if(SETQ_P(reg->exp))           goto ev_setq;

    // Application
    if(APPLICATIONP(reg->exp))     goto ev_application;
    goto expression_error;

ev_self_eval:
    edbg("ev-self-eval\n");
    reg->val = reg->exp;
    GOTO_CONTINUE_REGISTER;
    
ev_variable:
    edbg("ev-variable\n");
    reg->val = vm_lookup(vm, reg->exp);
    GOTO_CONTINUE_REGISTER;

ev_quote:
    edbg("ev-quote\n");
    reg->val = vm_cadr(vm, reg->exp);
    GOTO_CONTINUE_REGISTER;

ev_lambda:
    edbg("ev-lambda\n");
    reg->val = make_pointer(TYPE_FUNCTION, vm_cdr(vm, reg->exp));
    GOTO_CONTINUE_REGISTER;

ev_setq:
    edbg("ev-setq\n");
    vm_stack_push(vm, reg->cont);
    vm_stack_push(vm, vm_cadr(vm, reg->exp));
    reg->exp = vm_caddr(vm, reg->exp);
    reg->cont = LBL_EVAL_ASSIGN;
    goto eval_dispatch;

eval_assign:
    edbg("ev-assign\n");
    reg->exp = reg->val;
    reg->val = vm_stack_pop(vm);
    reg->val = vm_bind(vm, reg->val, reg->exp);
    reg->cont = vm_stack_pop(vm);
    if(reg->val == tp_exp_err) return tp_exp_err;
    GOTO_CONTINUE_REGISTER;
    
ev_application:
    edbg("ev-application\n");
    reg->unev = vm_cdr(vm, reg->exp);
    reg->exp  = vm_car(vm, reg->exp);
    vm_stack_push(vm, reg->cont);
    vm_stack_push(vm, reg->env);
    vm_stack_push(vm, reg->unev);
    reg->cont = LBL_EVAL_ARGS;
    goto eval_dispatch;

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
    reg->cont = LBL_ACCUMULATE_ARG;
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
    reg->cont = LBL_ACCUMULATE_LAST_ARG;
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
    if(PRIMITIVE_FUNCTION_P(reg->fun)) goto primitive_fn_apply;
    //if(PRIMITIVE_SPECIAL_P(reg->fun))  goto expression_error; // Unimplemented
    if(COMPOUND_FUNCTION_P(reg->fun))  goto compound_fn_apply;
    goto unknown_function_type_error;
    
primitive_fn_apply:
    edbg("primitive-fn-apply\n");
    reg->val = apply_primitive_fn(vm, reg->fun, reg->argl);
    reg->cont = vm_stack_pop(vm);
    GOTO_CONTINUE_REGISTER;
    
compound_fn_apply:
    edbg("compound-fn-apply\n");
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
    return vm_get_atom(vm, "unknown-function-type");
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
apply_primitive_fn(lisp_vm_t *vm, lisp_ptr_t fun, lisp_ptr_t argl)
{
    // REMINDER: argl is inverted!
    lisp_ptr_t
	tp_typeof = vm_get_atom(vm, "typeof"),
        tp_plus   = vm_get_atom(vm, "+");

    if(get_ptr_content(fun) == get_ptr_content(tp_plus)) {
        lisp_ptr_t result =
            make_pointer(
                TYPE_NUMBER,
                get_ptr_content(vm_car(vm, argl)) +
                get_ptr_content(vm_cadr(vm, argl)));
        return result;
    } else if(get_ptr_content(fun) == get_ptr_content(tp_typeof)) {
        lisp_ptr_t result =
		vm_get_atom(vm, type_repr_str(get_ptr_tag(vm_car(vm, argl))));
       return result;	
    }
    return TP_NIL;
}

lisp_ptr_t
vm_make_bindings(lisp_vm_t *vm, lisp_ptr_t lambda_list, lisp_ptr_t argl)
{
    // TODO: Make proper binding routine
    UNUSED(vm);
    UNUSED(lambda_list);
    UNUSED(argl);
    return TP_NIL;
}

lisp_ptr_t
vm_bind(lisp_vm_t *vm, lisp_ptr_t atom, lisp_ptr_t value)
{
    if((get_ptr_tag(atom) != TYPE_ATOM)
       || (get_ptr_tag(value) == TYPE_UNDEFINED))
        return vm_get_atom(vm, "expression-error");
    bind_atom(&vm->table, get_ptr_content(atom), value);
    return value;
}
