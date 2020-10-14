.PHONY : all run debug runmem gcov clean func check unit

CC := gcc
CFLAGS := --std=c99 -Wall -Werror -Wfloat-equal -Wfloat-conversion -Wpedantic
CFLAGS += $(cflags)
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=out/%.o)

UNITCFLAGS := -lcheck -lpthread -lrt
UNITSRCS := $(wildcard unit_tests/*.c)
UNITOBJS := $(UNITSRCS:unit_tests/%.c=out/%.o)

CODECHECKERDIR := ../../CodeChecker/CodeChecker
CODECHECKER := $(CODECHECKERDIR)/CodeChecker.exe --rules $(CODECHECKERDIR)/Rules.txt

all : app.exe

run : app.exe
	./app.exe $(args)

app.exe : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug : CFLAGS += -g -fno-inline -fno-omit-frame-pointer -O0 --coverage
debug : all

runmem : debug
	drmemory -- ./app.exe $(args)

gcov :
	@rm -f out/*.c.gcov
	@gcov -d out/*.gcda
	@mv *.c.gcov out

func : app.exe
	@./func_tests/test.sh ./app.exe

unit : unit_tests.exe
	./unit_tests.exe

unit_tests.exe : $(UNITOBJS) $(filter-out out/main.o, $(OBJS))
	$(CC) $(CFLAGS) -o $@ $^ $(UNITCFLAGS)

out/%.d: ./src/%.c
	@set -e; rm -f $@; \
		$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,out/\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

out/check_%.o : unit_tests/check_%.c
	$(CC) $(CFLAGS) -o $@ -c $<

out/%.o : ./src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	@rm -f *.exe out/*.o out/*.d out/*.gcda out/*.gcno

check : SHELL:=/usr/bin/bash
check :
	$(CODECHECKER) $(SRCS)

-include $(SRCS:src/%.c=out/%.d)
