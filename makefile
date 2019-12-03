all: sleeping_barber.c sem.c sem.h 
	gcc sleeping_barber.c sem.c sem.h -o rr
clean:
	rm -f rr

