CC = gcc
OPT = -lpthread

all: clean svr_c svr_s 

svr_c: svr_c.c 
	$(CC) svr_c.c -o $@ 
#	rm -rf *.o *.dSYM 
 
svr_s: svr_s.c
	$(CC) svr_s.c -o $@ $(OPT) 
#	rm -rf *.o *.dSYM 

#.PHONY: clean

clean: 
	rm -rf *.o *.dSYM *~ svr_c svr_s reg.txt