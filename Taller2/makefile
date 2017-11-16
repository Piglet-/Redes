CC = gcc
OPT = -g -Wall -lpthread

all: clean svr_c svr_s 

svr_c: svr_c.c 
	$(CC) $(OPT) -o $@ svr_c.c 
	rm -rf *.o *.dSYM 
 
svr_s: svr_s.c
	$(CC) $(OPT) -o $@ svr_s.c 
	rm -rf *.o *.dSYM 

#.PHONY: clean

clean: 
	rm -rf *.o *.dSYM *~