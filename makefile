default: shell.o cmd.o
	gcc -o test shell.o cmd.o
shell.o: shell.c cmd.h
	gcc -c shell.c
cmd.o: cmd.c cmd.h
	gcc -c cmd.c
run:
	./test
clean:
	rm -f *out test* *exe *o *~
fun:
	echo NORMAL TEST
	ls
	ls -a -l
	echo CD TEST
	cd ..
	pwd
	echo SEMICOLON TEST
	ps ; echo HELLO
	echo STDOUT REDIRECT TEST
	w > wholist
	cat wholist
	echo STDIN REDIRECT TEST
	tr a-z A-Z < wholist
	echo DOUBLE REDIRECT TEST
	tr a-z A-Z < wholist > foo
	cat foo
	echo PIPE TEST
	ls | wc
	rm wholist foo
	echo EXIT
	exit
