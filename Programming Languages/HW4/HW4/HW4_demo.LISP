;; Axioms
(let (
	(axioms '(
		(("father" "jim" "jill"))
		(("mother" "anna" "jill"))
		(("father" "sam" "jim"))
		(("mother" "lisa" "anna"))
		(("father" "john" "sam"))
		(("parent" "X" "Y") "<" ("father" "X" "Y"))
		(("parent" "X" "Y") "<" ("mother" "X" "Y"))
		(("ancestor" "X" "Y") "<" ("parent" "X" "Y"))
		(("ancestor" "X" "Y") "<" ("parent" "X" "Z") ("ancestor" "Z" "Y"))))
	(query1 '( ("ancestor" "X" "jill") ) )
	(query2 '( ("parent" "X" "jill") ) ))
	(prolog_prove axioms query1)
;;	(prolog_prove axioms query2)
)


;; output: (("X" "jim") ("X" "anna") ("X" "sam") ("X" "lisa") ("X" "john"))
;; output-2: (("X" "jim") ("X" "anna"))