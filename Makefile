all:
	gcc pi.c -o findPI -lpthread -lgmpxx -lgmp
Clean:
	bash clean.bash
