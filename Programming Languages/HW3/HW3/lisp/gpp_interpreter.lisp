(defpackage :gpp
  (:use :cl)
  (:export :main))
(in-package :gpp)

(defvar *environment* (make-hash-table :test 'equal))

(defun validate-values (values)
  (if (null values) t
    (let ((first (car values))
          (rest (cdr values)))
      (and (validate-value first) (or (null rest)
               (and (eq (car rest) 'comma) (validate-values (cdr rest))))))))

(defun validate-value (value)
  (or (floatp value) (integerp value) (symbolp value) (validate-exp value)))

(defun validate-exp (exp)
  (cond
    ((numberp exp) t) ((symbolp exp) t) ((listp exp)         
     (or (check-syntax (format nil "~a" exp)) (validate-exp-list exp))) (t nil))) 


(defun validate-exp-list (exp-list)
  (and (listp exp-list) (every #'validate-exp exp-list)))


(defun check-syntax (input)
  (handler-case
      (let ((tokens (read-from-string input nil nil)))
        (cond
          ((and (listp tokens) (member (car tokens) '(+ - * /)))
           (check-arith-syntax tokens))
          ((and (listp tokens) (member (car tokens) '(and or not less equal true false)))
           (check-boolean-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'defvar))
           (check-defvar-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'set))
           (check-set-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'deffun))
           (check-deffun-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'if))
           (check-if-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'while))
           (check-while-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'for))
           (check-for-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'list))
           (check-list-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'concat))
           (check-concat-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'append))
           (check-append-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'load))
           (check-load-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'print))
           (check-print-syntax tokens))
          ((and (listp tokens) (eq (car tokens) 'exit))
           (check-exit-syntax tokens))
          ((and (listp tokens) (symbolp (car tokens)))
           (check-function-call-syntax tokens))

          (t (error "Error: ~a" input))))
    (error (e) (progn (format t "REJECT: ~a~%" e) nil))))

(defun check-arith-syntax (tokens)
  (when (/= (length tokens) 3)
    (error "arithmetic expression must have 3 tokens"))
  (unless (and (validate-exp (second tokens))
               (validate-exp (third tokens)))
    (error "arithmetic expressions must have valid operands")) t)

(defun check-boolean-syntax (tokens)
  (let ((operator (car tokens))
        (operands (cdr tokens)))
    (cond
      ((member operator '(and or less equal))
       (unless (= (length operands) 2)
         (error  "'~a' requires two operands" operator))
       (unless (and (validate-exp (first operands))
                    (validate-exp (second operands)))
         (error "invalid operands for '~a'." operator)) t)
      ((eq operator 'not)
       (unless (= (length operands) 1)
         (error "'NOT' requires one operand"))
       (unless (validate-exp (first operands))
         (error "invalid operand for 'NOT'")) t)
      ((member operator '(true false))
       (unless (null operands)
         (error "'~a' does not require any operands" operator)) t)
      (t (error "invalid '~a' operator" operator)))))

(defun check-defvar-syntax (tokens)
  (let ((operator (car tokens))(args (cdr tokens)))
    (unless (and (eq operator 'defvar) (= (length args) 2) (symbolp (first args)) (validate-exp (second args)))
      (error "invalid syntax for 'defvar'")) t))

(defun check-set-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (eq operator 'set) (= (length args) 2) (symbolp (first args)) (validate-exp (second args)))
      (error "invalid syntax for 'set'")) t))

(defun check-deffun-syntax (tokens)
  (let ((operator (car tokens))
        (args (cdr tokens)))
    (unless (and (eq operator 'deffun) (= (length args) 3) (symbolp (first args)) (listp (second args))        
                 (validate-exp-list (second args)) (listp (third args)) (validate-exp-list (third args)))
      (error "invalid syntax for 'deffun'")) t))

(defun check-function-call-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (symbolp operator)
                 (validate-exp-list args))
      (error "invalid syntax for function call")) t))

(defun check-if-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (eq operator 'if) (or (= (length args) 2) (= (length args) 3))
                 (validate-exp (first args)) (validate-exp (second args))
                 (or (null (third args)) (validate-exp (third args))))
      (error "invalid syntax for 'if'")) t))

(defun check-while-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (eq operator 'while) (>= (length args) 2) (validate-exp (first args)) (validate-exp-list (second args))) 
      (error "invalid syntax for 'while'"))
    t))

(defun check-for-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (eq operator 'for) (= (length args) 2) (listp (first args))
                 (= (length (first args)) 3) (symbolp (first (first args))) 
                 (validate-exp (second (first args))) (validate-exp (third (first args))) (validate-exp-list (second args)))
      (error "invalid syntax for 'for'")) t))

(defun check-list-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (cond
      ((and (eq operator 'list) (validate-values args)) t)
      ((and (eq operator ''list) (validate-values args)) t)
      ((and (eq operator ''list) (null args)) t)
      ((eq operator 'nil) t)
      (t (error "invalid syntax for 'list'")))))

(defun check-concat-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (eq operator 'concat) (= (length args) 2) (validate-exp (first args)) (validate-exp (second args)))
      (error "invalid syntax for 'concat'")) t))

(defun check-append-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (eq operator 'append) (= (length args) 2) (validate-exp (first args)) (validate-exp (second args)))
      (error "invalid syntax for 'append'")) t))

(defun check-load-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (eq operator 'load) (= (length args) 1)(stringp (first args)))
      (error "invalid syntax for 'load'")) t))

(defun check-print-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (eq operator 'print)(= (length args) 1)        
                 (or (stringp (first args)) 
                     (validate-exp (first args))))
                     (error "invalid syntax for 'print'")) t))

(defun check-exit-syntax (tokens)
  (let ((operator (car tokens)) (args (cdr tokens)))
    (unless (and (eq operator 'exit) (= (length args) 0))
      (error "invalid syntax for 'exit'")) t))

(defun gppinterpreter (&optional (file nil))
  (if file
      (with-open-file (stream file)
        (loop
          (let ((input (read-line stream nil))) 
            (unless input (return))
            (let ((result (check-syntax input)))
              (if result
                  (format t "ACCEPT~%")
                  (format t "REJECT~%"))))))
      (loop
        (format t "gpp> ")
        (let ((input (read-line)))
          (cond
            ((string= input "(exit)")
             (return))
            (t
             (let ((result (check-syntax input)))
               (if result
                   (format t "ACCEPT~%")
                   (format t "REJECT~%")))))))))

(defun main ()
  (let ((args ext:*args*))
    (if (and args (> (length args) 0))
        (gppinterpreter (first args))
        (gppinterpreter))))

(main)
