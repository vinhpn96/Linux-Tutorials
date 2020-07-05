all: main.c text.txt
	ld -r -b binary -o text.o text.txt
	gcc -o main main.c text.o
clean:
	rm main
	rm text.o
