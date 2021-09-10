bin/vida:obj/main.o obj/util.o
	gcc obj/main.o obj/util.o -o $@ -pthread 
obj/main.o:src/main.c include/vida.h include/util.h include/registro.h
	gcc -Wall -c -L include/ src/main.c -o $@ 
obj/util.o:src/util.c include/vida.h include/util.h include/registro.h 
	gcc -c src/util.c -L include/ -o $@
.PHONY:clean
clean:
	rm -rf obj
	rm -rf bin
	mkdir obj
	mkdir bin
