Name         = lem_in
#Flags        = -Wall -Wextra -Werror -O3
Flags        = -O3
Flags       += -I src
Flags       += -fsanitize=address -g
Sources      = $(shell find src/ -type f -name '*.c')
Dependencies = $(shell find src/ -type f -name '*.h') Makefile
Objects      = $(patsubst src/%.c,bin/%.o,$(Sources))
Jobs         = 16

Print        = echo -e
LogPre       = \r\x1b[0K\x1b[90m[ 
LogPost      = \x1b[0;90m ]\x1b[0m

all:
	@make --jobs $(Jobs) --no-print-directory $(Name)

bin/%.o: src/%.c $(Dependencies)
	@mkdir -p $(dir $@)
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

.PHONY: all re clean fclean run

update_test_ref:
	cp .test_log .test_log_ref

test: all
	@for file in map/tests/*; do \
		echo -e "\n\n=== $$file ==="; \
		./$(Name) $$file; \
	done > .test_log 2>&1
	@echo -e ' - Right click select to compare \x1b[4m./.test_log\x1b[0m'
	@echo -e ' - Right click compare with selected \x1b[4m./.test_log_ref\x1b[0m'

.PHONY: update_test_ref test

gen:
	cd gen && cargo run

.PHONY: gen