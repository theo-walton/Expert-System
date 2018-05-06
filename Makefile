NAME = expert_system

FLAGS = -Wfatal-errors

SRC =  ./src/*.cpp

INC = -I ./src

all:
	@g++ $(FLAGS) -o $(NAME) $(SRC) $(INC)

fclean:
	rm $(NAME)
