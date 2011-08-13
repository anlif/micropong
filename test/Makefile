C_FLAGS := -std=c99
LIBS := -L/usr/lib -lSDL

main_host_out: main_host.o hw_host.o draw.o pong.o 
	gcc -o main_host_out $(LIBS) main_host.o hw_host.o draw.o pong.o

main_host.o: hw_host.o draw.o pong.o main_host.c
	gcc -c $(C_FLAGS) main_host.c 

hw_host.o: draw.o hw_host.c hw_host.h
	gcc -c $(C_FLAGS) hw_host.c 

pong.o: draw.o ../pong.c ../pong.h
	gcc -c $(C_FLAGS) ../pong.c

draw.o: ../draw.c ../draw.h
	gcc -c $(C_FLAGS) ../draw.c

clean:
	rm -f *.o main_host_out
