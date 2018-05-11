NAME = expert_system

FLAGS = -Wfatal-errors -std=c++14

SRC =  ./src/*.cpp

INC = -I ./src

all:
	@g++ $(FLAGS) -o $(NAME) $(SRC) $(INC)

fclean:
	rm $(NAME)
