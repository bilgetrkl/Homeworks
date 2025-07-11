all: 
	@yacc -d gpp_interpreter.y
	@flex gpp_lexer.l
	@gcc -o gpp_interpreter y.tab.c lex.yy.c
	@echo "Run ./gpp_interpreter or ./gpp_interpreter input.gpp to start the interpreter."


clean:
	@rm -f gpp_interpreter y.tab.c y.tab.h lex.yy.c