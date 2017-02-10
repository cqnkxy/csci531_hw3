hw3: hw3.o utility.o KeyGenerator.o
	g++ -W hw3.o utility.o KeyGenerator.o -o hw3 -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include

KeyGenerator.o: KeyGenerator.h KeyGenerator.cpp
	g++ -W -c KeyGenerator.cpp -I/usr/local/opt/openssl/include

utility.o: utility.h utility.cpp
	g++ -W -c utility.cpp -I/usr/local/opt/openssl/include

hw3.o: hw3.cpp
	g++ -W -c hw3.cpp -I/usr/local/opt/openssl/include

clean:
	rm -f *.o *.gch hw3
