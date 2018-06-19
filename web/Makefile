cliente:cliente.o mongoose.o
	gcc cliente.o mongoose.o -o cliente
server:server.o mongoose.o
	gcc server.o mongoose.o -o server
server.o:
	gcc -c -Wall server.c
cliente.o:
	gcc -c -Wall cliente.c
mongoose.o:
	gcc -c -Wall mongoose.c
clean:
	rm *.o ./server ./cliente