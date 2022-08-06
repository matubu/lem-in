Name         = lem_in
#Flags        = -Wall -Wextra -Werror -O3 -include src/def.h
Flags        = -include src/def.h
Flags       += -fsanitize=address -g
Sources      = $(wildcard src/*.c)
Dependencies = $(wildcard src/*.h) Makefile
Objects      = $(patsubst src/%.c,bin/%.o,$(Sources))

all: $(Name)

bin/%.o: src/%.c $(Dependencies)
	mkdir -p bin
	gcc $(Flags) $< -o $@ -c

$(Name): $(Objects)
	gcc $(Flags) $^ -o $@

re: fclean all

clean:
	rm -rf bin

fclean: clean
	rm -rf $(Name)

run: $(Name)
	./$(Name)

.PHONY: all re clean fclean run