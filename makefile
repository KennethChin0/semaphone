all: control.o write.o
	gcc -o control control.o
	gcc -o write write.o

control.o: control.c semaphone.h
	gcc -c control.c

write.o: write.c semaphone.h
	gcc -c write.c

clean:
	rm story.txt
	rm *.o
