Name         = lem_in
#Flags        = -Wall -Wextra -Werror -O3 -include def.h
Flags = -include def.h
Flags       += -fsanitize=address -g
Sources      = $(wildcard *.c)
Dependencies = $(wildcard *.h) Makefile
Objects      = $(addprefix bin/, $(Sources:.c=.o))

all: $(Name)

bin/%.o: %.c $(Dependencies)
	mkdir -p bin
	gcc $(Flags) $< -o $@ -c

$(Name): $(Objects)
	gcc $(Flags) $^ -o $@

re: fclean all

clean:
	rm -rf $(Objects)

fclean:
	rm -rf $(Name)

run: $(Name)
	./$(Name)

.PHONY: all re clean fclean run