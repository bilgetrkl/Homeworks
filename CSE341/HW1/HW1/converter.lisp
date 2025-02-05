(defun line-type (line)
      (cond
      ((search "if" line) 'if-statement)

      ((search "for" line) 'for-loop)

      ((search "while" line) 'while-loop)

      ((search "}" line) 'curly-brace)

      ((search "return" line) (cond
            ((or (search "++" line) (search "--" line)) 'post-increment-decrement-return)
            ((or (search "+" line) (search "-" line) (search "*" line) (search "/" line) (search "%" line)) 'arithmetic-operation-return)
            ((or (search "&&" line) (search "||" line) (search ">" line) (search "<" line)
                 (search ">=" line) (search "<=" line) (search "==" line) (search "!=" line)) 'logical-operation-return)
            (t 'return-statement)))

      ((or (search "++" line) (search "--" line)) 'post-increment-decrement)

      ((or (search "<" line) (search ">" line) (search "<=" line) (search ">=" line) 
           (search "==" line) (search "!=" line) (search "&&" line) (search "||" line)) 'logical-operation)

      ((and (search "=" line) (search "(" line) (search ")" line) (search ";" line)) 'variable-assignment-by-function-return)

      ((and (or (search "int " line) (search "float " line) (search "char " line) (search "double " line))
            (search "(" line) (search ")" line) (search ";" line) (not (search "{" line))) 'function-prototype-definition)

      ((and (search "(" line) (search ")" line) (search ";" line) (not (search "{" line)) (not (search "return" line)) (not (search "=" line))) 'function-call)

      ((and (search "(" line) (search ")" line) (search "{" line)) 'function-definition)

      ((and (or (search "int " line) (search "float " line) (search "char " line) (search "double " line)) (search "=" line) 
            (or (search "+" line) (search "-" line) (search "*" line) (search "/" line) (search "%" line)) 
            (search ";" line)) 'variable-definition-by-arithmetic-operation)

      ((and (or (search "int" line) (search "float" line) (search "char" line) (search "double" line))
            (search "=" line) (search ";" line)) 'variable-definition)

      ((and (search "=" line) (or (search "+" line) (search "-" line) (search "*" line) (search "/" line) (search "%" line)) 
            (search ";" line) (not (search "(" line))) 'variable-assignment-by-arithmetic-operation)

      ((and (search "=" line) (search ";" line) (not (search "(" line))) 'variable-assignment)

      ((and (or (search "+" line) (search "-" line) (search "*" line) (search "/" line) (search "%" line)) (not (search "=" line ))) 'arithmetic-operation)

      (t 'unknown)))

(defun data-types (data-type)
      (cond ((string= data-type "int") "integer")
            ((string= data-type "float") "float")
            ((string= data-type "char") "character")
            ((string= data-type "double") "double-float")
            (t 'unknown)))

(defun logical-operators (operator)
      (cond
            ((string= operator "<") "<")
            ((string= operator ">") ">")
            ((string= operator "<=") "<=")
            ((string= operator ">=") ">=")
            ((string= operator "!=") "/=")
            ((string= operator "==") "=")
            ((string= operator "&&") "and")
            ((string= operator "||") "or")
            (t operator)))

(defun arithmetic-operators (operator)
      (cond
            ((string= operator "+") "+")
            ((string= operator "-") "-")
            ((string= operator "*") "*")
            ((string= operator "/") "/")
            ((string= operator "%") "MOD")
            ((string= operator "++") "incf")
            ((string= operator "--") "decf")
            (t operator)))

(defun split-string (line)
      (labels ((helper (chars current-word result-list)
                       (cond
                              ((null chars) 
                               (if (string= current-word "") (nreverse result-list)
                                   (nreverse (cons current-word result-list))))
                              ((member (car chars) '(#\Space #\Tab #\; #\( #\, #\)))
                               (if (string= current-word "")
                                   (helper (cdr chars) "" result-list)
                                    (helper (cdr chars) "" (cons current-word result-list))))
                              (t 
                               (helper (cdr chars) 
                                       (concatenate 'string current-word (string (car chars))) result-list)))))
                              (helper (coerce line 'list) "" '())))

(defun split-logical-arithmetic (line)
      (defun clean-line (line) (string-trim " " (substitute #\Space #\; line)))
      (defun process-tokens (tokens)
                            (if (and (>= (length tokens) 3) (string= (first tokens) "return"))
                                (progn
                                    (if (>= (length (rest tokens)) 3)
                                        (values (second (rest tokens)) (first (rest tokens)) (third (rest tokens)))))
                                (if (>= (length tokens) 3) (values (second tokens) (first tokens) (third tokens)))))
      (process-tokens (remove-if (lambda (token) (string= token ";")) (split-string (clean-line line)))))

(defun split-function-call (line)
      (labels ((split-helper (chars current-word result)
             (cond ((null chars) 
                    (if (string= current-word "") (nreverse result) (nreverse (cons current-word result))))
                   ((member (first chars) '(#\" #\; #\( #\) #\,))
                    (if (string= current-word "") (split-helper (rest chars) "" result) (split-helper (rest chars) "" (cons current-word result))))
                   (t (split-helper (rest chars) (concatenate 'string current-word (string (first chars))) result)))))
                   (split-helper (coerce line 'list) "" '())))

(defun split-post-increment-decrement (line)
      (loop for clean-line = (string-trim " " (substitute #\Space #\; line))
            for trimmed-line = (if (string= (subseq clean-line 0 (min 6 (length clean-line))) "return")
                                   (string-trim " " (subseq clean-line 6)) clean-line)
            for index from 0 below (- (length trimmed-line) 1) 
            for var-name = (string-trim " " (subseq trimmed-line 0 index))
            for operator = (cond
                              ((string= (subseq trimmed-line index (+ index 2)) "++") "++")
                              ((string= (subseq trimmed-line index (+ index 2)) "--") "--") (t nil))
            when operator do (return (values var-name operator))))

(defun if-conversion (line)
      (let* ((space-count (loop for char across line
                                while (char= char #\Space) count 1))
             (trimmed-line (subseq line space-count)) 
             (start (search "(" trimmed-line)) (end (search ")" trimmed-line))
             (condition (if (and start end) (subseq trimmed-line (+ start 1) end) ""))
             (converted-condition (logical-operation-conversion condition)) 
             (core-conversion (if converted-condition 
                                    (format nil "(if ~A" converted-condition) trimmed-line)) 
             (converted-line (concatenate 'string 
                                    (make-string space-count :initial-element #\Space) core-conversion))) converted-line))

(defun for-conversion (line)
      (let* ((space-count (loop for char across line
                                while (char= char #\Space) count 1))
             (trimmed-line (subseq line space-count))
             (plus-minus (cond ((search "++" trimmed-line) "++") ((search "--" trimmed-line) "--") (t nil)))
             (tokens (split-string trimmed-line)) (var-name (nth 2 tokens)) (start-value (nth 4 tokens)) (end-value (nth 7 tokens))
             (loop-direction (if (string= plus-minus "++") "below" "downto")))
            (let ((converted-line (format nil "(loop for ~A from ~A ~A ~A do" var-name start-value loop-direction end-value)))
                 (concatenate 'string 
                        (make-string space-count :initial-element #\Space) converted-line))))

(defun while-conversion (line)
      (let* ((space-count (loop for char across line
                                while (char= char #\Space) count 1))
             (trimmed-line (subseq line space-count))
             (start (search "(" trimmed-line)) (end (search ")" trimmed-line))
             (condition (if (and start end) (subseq trimmed-line (+ start 1) end) ""))
             (converted-condition (logical-operation-conversion condition)))
            (if converted-condition
                  (let ((converted-line (format nil "(loop while ~A do" converted-condition)))
                       (concatenate 'string (make-string space-count :initial-element #\Space) converted-line)) trimmed-line))) 

(defun curly-brace-conversion (line)
      (let* ((space-count (loop for char across line
                                while (char= char #\Space) count 1))
             (trimmed-line (subseq line space-count))
             (converted-line (substitute (code-char 41) (code-char 125) trimmed-line))) 
            (concatenate 'string (make-string space-count :initial-element #\Space) converted-line)))

(defun function-prototype-definition-conversion (line)
      (let* ((space-count (loop for char across line
                                while (char= char #\Space) count 1))
             (trimmed-line (subseq line space-count)) 
             (tokens (split-string trimmed-line))
             (returntype (first tokens)) (funcname (second tokens)) (args (cddr tokens))
             (arg-types (loop for i from 0 below (length args) by 2 collect (data-types (nth i args))))
             (leading-spaces (make-string space-count :initial-element #\Space))
             (converted-line (format nil "~A(declaim (ftype (function (~{~A~^ ~}) ~A) ~A))"
                                    leading-spaces arg-types (data-types returntype) funcname))) converted-line))

(defun function-definition-conversion (line)
      (let* ((space-count (length (loop for char across line
                                        while (char= char #\Space) collect char)))
             (trimmed-line (subseq line space-count)) 
             (tokens (split-string trimmed-line)) 
             (funcname (second tokens)) (args (cddr tokens))  
             (args-list (loop for i from 0 to (1- (length args))
                          when (oddp i) collect (nth i args)))
             (leading-spaces (make-string space-count :initial-element #\Space))
             (converted-line (format nil "~A(defun ~A (~{~A~^ ~})"  leading-spaces funcname args-list))) converted-line))

(defun printf-conversion (args)
      (let* ((initial-format "(format t \"")
             (format-string (substitute #\~ #\% (first args)))  
             (arg-list (if (> (length args) 1) (rest args) '())) 
             (processed-format (if (search "\\n" format-string) (concatenate 'string (string-right-trim "\\n" format-string) "~%") format-string))
             (converted-format (concatenate 'string initial-format processed-format "\""))
             (full-conversion (concatenate 'string converted-format 
                                    (apply #'concatenate 'string (mapcar (lambda (arg) (concatenate 'string " " (princ-to-string arg))) arg-list)) ")"))) full-conversion))

(defun function-call-conversion (line)
      (let* ((space-count (loop for char across line
                                while (char= char #\Space) count 1)) 
             (trimmed-line (subseq line space-count))
             (tokens (split-function-call trimmed-line)) (func-name (first tokens)) (args (rest tokens))
             (core-conversion (if (string= func-name "printf") (printf-conversion args) (format nil "(~A~{~A~^~})" func-name args)))
             (converted-line (concatenate 'string 
                                    (make-string space-count :initial-element #\Space) core-conversion))) converted-line))
(defun variable-definition-conversion (line)
      (let* ((space-count (length (loop for char across line
                                        while (char= char #\Space) collect char)))
             (trimmed-line (subseq line space-count))
             (tokens (split-string trimmed-line))
             (var-type (first tokens)) (var-name (second tokens)) (initial-value (third tokens)))
            (let ((final-initial-value (if (and (string= (aref initial-value (1- (length initial-value))) #\=) (>= (length tokens) 4))
                                           (fourth tokens) initial-value)))
                 (let ((converted-line (format nil "(~A ~A)" var-name final-initial-value)))
                      (concatenate 'string (make-string space-count :initial-element #\Space) converted-line)))))

(defun variable-assignment-conversion (line)
      (let* ((space-count (length (loop for char across line
                                     while (char= char #\Space) collect char)))  
             (trimmed-line (subseq line space-count))       
             (tokens (split-string trimmed-line))          
             (var-name (first tokens)) (assignment (third tokens))                    
             (converted-line (format nil "(setf ~A ~A)" var-name assignment)))  
            (concatenate 'string (make-string space-count :initial-element #\Space) converted-line)))  

(defun variable-definition-by-arithmetic-operation-conversion (line)
      (let* ((space-count (length (loop for char across line
                                     while (char= char #\Space) collect char)))  
             (trimmed-line (subseq line space-count))       
             (tokens (split-string trimmed-line))
             (var-name (second tokens)) (operator (fifth tokens)) (left-var (fourth tokens)) (right-var (sixth tokens))                    
             (converted-line (format nil "(~A (~A ~A ~A))" var-name operator left-var right-var)))
            (concatenate 'string (make-string space-count :initial-element #\Space) converted-line))) 

(defun variable-assignment-by-arithmetic-operation-conversion (line)
      (let* ((space-count (length (loop for char across line
                                        while (char= char #\Space) collect char)))  
             (trimmed-line (subseq line space-count))       
             (tokens (split-string trimmed-line))
             (var-name (first tokens)) (left-var (third tokens)) (operator (arithmetic-operators (fourth tokens))) (right-var (fifth tokens))
             (converted-line (format nil "(setf ~A (~A ~A ~A))" var-name operator left-var right-var)))  
            (concatenate 'string (make-string space-count :initial-element #\Space) converted-line)))

(defun variable-assignment-by-function-return-conversion (line)
      (let* ((space-count (length (loop for char across line
                                    while (char= char #\Space) collect char)))  
             (trimmed-line (subseq line space-count))    
             (tokens (split-string trimmed-line))     
             (var-name (second tokens)) (assignment-operator (third tokens)) (function-name (fourth tokens)) (args (subseq tokens 4))                
             (converted-line (if (string= assignment-operator "=")  
                             (format nil "(~A (~A ~{~A~^ ~}))" var-name function-name args) ""))) 
            (concatenate 'string (make-string space-count :initial-element #\Space) converted-line)))

(defun post-increment-decrement-conversion (line)
      (let* ((space-count (length (loop for char across line
                                        while (char= char #\Space) collect char)))
             (trimmed-line (subseq line space-count)) 
             (converted-line (multiple-value-bind (var-name operator) 
                                    (split-post-increment-decrement trimmed-line)
                                    (let ((converted-operator (arithmetic-operators operator)))  
                                         (format nil "(~A ~A ~A)" converted-operator var-name 1))))) 
            (concatenate 'string (make-string space-count :initial-element #\Space) converted-line)))

(defun arithmetic-operation-conversion (line)
      (let ((space-count (length (loop for char across line
                                       while (char= char #\Space) collect char)))  
            (converted-line (progn
                              (multiple-value-bind (operator left right) (split-logical-arithmetic line)
                                    (if (and operator left right)
                                        (format nil "(~A ~A ~A)" (arithmetic-operators operator) left right) nil))))) 
            (concatenate 'string (make-string space-count :initial-element #\Space) converted-line)))

(defun logical-operation-conversion (line)
      (let ((space-count (length (loop for char across line
                                       while (char= char #\Space) collect char))))
           (multiple-value-bind (operator left right) (split-logical-arithmetic line)
                  (if (and operator left right)
                      (let* ((lisp-operator (logical-operators operator)) 
                             (converted-line (format nil "(~A ~A ~A)" lisp-operator left right)))
                            (concatenate 'string (make-string space-count :initial-element #\Space) converted-line))))))

(defun return-statement-conversion (line)
  (let ((space-count (length (loop for char across line
                                    while (char= char #\Space) collect char))))
       (let ((trimmed-line (string-trim " " line)))
            (when (and (>= (length trimmed-line) 6)  
                       (string= (subseq trimmed-line 0 6) "return")) 
                  (let* ((number (string-trim " " (subseq trimmed-line 6)))
                         (number (car (remove-if (lambda (token) (string= token ";")) (split-string number))))) 
                        (when number (concatenate 'string (make-string space-count :initial-element #\Space) number)))))))

(defun unknown-conversion (line)"")

(defun conversion-foo (line-type)
      (case line-type
            ('if-statement 'if-conversion)
            ('for-loop 'for-conversion)
            ('while-loop 'while-conversion)
            ('curly-brace 'curly-brace-conversion)
            ('post-increment-decrement 'post-increment-decrement-conversion)
            ('post-increment-decrement-return 'post-increment-decrement-conversion)
            ('variable-assignment-by-arithmetic-operation 'variable-assignment-by-arithmetic-operation-conversion)
            ('variable-definition-by-arithmetic-operation 'variable-definition-by-arithmetic-operation-conversion)
            ('variable-assignment-by-function-return 'variable-assignment-by-function-return-conversion)
            ('function-prototype-definition 'function-prototype-definition-conversion)
            ('function-call 'function-call-conversion)
            ('function-definition 'function-definition-conversion)
            ('variable-definition 'variable-definition-conversion)
            ('variable-assignment 'variable-assignment-conversion)
            ('return-statement 'return-statement-conversion)
            ('arithmetic-operation-return 'arithmetic-operation-conversion)
            ('arithmetic-operation 'arithmetic-operation-conversion)
            ('logical-operation-return 'logical-operation-conversion)
            ('logical-operation 'logical-operation-conversion)
            ('return-statement 'return-statement-conversion)
            ('unknown 'unknown-conversion)))

(defun convert (line conversion-fn) (funcall conversion-fn line))

(defun read-file (filename)
      (labels ((process-lines (in) 
                  (multiple-value-bind 
                        (line eof) (read-line in nil) 
                        (if line
                              (cons (convert line (conversion-foo (line-type line)) ) (process-lines in)) nil)))) 
              (with-open-file (in filename :direction :input) (process-lines in))))
            
(defun write-file (filename lines)
      (with-open-file (out filename :direction :output :if-exists :supersede) 
                      (dolist (line lines) (format out "~a~%" line))))

(write-file "output.lisp" (read-file "input.c"))
