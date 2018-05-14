NAME = expert_system

FLAGS = -Wfatal-errors -std=c++14 -O3 -march=native

SRC =  ./src/*.cpp

INC = -I ./src

all:
	@g++ $(FLAGS) -o $(NAME) $(SRC) $(INC)

fclean:
	rm $(NAME)
