Name         = lem_in
Flags        = -Wall -Wextra -Werror -O3 -include src/def.h
# Flags        = -include src/def.h
Flags       += -fsanitize=address -g
Sources      = $(wildcard src/*.c)
Dependencies = $(wildcard src/*.h) Makefile
Objects      = $(patsubst src/%.c,bin/%.o,$(Sources))
Jobs         = 16

Print        = echo -e
LogPre       = \r\x1b[0K\x1b[90m[ 
LogPost      = \x1b[0;90m ]\x1b[0m

all:
	@make --jobs $(Jobs) --no-print-directory $(Name)

bin/%.o: src/%.c $(Dependencies)
	@mkdir -p bin
	@gcc $(Flags) $< -o $@ -c
	@$(Print) -n "$(LogPre)\x1b[1;92mCOMPIL$(LogPost)" $<

$(Name): $(Objects)
	@gcc $(Flags) $^ -o $@
	@$(Print) "\n$(LogPre)\x1b[1;95mLINKED$(LogPost)" $@

re: fclean all

clean:
	@rm -rf bin
	@$(Print) "$(LogPre)\x1b[1;91mDELETE$(LogPost)" bin

fclean: clean
	@rm -rf $(Name)
	@$(Print) "$(LogPre)\x1b[1;91mDELETE$(LogPost)" $(Name)

run: all
	@$(Print) "$(LogPre)\x1b[1;96mLAUNCH$(LogPost)" ./$(Name)
	@./$(Name)

.PHONY: all all_no_opt re clean fclean run