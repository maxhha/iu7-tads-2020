.PHONY : all run debug runmem gcov clean func check unit

CC := gcc
CFLAGS := --std=c99 -Wall -Werror -Wfloat-equal -Wfloat-conversion -Wpedantic
CFLAGS += $(cflags)
OUTDIR := ./out
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=$(OUTDIR)/%.o)

UNITCFLAGS := -lcheck -lpthread -lrt
UNITSRCS := $(wildcard unit_tests/*.c)
UNITOBJS := $(UNITSRCS:unit_tests/%.c=$(OUTDIR)/%.o)

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
	@rm -f $(OUTDIR)/*.c.gcov
	@gcov -d $(OUTDIR)/*.gcda
	@mv *.c.gcov $(OUTDIR)

func : app.exe
	@./func_tests/test.sh ./app.exe

unit : unit_tests.exe
	./unit_tests.exe

unit_tests.exe : $(UNITOBJS) $(filter-out $(OUTDIR)/main.o, $(OBJS))
	$(CC) $(CFLAGS) -o $@ $^ $(UNITCFLAGS)

$(OUTDIR)/check_%.o : unit_tests/check_%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTDIR)/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	@rm -f *.exe $(OUTDIR)/*.o $(OUTDIR)/*.gcda $(OUTDIR)/*.gcno

check : SHELL:=/usr/bin/bash
check :
	$(CODECHECKER) $(SRCS)
