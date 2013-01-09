all: server
server: server.o dbapi.o
	gcc -o $@ $^ -ltokyocabinet -lpthread
.c.o: 
	gcc -c $<
clean:
	rm *.o server
