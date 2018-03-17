CC=gcc

CFLAGS=-Wall -g -pedantic -pipe -W  -Wpointer-arith -Wno-unused-parameter -Wunused-function -Wunused-variable -Wunused-value -Werror

INCLUDES=-I.

LIBS=

LINKS=

TARGET=jv_timer_main

TEST=jv_timer_test

all:	
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_rbtree.c -o jv_rbtree.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_timer.c -o jv_timer.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_timer_main.c -o jv_timer_main.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_timer_test.c -o jv_timer_test.o
	
	$(CC) -o $(TARGET) jv_timer_main.o jv_rbtree.o jv_timer.o $(LINKS) $(LIBS)
	$(CC) -o $(TEST) jv_timer_test.o jv_rbtree.o jv_timer.o $(LINKS) $(LIBS)

	@echo 
	@echo Project has been successfully compiled.
	@echo

clean:
	rm -rf $(TARGET) $(TEST) *.depend *.layout bin obj *.o *.stackdump *.exe *.log *~