# change application name here (executable output name)
TARGET=Dictionary
 
# compiler
CC=gcc

main: src/main.c
	$(CC) -w -o $(TARGET) src/main.c src/dcallback.c src/soundex.c src/bt-5.0.0/lib/libbt.a -pthread `pkg-config gtk+-3.0 --cflags --libs` -export-dynamic
run: main
	./Dictionary 
clean:
	rm -f Dictionary && rm -f resource.dat
