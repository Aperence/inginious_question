CC=gcc
CCFLAGS= -std=gnu99 -g
LIBS= -lcunit
HEADER=-I$(HOME)/local/include
LINK=-L$(HOME)/local/lib

inOrder: inOrder.o inOrder.h
	$(CC) $(CCFLAGS) -o $@ $<

tests: tests.o inOrder.o
	$(CC) $(CCFLAGS) $(HEADER) -o $@ $(LINK) $^ $(LIBS)

error1: tests.o ./errors_examples/error1.o
	$(CC) $(CCFLAGS) $(HEADER) -o $@ $(LINK) $< error1.o $(LIBS)

error2: tests.o ./errors_examples/error2.o
	$(CC) $(CCFLAGS) $(HEADER) -o $@ $(LINK) $< error2.o $(LIBS)

error3: tests.o ./errors_examples/error3.o
	$(CC) $(CCFLAGS) $(HEADER) -o $@ $(LINK) $< error3.o $(LIBS)

%.o: %.c
	@$(CC) $(CCFLAGS) $(HEADER) -c $< 

clean:
	@rm -f ./*.o
	@rm -f ./tests
	@rm -f ./inOrder
	@rm -f ./error1
	@rm -f ./error2
	@rm -f ./error3

.PHONY:
	clean

