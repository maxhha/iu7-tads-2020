.PHONY : init all run debug runmem gcov clean func unit dev fdev udev

CC := gcc
CFLAGS := --std=gnu99 -Wall -Werror -Wfloat-equal -Wfloat-conversion -Wpedantic
CFLAGS += $(cflags)
SRCS := $(filter-out $(wildcard src/*.tmpl.c), $(wildcard src/*.c))
OBJS := $(SRCS:src/%.c=out/%.o)

UNITCFLAGS := -lcheck -lpthread -lrt
UNITSRCS := $(wildcard unit_tests/*.c)
UNITOBJS := $(UNITSRCS:unit_tests/%.c=out/%.o)

all : app.exe

init :
	mkdir inc src func_tests unit_tests

run : app.exe
	./app.exe $(args)

app.exe : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

debug func: CFLAGS += -g -fno-inline -fno-omit-frame-pointer -O0 --coverage
debug : all

runmem : debug
	drmemory -- ./app.exe $(args)

gcov :
	@rm -f out/*.c.gcov
	@echo
	@gcov -d out/*.gcda | \
		sed -u \
			-Ee "s/File ('[^']+?')/File \x1B[0;33m\1\x1B[0m/g" \
			-e "s/:([0-8]?[0-9]\.[0-9]+% of [0-9]+?)/:\x1B[0;31m\1\x1B[0m/g" \
			-e "s/:(9[0-9]+\.[0-9]+% of [0-9]+?)/:\x1B[0;32m\1\x1B[0m/g"

	@mv *.c.gcov out

func : app.exe
	@./func_tests/test.sh ./app.exe

unit : unit_tests.exe
	@./unit_tests.exe | \
		sed -u \
			-Ee "s/.+ Passed/\x1B[0;32m&\x1B[0m/g" \
			-Ee "s/.+ Assertion.+/\x1B[0;31m&\x1B[0m/g"

unit_tests.exe : $(UNITOBJS) $(filter-out out/main.o, $(OBJS))
	$(CC) $(CFLAGS) $(flags) -o $@ $^ $(UNITCFLAGS)

out :
	mkdir out

out/%.d: ./src/%.c | ./out
	@set -e; rm -f $@; \
		$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,out/\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

out/check_%.o : unit_tests/check_%.c | ./out
	$(CC) $(CFLAGS) $(flags) -o $@ -c $<

out/%.o : ./src/%.c | ./out
	$(CC) $(CFLAGS) $(flags) -o $@ -c $<

clean :
	@rm -f *.exe out/*.o out/*.d out/*.gcda out/*.gcno

dev :
	@clear
	@make -s run
	@inotifywait -e close_write -mq src/ inc/ | \
		while read -r dir evts file; do clear; make -s run; done

fdev:
	@clear
	@make -s func gcov
	@inotifywait -e close_write -mq func_tests/make_tests.sh func_tests/test.sh src/ inc/ | \
		while read -r dir evts file; do clear; make -s func gcov; done

udev:
	@clear
	@rm -f out/*.gcda
	@make -s debug unit flags="-g -O0 --coverage" gcov
	@inotifywait -e close_write -mq src/ inc/ unit_tests/ | \
		while read -r dir evts file; do \
			rm -f out/*.gcda; \
			clear; \
			make -s debug unit gcov flags="-g -O0  --coverage"; \
		done

-include $(SRCS:src/%.c=out/%.d)
