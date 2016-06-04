all: llist.o rsort.o
		gcc llist.o rsort.o -o all
llist.o:  list.h llist.c
		gcc -c llist.c
rsort.o: list.h rsort.c
		gcc -c rsort.c	