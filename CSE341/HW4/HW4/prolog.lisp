(defun find_fact (facts query)
  (if (null facts) nil
    (if (and (string= (first (car facts)) (first query)) (string= (second (car facts)) (second query))
            (string= (third (car facts)) (third query))) (car facts) (find_fact (cdr facts) query))))

(defun find_matching_facts (facts query predicate_type)
  (if (null facts) nil
    (let ((first_exp (first query)) (last_exp (third query)))
          (if (or (and (= predicate_type 1) (string= (first (car facts)) first_exp) (string= (third (car facts)) last_exp))
                  (and (= predicate_type 2) (string= (first (car facts)) first_exp) (string= (second (car facts)) (second query))))
              (cons (if (= predicate_type 1) (second (car facts)) (third (car facts))) (find_matching_facts (cdr facts) query predicate_type))
              (find_matching_facts (cdr facts) query predicate_type)))))

(defun queries (list1 list2)
	(let ((new_list '()))
		(loop for i from 0 below (length list1) do
			(if (member (nth i list1) list2 :test #'string=)
				(setq new_list (append new_list (list (nth i list1)))))) new_list))

(defun construct_subquery (rule query predicate_type)
  (let* ((var1 (if (= predicate_type 1) (second (first rule)) (third (first rule))))
         (var2 (if (= predicate_type 1) (third (first rule)) (second (first rule)))))
    (loop for i from 2 below (length rule) do
          (if (string= (second (nth i rule)) var2)
              (return (list 
                (list (first (nth i rule)) (second query) (third (nth i rule)))
                (nth (if (= i 2) 3 2) rule) 2)))
          (if (string= (third (nth i rule)) var2)
              (return (list 
                (list (first (nth i rule)) (second (nth i rule)) (third query))
                (nth (if (= i 2) 3 2) rule) 1))))))

(defun construct_extended_query (rule subquery var)
  (let ((predicate_type (if (string= (second rule) var) 1 2)))
    (if (= predicate_type 1)
        (list (first rule) (second rule) subquery)
        (list (first rule) subquery (third rule)))))

(defun resolve_subqueries (facts rules query query_type)
  (let ((result (find_matching_facts facts query query_type)))
    (loop for rule in rules do
          (if (string= (first (first rule)) (first query))
              (cond
               ((= (length rule) 3)
                (setq result (append_unique_results result (resolve_subqueries facts rules 
                                  (list (first (third rule))  (second query) (third query)) query_type))))
               ((= (length rule) 4)
                (let* ((elements (construct_subquery rule query query_type))
                       (new_query (first elements)) (new_rule (second elements))
                       (new_query_type (third elements)) (remaining_rules (remove rule rules))
                       (subqueries (resolve_subqueries facts remaining_rules new_query new_query_type)))
                  (dolist (resolved_item subqueries)
                    (setq result (append_unique_results result
                                            (resolve_subqueries facts rules 
                                                      (construct_extended_query new_rule resolved_item (second query)) new_query_type))))))))) result))
(defun process_query (facts rules query)
    (let ((char1 (subseq (first query) 0 1)) (char2 (subseq (second query) 0 1)) (char3 (subseq (third query) 0 1)))
      (if (string= char1 (string-downcase char1))
          (if (and (string= char2 (string-downcase char2)) (string= char3 (string-downcase char3)))
              (if (null (find_fact facts query)) nil (list (list "TRUE")))
              (if (string= char2 (string-upcase char2)) (resolve_subqueries facts rules query 1)
                  (resolve_subqueries facts rules query 2))))))

(defun append_unique_results (list1 list2)
  (dolist (element list2 list1) 
    (unless (member element list1 :test #'equal) 
      (setf list1 (append list1 (list element))))))

(defun prolog_prove (axioms query)
  (let ((facts '()) (rules '()))
    (loop for i from 0 below (length axioms) do
          (if (= (length (nth i axioms)) 1)
              (setf facts (append facts (nth i axioms)))
              (setf rules (append rules (list (nth i axioms))))))
    (let ((results '()))
      (dolist (q query)
        (let ((res (process_query facts rules q)))
          (when (null res)
              (return-from prolog_prove nil))
          (setq results (if (null results) res
                            (queries results res))))) results)))

(let ((axioms '(
       ( ("father" "jim" "jill") )
       ( ("mother" "mary" "jill") )
       ( ("father" "samm" "jim") )
       ( ("ancestor" "X" "Y") "<" ("parent" "X" "Y") )
       ( ("ancestor" "X" "Y") "<" ("ancestor" "X" "Z") ("ancestor" "Z" "Y") )
       ( ("parent" "X" "Y") "<" ("mother" "X" "Y") )
       ( ("parent" "X" "Y") "<" ("father" "X" "Y") )))
      (query1 '(("ancestor" "X" "jill")))
      (query2 '(("ancestor" "X" "jill") ("mother" "X" "bob"))))

(print (prolog_prove axioms query1))
(print (prolog_prove axioms query2)))