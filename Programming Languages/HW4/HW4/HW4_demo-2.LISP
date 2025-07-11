;; Axioms
(let (
	(axioms '(
		(("manager" "alice" "bob"))
		(("manager" "bob" "charlie"))
		(("boss" "X" "Y") "<" ("manager" "X" "Y"))
		(("boss" "X" "Y") "<" ("manager" "X" "Z") ("boss" "Z" "Y"))))
	(query1 '( ("boss" "X" "charlie") ) ))
)

;; output: (("X" "bob") ("X" "alice"))