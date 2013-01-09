all: master
master: master.o
	gcc -o $@ $^ -lpthread
.c.o: 
	gcc -c $<
clean:
	rm *.o master
