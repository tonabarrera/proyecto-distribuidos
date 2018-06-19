servidor:websocket_chat.o SocketDatagrama.o PaqueteDatagrama.o mongoose.o
	g++ websocket_chat.o SocketDatagrama.o PaqueteDatagrama.o mongoose.o -o servidor
SocketDatagrama.o: SocketDatagrama.cpp
	g++ SocketDatagrama.cpp -c -Wall
PaqueteDatagrama.o: PaqueteDatagrama.cpp
	g++ PaqueteDatagrama.cpp -c -Wall
mongoose.o:mongoose.c
	gcc mongoose.c -c -Wall
websocket_chat.o:websocket_chat.cpp
	g++ websocket_chat.cpp -c -Wall
clean:
	rm ./web *.o
