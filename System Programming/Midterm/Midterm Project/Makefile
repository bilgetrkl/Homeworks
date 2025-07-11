FLAGS = -Wall -Wextra -Werror

all: compile

compile:
	@gcc server.c -o server $(FLAGS)
	@gcc client.c -o client $(FLAGS)

clean:
	@rm -f ServerFifo
	@rm -f ClientFifo
	@rm -f server
	@rm -f client

re: clean compile
