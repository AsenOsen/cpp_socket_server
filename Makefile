main: build clean
	echo "Build successful!"

build: headers app
	g++ app.o server_socket.o -o socket

app:
	g++ -c app.cpp -o app.o

headers:
	g++ -c server_socket.cpp -o server_socket.o

clean:
	rm -f *.o
