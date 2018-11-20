default: shell.o cmd.o
	gcc -o test shell.o cmd.o
shell.o: shell.c cmd.h
	gcc -c shell.c
cmd.o: cmd.o cmd.h
	gcc -c cmd.c
run:
	./test
clean:
	rm -f *out test* *exe *o *~
