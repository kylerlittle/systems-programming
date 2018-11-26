run:
	cc -o mycgi.bin util.o cmd/*.c cmd.c mycgi.c   # generate YOUR OWN mycgi executable.
	chmod u+s mycgi.bin              # chmod mycgi to a SETUID executable

clean:
	find . -type f -name '*~' -exec rm {} \;