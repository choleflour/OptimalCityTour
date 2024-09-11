CC=clang
CFLAGS= -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
OBJS=graph.o path.o stack.o tsp.o
# OBJS=graph.o stack.o tsp.o
# OBJS=stack.o
LFLAGS=
HEAD=graph.h path.h stack.h
HEAD=graph.h stack.h
# HEAD=stack.h
EXEC=tsp
# EXEC = stack_test

.PHONY: clean format scan-build

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

%.o: %.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

# stack_test: stack.o stack_test.o
# 	$(CC) $^ $(LFLAGS) -o $@

# %.o: %.c $(HEAD)
# 	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) *.o

scan-build: clean
	scan-build --use-cc=clang make

format:
	clang-format -i -style=file *.[ch]

