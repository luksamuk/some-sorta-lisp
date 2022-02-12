(SETQ APPEND
      (LAMBDA (X Y)
        (COND ((EQ X NIL) Y)
              ((ATOM X) (CONS X Y))
              (T (CONS (CAR X)
                       (APPEND (CDR X) Y))))))

(SETQ REVERSE
      (LAMBDA (X)
        (COND ((ATOM X) X)
              (T (APPEND (REVERSE (CDR X))
                         (CONS (CAR X) NIL))))))

(SETQ EQUAL
      (LAMBDA (X Y)
        (COND ((OR (ATOM X) (ATOM Y))
               (EQ X Y))
              ((EQUAL (CAR X) (CAR Y))
               (EQUAL (CDR X) (CDR Y)))
              (T NIL))))

(SETQ LENGTH
      (LAMBDA (L)
        (COND ((ATOM L) 0)
              (T (PLUS 1 (LENGTH (CDR L)))))))

(SETQ LISTP
      (LAMBDA (L)
        (COND ((ATOM L)
               (EQ L NIL))
              (T (LISTP (CDR L))))))

(SETQ MEMBER
      (LAMBDA (A S)
        (COND ((EQ S NIL) NIL)
              ((EQUAL A (CAR S)) T)
              (T (MEMBER A (CDR S))))))

(SETQ PLACE
      (LAMBDA (A S)
        (COND ((EQ S NIL) (QUOTE NULL))
              ((EQUAL A (CAR S)) (CDR S))
              (T (PLACE A (CDR S))))))

(SETQ DEEPMEM
      (LAMBDA (A S)
        (COND ((ATOM S) NIL)
              ((OR (EQUAL A (CAR S))
                   (DEEPMEM A (CAR S))) T)
              (T (DEEPMEM A (CDR S))))))

(SETQ TRUNC
      (LAMBDA (L)
        (COND ((ATOM L) L)
              ((EQ NIL (CDR L)) NIL)
              (T (CONS (CAR L) (TRUNC (CDR L)))))))

(SETQ DEPTH
      (LAMBDA (A)
        (COND ((ATOM A) 0)
              (T (PLUS 1 (PLUS (DEPTH (CAR A))
                               (DEPTH (CDR A))))))))

(SETQ FLATP
      (LAMBDA (L)
        (COND ((EQ NIL L) T)
              ((ATOMP (CAR L))
               (FLATP (CDR L)))
              (T NIL))))

(SETQ FLAT
      (LAMBDA (L)
        (COND ((ATOM L) L)
              (T (APPEND (FLAT (CAR L))
                         (FLAT (CDR L)))))))

(SETQ PURE
      (LAMBDA (L)
        (COND ((ATOM L) (EQ NIL L))
              (T (PURE (CDR L))))))

(SETQ SETQQ
      (SPECIAL (X Y)
               (EVAL
                (LIST (QUOTE SETQ)
                      X
                      (CONS (QUOTE QUOTE)
                            (CONS Y NIL))))))

(SETQ SET
      (SPECIAL (X Y)
               (EVAL
                (LIST (QUOTE SETQ)
                      (EVAL X)
                      Y))))

(SETQ IF
      (SPECIAL (PRED CONSEQ ALT)
               (COND ((EVAL PRED) (EVAL CONSEQ))
                     (T (EVAL ALT)))))

(SETQ NOT
      (LAMBDA (X)
        (EQ X NIL)))

(SETQ NULL
      (LAMBDA (X)
        (NOT X)))

