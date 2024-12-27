LDFLAGS+=-pthread

server : server.o pool.o queue.o media_lib.o
	gcc -o $@ $^ $(LDFLAGS)
clean :
	rm -rf *.o server
