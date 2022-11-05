Name = viz
Sources = main.cpp
Flags = -Wall -Wextra -Werror -O3
Flags += -lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ $(Sources) -o $(Name) $(Flags)

run: all
	./$(Name)

.PHONY: all run