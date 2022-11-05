Name = viz
Sources = main.cpp
Flags = -O3
# Flags += -Wall -Wextra -Werror
Flags += -lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ $(Sources) -o $(Name) $(Flags)

run: all
	./$(Name)

.PHONY: all run