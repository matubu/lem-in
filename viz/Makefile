Name = viz
Sources = main.cpp parse.cpp
#Flags = -Wall -Wextra -Werror -O3
Flags = -O3
# Flags += -Wall -Wextra -Werror
Flags += -lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ $(Sources) -o $(Name) $(Flags)

clean:

fclean: clean
	rm -rf $(Name)

re: fclean all

run: all
	./$(Name)

.PHONY: all clean fclean re run