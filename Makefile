OBJS = main.o MainAux.o Shared.o IO.o
OBJS += dataStructures/Activity.o dataStructures/Puzzle.o
OBJS += parser/Commands.o parser/Parser.o
OBJS += algs/SudokuAlgs.o algs/exhBacktr.o algs/ILPSolver.o
OBJS += utils/EnumSubset.o utils/Strings.o
OBJS += utils/dataStructures/DoublyLinkedList.o utils/dataStructures/Stack.o

EXEC = sudoku-console

GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

COMP_FLAG = -ansi -O2 -Wall -Wextra -Werror -pedantic-errors

CC = gcc

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm

.PHONY: clean cleanobj cleanlog rebuild all

rebuild: clean
	$(MAKE)

all:
	$(MAKE)

clean: cleanobj cleanlog
	rm -f $(EXEC)

cleanobj:
	find . -type f -name '*.o' -delete

cleanlog:
	find . -type f -name '*.log' -delete
	find . -type f -name '*.lp' -delete

main.o: MainAux.h Strings.h
	$(CC) $(COMP_FLAG) $*.c -c

MainAux.o: MainAux.h Shared.h Strings.h dataStructures/Puzzle.h parser/Parser.h
	$(CC) $(COMP_FLAG) $*.c -c

Shared.o: Shared.h
	$(CC) $(COMP_FLAG) $*.c -c

IO.o: IO.h utils/MemAlloc.h
	$(CC) $(COMP_FLAG) $*.c -c

dataStructures/Activity.o: dataStructures/Activity.h utils/MemAlloc.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c

dataStructures/Puzzle.o: dataStructures/Puzzle.h utils/MemAlloc.h MainAux.h algs/SudokuAlgs.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c

parser/Commands.o: parser/Commands.h IO.h Shared.h Strings.h algs/SudokuAlgs.h dataStructures/Activity.h dataStructures/Puzzle.h utils/MemAlloc.h utils/Strings.h parser/Parser.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c

parser/Parser.o: parser/Parser.h parser/Commands.h utils/MemAlloc.h utils/Strings.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c

algs/SudokuAlgs.o: algs/SudokuAlgs.h algs/exhBacktr.h algs/ILPSolver.h utils/MemAlloc.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c

algs/exhBacktr.o: algs/exhBacktr.h algs/SudokuAlgs.h utils/dataStructures/Stack.h utils/MemAlloc.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c

algs/ILPSolver.o: algs/ILPSolver.h Strings.h utils/MemAlloc.h
	$(CC) -o $@ -c $(COMP_FLAG) $(GUROBI_COMP) $(basename $@).c

utils/EnumSubset.o: utils/EnumSubset.h utils/MemAlloc.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c

utils/Strings.o: utils/Strings.h utils/MemAlloc.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c

utils/dataStructures/DoublyLinkedList.o: utils/dataStructures/DoublyLinkedList.h utils/MemAlloc.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c

utils/dataStructures/Stack.o: utils/dataStructures/Stack.h utils/MemAlloc.h
	$(CC) -o $@ -c $(COMP_FLAG) $(basename $@).c
