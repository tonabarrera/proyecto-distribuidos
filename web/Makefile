run:server
	./server
server:server.o mongoose.o
	gcc server.o mongoose.o -o server
server.o:
	gcc -c -Wall server.c
mongoose.o:
	gcc -c -Wall mongoose.c